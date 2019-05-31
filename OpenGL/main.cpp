#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <iostream>

class OpenGL : public QOpenGLWidget {
	void initializeGL() {
		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void paintGL() {
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glEnd();
	}
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	OpenGL openGL;
	openGL.show();

	return a.exec();
}
