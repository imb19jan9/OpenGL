#include "Texture2D.h"

#include <QOpenGLFunctions_4_5_Core>

#include "ui_Texture2D.h"
#include "VBOLayout.h"

Texture2D::Texture2D(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::Texture2D();
	ui->setupUi(this);
}

Texture2D::~Texture2D()
{
	delete ui;
}

Texture2D::Screen::Screen(QWidget *parent) : QOpenGLWidget(parent)
{

}

Texture2D::Screen::~Screen()
{
	makeCurrent();

	delete vao;
	delete vbo;
	delete ibo;
	delete program;
	delete texture;
}

void Texture2D::Screen::initializeGL()
{
	vao = new VAO;
	vbo = new VBO(positions, 16 * sizeof(float));
	VBOLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao->AddBuffer(*vbo, layout);

	ibo = new IBO(indices, 6);

	program = new ShaderProgram("res/shaders/Texture2D.vertex", "res/shaders/Texture2D.fragment");
	texture = new Texture("res/textures/hccl_logo.png");
	program->SetUniform1i("u_Texture", 0);

	mvp.ortho(-100.0f, 100.0f, -75.0f, 75.0f, -1.0f, 1.0f);
}

void Texture2D::Screen::paintGL()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT);

	texture->Bind();
	program->Bind();
	program->SetUniformMat4f("u_MVP", mvp.data());

	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}
