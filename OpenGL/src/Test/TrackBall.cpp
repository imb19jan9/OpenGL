#include "TrackBall.h"
#include "ui_TrackBall.h"

Tetrahedron::Tetrahedron(float scaleSize_)
	: vao(0),
	vbo(0),
	ibo(0),
	prog(0),
	scaleSize(scaleSize_)
{
	Create();
}

Tetrahedron::~Tetrahedron()
{
	delete vao;
	delete vbo;
	delete ibo;

	delete prog;
}

void Tetrahedron::Init()
{
	vao = new VAO;
	vbo = new VBO(mesh.GetVertices().data(), mesh.GetVertices().size() * sizeof(float));

	VBOLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	ibo = new IBO(mesh.GetIndices().data(), mesh.GetIndices().size());

	prog = new ShaderProgram("res/shaders/Phong.vertex",
		"res/shaders/Phong.fragment");
}

void Tetrahedron::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	QMatrix4x4 model, model1, model2;
	model1.scale(scaleSize);
	model2.translate(pos);
	model = model2 * model1;

	QMatrix4x4 mv = view_ * model;
	prog->Bind();
	prog->SetUniformMat4f("u_ModelView", mv.data());
	prog->SetUniformMat4f("u_Proj", proj_.data());

	prog->SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);
	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void Tetrahedron::Create()
{
	mesh.Clear();

	TriMesh::Point v1(sqrt(8 / 9.0), 0, -1 / 3.0);
	TriMesh::Point v2(-sqrt(2 / 9.0), sqrt(2 / 3.0), -1 / 3.0);
	TriMesh::Point v3(-sqrt(2 / 9.0), -sqrt(2 / 3.0), -1 / 3.0);
	TriMesh::Point v4(0, 0, 1);

	TriMesh::VertexHandle vh1 = mesh.add_vertex(v1);
	TriMesh::VertexHandle vh2 = mesh.add_vertex(v2);
	TriMesh::VertexHandle vh3 = mesh.add_vertex(v3);
	TriMesh::VertexHandle vh4 = mesh.add_vertex(v4);

	mesh.add_face(vh1, vh3, vh2);
	mesh.add_face(vh1, vh2, vh4);
	mesh.add_face(vh2, vh3, vh4);
	mesh.add_face(vh1, vh4, vh3);

	mesh.Update();
}

TrackBall::TrackBall(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::TrackBall();
	ui->setupUi(this);
}

TrackBall::~TrackBall()
{
	delete ui;
}

TrackBall::Screen::Screen(QWidget *parent)
	: QOpenGLWidget(parent),
	tetra(15),
	sphere(50, 25),
	moving(3, 50, 25),
	fixed(3, 50, 25),
	dragging(false),
	radius(30.f),
	pivot(50, 50, 0),
	camDistance(150)
{
	tetra.SetColor(QVector4D(1.0, 0.0, 1.0, 1.0));
	tetra.SetPosition(pivot);

	sphere.SetRadius(radius);
	sphere.SetColor(QVector4D(1.0, 0.0, 0.0, 0.1));
	sphere.SetPosition(pivot);

	moving.SetColor(QVector4D(0.0, 1.0, 0.0, 1.0));
	fixed.SetColor(QVector4D(1.0, 1.0, 0.0, 1.0));
	toMoving.SetColor(QVector4D(0.0, 0.0, 0.0, 1.0));
	toFixed.SetColor(QVector4D(0.0, 0.0, 0.0, 1.0));

	setMouseTracking(true);
}

TrackBall::Screen::~Screen()
{

}

void TrackBall::Screen::initializeGL()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glEnable(GL_DEPTH_TEST);
	f->glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	f->glEnable(GL_CULL_FACE);
	f->glCullFace(GL_BACK);

	tetra.Init();
	sphere.Init();
	moving.Init();
	fixed.Init();
	toMoving.Init();
	toFixed.Init();
	f->glLineWidth(5);

	QMatrix4x4 r, t1, t2;
	r.rotate(currQ);
	t1.translate(0, 0, camDistance);
	t2.translate(pivot);
	view = (t2 * r * t1).inverted();
	proj.perspective(60, width() / (float)height(), 1, 1000);
}

void TrackBall::Screen::paintGL()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 t1, t2;
	t1.translate(0, 0, camDistance);
	t2.translate(pivot);
	QMatrix4x4 t2t1 = (t2 * t1).inverted();
	moving.Draw(t2t1, proj);
	toMoving.Draw(t2t1, proj);
	if (dragging) {
		fixed.Draw(t2t1, proj);
		toFixed.Draw(t2t1, proj);
	}

	tetra.Draw(view, proj);
	sphere.Draw(view, proj);
}

void TrackBall::Screen::mousePressEvent(QMouseEvent * e_)
{
	if (e_->button() != Qt::LeftButton)
		return;

	makeCurrent();

	clicked = e_->pos();
	clicked.setY(height() - 1 - clicked.y());

	QVector3D op1 = OnTrackBall(clicked);
	QVector3D pos = op1 * radius + pivot;
	fixed.SetPosition(pos);

	toFixed.Clear();
	toFixed.AddSegment(pivot, pos);

	prevQ = currQ;

	dragging = true;
}

void TrackBall::Screen::mouseMoveEvent(QMouseEvent * e_)
{
	update();
	makeCurrent();

	cursor = e_->pos();
	cursor.setY(height() - 1 - cursor.y());

	QVector3D op1 = OnTrackBall(clicked);
	QVector3D op2 = OnTrackBall(cursor);
	QVector3D pos = op2 * radius + pivot;
	moving.SetPosition(pos);

	toMoving.Clear();
	toMoving.AddSegment(pivot, pos);

	if (!dragging)
		return;

	QQuaternion delta = QQuaternion::rotationTo(op1, op2);

	currQ = delta * prevQ;

	QMatrix4x4 r, t1, t2;
	r.rotate(currQ);
	t1.translate(0, 0, camDistance);
	t2.translate(pivot);
	view = (t2 * r * t1).inverted();
}
void TrackBall::Screen::mouseReleaseEvent(QMouseEvent * e_)
{
	makeCurrent();

	dragging = false;
}

QVector3D TrackBall::Screen::OnTrackBall(QPoint p_)
{
	QVector3D v(p_.x() / (float)(width() - 1) * 2.0 - 1,
		p_.y() / (float)(height() - 1) * 2.0 - 1,
		0);

	float dd = v[0] * v[0] + v[1] * v[1];
	v[2] = dd < 0.5 ? sqrt(1 - dd) : 0.5 / sqrt(dd);
	v.normalize();

	return v;
}