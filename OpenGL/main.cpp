#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

class OpenGL : public QOpenGLWidget {
private:
	float positions[6] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f,
	};

	unsigned int vbo;

private:
	void initializeGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		std::cout << f->glGetString(GL_VERSION) << std::endl;

		f->glGenBuffers(1, &vbo);
		f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
		f->glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
		
		f->glEnableVertexAttribArray(0);
		f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	}

	void paintGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		f->glClear(GL_COLOR_BUFFER_BIT);

		f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	OpenGL openGL;
	openGL.show();

	return a.exec();
}
