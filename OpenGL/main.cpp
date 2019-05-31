#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

unsigned int CompileShader(unsigned int type, const std::string& source)
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

	unsigned int id = f->glCreateShader(type);
	const char* src = source.c_str();
	f->glShaderSource(id, 1, &src, 0);
	f->glCompileShader(id);

	int result;
	f->glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		f->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char *message = new char[length * sizeof(char)];
		f->glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex " : "fragment ") <<
			"shader!" << std::endl;
		std::cout << message << std::endl;

		delete message;

		return 0;
	}

	return id;
}

unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

	unsigned int program = f->glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	f->glAttachShader(program, vs);
	f->glAttachShader(program, fs);
	f->glLinkProgram(program);
	f->glValidateProgram(program);

	f->glDeleteShader(vs);
	f->glDeleteShader(fs);

	return program;
}

class OpenGL : public QOpenGLWidget {
private:
	float positions[6] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f,
	};

	unsigned int vbo;
	unsigned int program;
	bool initialized;
	
public:
	OpenGL() : initialized(false) {	}
	~OpenGL() {
		if (initialized) {
			QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
			f->glDeleteBuffers(1, &vbo);
			f->glDeleteProgram(program);
		}
	}

private:
	void initializeGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		std::cout << f->glGetString(GL_VERSION) << std::endl;

		std::string vertexShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec4 position;\n"
			"\n"
			"void main() {\n"
			"  gl_Position = position;\n"
			"}\n";
		std::string fragmentShader =
			"#version 330 core\n"
			"\n"
			"out vec4 color;\n"
			"\n"
			"void main() {\n"
			"  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
			"}\n";

		program = CreateShaderProgram(vertexShader, fragmentShader);

		f->glGenBuffers(1, &vbo);
		f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
		f->glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
		
		f->glEnableVertexAttribArray(0);
		f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	}

	void paintGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		f->glClear(GL_COLOR_BUFFER_BIT);

		f->glUseProgram(program);
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
