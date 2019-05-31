#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "VBOLayout.h"
#include "Renderer.h"

class OpenGL : public QOpenGLWidget {
private:
	float positions[8] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
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
	
public:
	OpenGL() : vao(nullptr), vbo(nullptr), ibo(nullptr), program(nullptr) {}
	~OpenGL() {
		makeCurrent();

		delete vao;
		delete vbo;
		delete ibo;
		delete program;
	}

private:
	void initializeGL() {
		QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

		std::cout << f->glGetString(GL_VERSION) << std::endl;

		program = new ShaderProgram("res/shaders/Basic.vertex", "res/shaders/Basic.fragment");
		
		vao = new VAO;
		vao->Bind();
		
		vbo = new VBO(positions, 8 * sizeof(float));
		vbo->Bind();

		VBOLayout layout;
		layout.Push<float>(2);
		vao->AddBuffer(*vbo, layout);

		ibo = new IBO(indices, 6);
	}

	void paintGL() {
		QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

		renderer.Clear();

		static float r = 0.0f;
		static float increment = 0.05f;

		r += increment;
		if (r > 1.0f) {
			r = 1.0f;
			increment = -0.05f;
		}
		else if (r < 0.0f) {
			r = 0.0f;
			increment = 0.05f;
		}
			
		std::cout << "r : " << r << std::endl;
		program->Bind();
		program->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer.Draw(*vao, *ibo, *program);
	}
};

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);

	OpenGL openGL;
	openGL.setFormat(format);
	openGL.show();

	return a.exec();
}
