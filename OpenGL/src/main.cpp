#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <QMatrix4x4>
#include <QSlider>
#include <QBoxLayout>
#include <QTimer>
#include <QObject>
#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "VBOLayout.h"
#include "Renderer.h"
#include "Texture.h"

class Sliders : public QWidget {
private:
	QBoxLayout layout;
	QSlider vSlider, hSlider;
	
public:
	Sliders(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags())
		: QWidget(parent, f),
		layout(QBoxLayout::LeftToRight, this),
		vSlider(Qt::Vertical, this), hSlider(Qt::Horizontal, this) {
		resize(200, 200);
		layout.addWidget(&vSlider);
		layout.addWidget(&hSlider);
		setLayout(&layout);

		vSlider.setMinimum(0);
		vSlider.setMaximum(100);
		hSlider.setMinimum(0);
		hSlider.setMaximum(100);
	}

	inline int GetVSliderValue() { return vSlider.value(); }
	inline int GetHSliderValue() { return hSlider.value(); }
};

class OpenGL : public QOpenGLWidget {
private:
	float positions[16] = {
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
	};

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	VAO* vao;
	VBO* vbo;
	IBO* ibo;
	ShaderProgram *program;
	Renderer renderer;
	Texture *texture;

	Sliders *sliders;

	QTimer *timer;
	
public:
	OpenGL() : vao(nullptr), vbo(nullptr), ibo(nullptr), program(nullptr) {
		sliders = new Sliders(this, Qt::Window);
		sliders->setVisible(true);

	}
	~OpenGL() {
		makeCurrent();

		delete vao;
		delete vbo;
		delete ibo;
		delete program;
		delete texture;

		delete sliders;
	}

private:
	void initializeGL() {
		QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

		std::cout << f->glGetString(GL_VERSION) << std::endl;

		program = new ShaderProgram("res/shaders/Basic.vertex", "res/shaders/Basic.fragment");
		program->Bind();
		program->SetUniform1i("u_Texture", 0);

		texture = new Texture("res/textures/hccl_logo.png");

		vao = new VAO;
		vao->Bind();
		
		vbo = new VBO(positions, 16 * sizeof(float));
		vbo->Bind();

		VBOLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vao->AddBuffer(*vbo, layout);

		ibo = new IBO(indices, 6);
	}

	void paintGL() {
		QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

		renderer.Clear();

		QMatrix4x4 model, view, proj, mvp;
		float posx = (sliders->GetHSliderValue() / 100.0 - 0.5) * 4;
		float posy = (sliders->GetVSliderValue() / 100.0 - 0.5) * 3;
		model.translate(posx, posy, 0);
		view.translate(0, 0, 0);
		proj.ortho(-2.0, 2.0, -1.5, 1.5, -1.0, 1.0);
		mvp = proj * view * model;
		program->SetUniformMat4f("u_MVP", mvp.data());

		texture->Bind();
		renderer.Draw(*vao, *ibo, *program);

		update();
	}
};

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	OpenGL openGL;
	openGL.resize(800, 600);
	openGL.show();

	return a.exec();
}
