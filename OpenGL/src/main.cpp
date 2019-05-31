#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <fstream>
#include <sstream>

std::string LoadShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	std::stringstream shader;
	std::string line;
	while (getline(stream, line)) {
		shader << line << '\n';
	}

	return shader.str();
}

unsigned int CompileShader(unsigned int type, const std::string& source) {
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

	unsigned int vbo;
	unsigned int ibo;
	unsigned int program;
	bool initialized;
	
public:
	OpenGL() : initialized(false) {	}
	~OpenGL() {
		if (initialized) {
			QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
			f->glDeleteBuffers(1, &vbo);
			f->glDeleteBuffers(1, &ibo);
			f->glDeleteProgram(program);
		}
	}

private:
	void initializeGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		std::cout << f->glGetString(GL_VERSION) << std::endl;

		std::string vertexShader = LoadShader("res/shaders/Basic.vertex");
		std::string fragmentShader = LoadShader("res/shaders/Basic.fragment");

		program = CreateShaderProgram(vertexShader, fragmentShader);

		f->glGenBuffers(1, &vbo);
		f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
		f->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

		f->glEnableVertexAttribArray(0);
		f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		f->glGenBuffers(1, &ibo);
		f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW);
	}

	void paintGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		f->glClear(GL_COLOR_BUFFER_BIT);

		f->glUseProgram(program);
		f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
		f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	OpenGL openGL;
	openGL.show();

	return a.exec();
}
