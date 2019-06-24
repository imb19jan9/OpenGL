#include "Axis.h"

Axis::Axis()
	: vaoLine(0),
	vaoPoint(0),
	vboLine(0),
	vboPoint(0),
	prog(0)
{
	Create();
}

Axis::Axis(float size_)
	: vaoLine(0),
	vaoPoint(0),
	vboLine(0),
	vboPoint(0),
	prog(0),
	size(size_)
{
	Create();
}

Axis::~Axis()
{
	delete vaoLine;
	delete vaoPoint;
	delete vboLine;
	delete vboPoint;

	delete prog;
}

void Axis::Init()
{
	vaoLine = new VAO;
	vboLine = new VBO(lines.data(), lines.size() * sizeof(float));
	VBOLayout layoutLine;
	layoutLine.Push<float>(3);
	layoutLine.Push<float>(4);
	vaoLine->AddBuffer(*vboLine, layoutLine);

	vaoPoint = new VAO;
	vboPoint = new VBO(points.data(), points.size() * sizeof(float));
	VBOLayout layoutPoint;
	layoutPoint.Push<float>(3);
	layoutPoint.Push<float>(4);
	vaoPoint->AddBuffer(*vboPoint, layoutPoint);

	prog = new ShaderProgram("res/shaders/BasicVertexColor.vertex",
		"res/shaders/BasicVertexColor.fragment");
}

void Axis::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glLineWidth(10);
	f->glPointSize(10);

	QMatrix4x4 model;
	model.scale(size);
	QMatrix4x4 mvp = proj_ * view_ * model;

	prog->Bind();
	prog->SetUniformMat4f("u_MVP", mvp.data());

	vaoLine->Bind();
	f->glDrawArrays(GL_LINES, 0, 6);

	vaoPoint->Bind();
	f->glDrawArrays(GL_POINTS, 0, 3);

	f->glLineWidth(1);
	f->glPointSize(1);
}

void Axis::Create()
{
	QVector3D origin(0, 0, 0);
	QVector3D ivec(1, 0, 0);
	QVector3D jvec(0, 1, 0);
	QVector3D kvec(0, 0, 1);
	QVector4D red(1, 0, 0, 1);
	QVector4D green(0, 1, 0, 1);
	QVector4D blue(0, 0, 1, 1);

	for (int i = 0; i < 3; i++) {
		lines.push_back(origin[i]);
	}
	for (int i = 0; i < 4; i++) {
		lines.push_back(red[i]);
	}
	for (int i = 0; i < 3; i++) {
		lines.push_back(ivec[i]);
	}
	for (int i = 0; i < 4; i++) {
		lines.push_back(red[i]);
	}

	for (int i = 0; i < 3; i++) {
		lines.push_back(origin[i]);
	}
	for (int i = 0; i < 4; i++) {
		lines.push_back(green[i]);
	}
	for (int i = 0; i < 3; i++) {
		lines.push_back(jvec[i]);
	}
	for (int i = 0; i < 4; i++) {
		lines.push_back(green[i]);
	}

	for (int i = 0; i < 3; i++) {
		lines.push_back(origin[i]);
	}
	for (int i = 0; i < 4; i++) {
		lines.push_back(blue[i]);
	}
	for (int i = 0; i < 3; i++) {
		lines.push_back(kvec[i]);
	}
	for (int i = 0; i < 4; i++) {
		lines.push_back(blue[i]);
	}

	for (int i = 0; i < 3; i++) {
		points.push_back(ivec[i]);
	}
	for (int i = 0; i < 4; i++) {
		points.push_back(red[i]);
	}

	for (int i = 0; i < 3; i++) {
		points.push_back(jvec[i]);
	}
	for (int i = 0; i < 4; i++) {
		points.push_back(green[i]);
	}

	for (int i = 0; i < 3; i++) {
		points.push_back(kvec[i]);
	}
	for (int i = 0; i < 4; i++) {
		points.push_back(blue[i]);
	}
}
