#include "LDNI.h"

#include <QOpenGLFunctions_4_5_Core>
#include <QFileDialog>

#include "ui_LDNI.h"
#include "VBOLayout.h"

LDNI::LDNI(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::LDNI();
	ui->setupUi(this);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LoadMesh()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Run()));
}

LDNI::~LDNI()
{
	delete ui;
}

void LDNI::LoadMesh()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"./",
		tr("Meshes (*.ply *.stl)"));

	ui->openGLWidget->LoadMesh(fileName.toStdString());
}

void LDNI::Run()
{
	ui->openGLWidget->RunLDNI();
}

LDNI::Screen::Screen(QWidget *parent)
	: QOpenGLWidget(parent),
	vao(0),
	vbo(0),
	ibo(0),
	program(0),
	fbo(0)
{
	width = 1024;
	height = 1024;

	QVector3D size(100.0f, 75.0f, 100.0f);

	eye = QVector3D(size.x() / 2.0f, size.y() / 2.0f, 100.0f);
	center = QVector3D(size.x() / 2.0f, size.y() / 2.0f, 0.0f);
	up = QVector3D(0.0f, 1.0f, 0.0f);

	view.lookAt(eye, center, up);
	proj.ortho(-size.x() / 2.0f, size.x() / 2.0f,
		-size.y() / 2.0f, size.y() / 2.0f,
		0.0f, 100.0f);
}

LDNI::Screen::~Screen()
{
	makeCurrent();

	delete vao;
	delete vbo;
	delete ibo;
	delete program;
	delete fbo;
}

void LDNI::Screen::LoadMesh(const std::string & filePath_)
{
	makeCurrent();

	mesh.Read(filePath_);
	TriMesh::Point boxMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	TriMesh::Point boxMin(FLT_MAX, FLT_MAX, FLT_MAX);
	for (TriMesh::VertexIter vit = mesh.vertices_begin(); 
		vit != mesh.vertices_end(); 
		vit++) {
		TriMesh::Point p = mesh.point(*vit);
		for (int i = 0; i < 3; i++) {
			if (boxMax[i] < p[i])
				boxMax[i] = p[i];
			if (boxMin[i] > p[i])
				boxMin[i] = p[i];
		}
	}
	TriMesh::Point boxCenter = (boxMax + boxMin) / 2.0f;
	TriMesh::Point boxBotCen(boxCenter[0], boxCenter[1], boxMin[2]);

	TriMesh::Point dest(center.x(), center.y(), 0.0f);
	TriMesh::Point disp = dest - boxBotCen;
	model.setToIdentity();
	model.translate(disp[0], disp[1], disp[2]);

	QMatrix4x4 mvp =  proj * view * model;
	program->Bind();
	program->SetUniformMat4f("u_MVP", mvp.data());

	delete vao;
	delete vbo;
	delete ibo;

	vao = new VAO;
	vbo = new VBO(mesh.GetVertices().data(), mesh.GetVertices().size() * sizeof(float));
	VBOLayout layout;
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	ibo = new IBO(mesh.GetIndices().data(), mesh.GetIndices().size());
}

void LDNI::Screen::RunLDNI()
{
	makeCurrent();

	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	if (!ibo)
		return;

	GLint viewport[4];
	f->glGetIntegerv(GL_VIEWPORT, viewport);
	f->glViewport(0, 0, width, height);

	f->glEnable(GL_DEPTH_TEST);
	f->glEnable(GL_STENCIL_TEST);

	fbo->Bind();

	f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	f->glClearDepth(1.0);
	f->glClearStencil(0);
	f->glClear(
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	f->glDepthFunc(GL_ALWAYS);
	f->glStencilFunc(GL_GREATER, 1, 0xff);
	f->glStencilOp(GL_INCR, GL_INCR, GL_INCR);

	program->Bind();

	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
	int renderN = 1;

	cv::Mat stencil = cv::Mat(height, width, CV_8UC1);
	f->glPixelStorei(GL_PACK_ALIGNMENT, 1);
	f->glPixelStorei(GL_PACK_ROW_LENGTH, stencil.step / stencil.elemSize());
	f->glReadPixels(0, 0,
		width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencil.data);
	cv::flip(stencil, stencil, 0);

	cv::Mat depth = cv::Mat(height, width, CV_32FC1);
	f->glPixelStorei(GL_PACK_ALIGNMENT, 4);
	f->glPixelStorei(GL_PACK_ROW_LENGTH, depth.step / depth.elemSize());
	f->glReadPixels(0, 0,
		width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth.data);
	cv::flip(depth, depth, 0);

	int maxDepthComplex = 0;
	for (int m = 0; m < stencil.rows; m++) {
		for (int n = 0; n < stencil.cols; n++) {
			if (stencil.at<uchar>(m, n) > maxDepthComplex)
				maxDepthComplex = stencil.at<uchar>(m, n);
		}
	}

	int totalFragments = 0;
	cv::Mat layer = cv::Mat::zeros(height, width, CV_32FC1);
	for (int i = 0; i < stencil.rows; i++) {
		for (int j = 0; j < stencil.cols; j++) {
			if (stencil.at<uchar>(i, j) >= renderN) {
				layer.at<float>(i, j) = depth.at<float>(i, j);
				totalFragments++;
			}
		}
	}

	f->glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
	for (int i = 2; i <= maxDepthComplex; i++) {
		f->glStencilFunc(GL_GREATER, i, 0xff);

		f->glClear(GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT);

		f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
		renderN++;

		f->glPixelStorei(GL_PACK_ALIGNMENT, 1);
		f->glPixelStorei(GL_PACK_ROW_LENGTH, stencil.step / stencil.elemSize());
		f->glReadPixels(0, 0,
			width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencil.data);
		cv::flip(stencil, stencil, 0);

		f->glPixelStorei(GL_PACK_ALIGNMENT, 4);
		f->glPixelStorei(GL_PACK_ROW_LENGTH, depth.step / depth.elemSize());
		f->glReadPixels(0, 0,
			width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth.data);
		cv::flip(depth, depth, 0);


		cv::Mat layer = cv::Mat::zeros(height, width, CV_32FC1);
		for (int i = 0; i < stencil.rows; i++) {
			for (int j = 0; j < stencil.cols; j++) {
				if (stencil.at<uchar>(i, j) >= renderN) {
					layer.at<float>(i, j) = depth.at<float>(i, j);
					totalFragments++;
				}
			}
		}
	}

	fbo->Unbind();

	f->glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	f->glStencilFunc(GL_ALWAYS, 0, 0xff);
	f->glDepthFunc(GL_LESS);
	f->glDisable(GL_STENCIL_TEST);
	f->glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	std::cout << "Maximum depth complexity : " << maxDepthComplex << std::endl;
	std::cout << "Total fragments : " << totalFragments << std::endl;
}

void LDNI::Screen::initializeGL()
{
	program = new ShaderProgram("res/shaders/Basic.vertex", "res/shaders/Basic.fragment");

	fbo = new FBO(width, height);
}

void LDNI::Screen::paintGL()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	if (!ibo)
		return;

	f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT);

	program->Bind();

	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}