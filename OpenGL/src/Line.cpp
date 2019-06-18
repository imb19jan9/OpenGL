#include "Line.h"

Line::Line()
	: vao(0),
	vbo(0),
	prog(0)
{
}

Line::~Line()
{
	delete vao;
	delete vbo;

	delete prog;
}

void Line::Init()
{
	vao = new VAO;
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));

	VBOLayout layout;
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	prog = new ShaderProgram("res/shaders/BasicColor.vertex",
		"res/shaders/BasicColor.fragment");
}

void Line::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glLineWidth(5);

	QMatrix4x4 mvp = proj_ * view_;

	prog->Bind();
	prog->SetUniformMat4f("u_MVP", mvp.data());
	prog->SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);

	vao->Bind();
	f->glDrawArrays(GL_LINES, 0, vertices.size() / 3);

	f->glLineWidth(1);
}

void Line::Clear()
{
	vertices.clear();

	Update();
}

void Line::AddSegment(QVector3D start_, QVector3D end_)
{
	for (int i = 0; i < 3; i++)
		vertices.push_back(start_[i]);
	for (int i = 0; i < 3; i++)
		vertices.push_back(end_[i]);

	Update();
}

void Line::Update()
{
	//FIXME : need memory managing like std::vector
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	vbo->Bind();
	f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

LineStrip::LineStrip()
	: vao(0),
	vbo(0),
	prog(0)
{
}

LineStrip::~LineStrip()
{
	delete vao;
	delete vbo;

	delete prog;
}

void LineStrip::Init()
{
	vao = new VAO;
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));

	VBOLayout layout;
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	prog = new ShaderProgram("res/shaders/BasicColor.vertex",
		"res/shaders/BasicColor.fragment");
}

void LineStrip::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glLineWidth(5);

	QMatrix4x4 mvp = proj_ * view_;

	prog->Bind();
	prog->SetUniformMat4f("u_MVP", mvp.data());
	prog->SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);

	vao->Bind();
	f->glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);

	f->glLineWidth(1);
}

void LineStrip::Clear()
{
	vertices.clear();

	Update();
}

void LineStrip::Add(QVector3D point_)
{
	for (int i = 0; i < 3; i++)
		vertices.push_back(point_[i]);

	Update();
}

void LineStrip::Update()
{
	//FIXME : need memory managing like std::vector
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	vbo->Bind();
	f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}
