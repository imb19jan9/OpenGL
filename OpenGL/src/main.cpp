#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

#include <string>
#include <iostream>
#include <fstream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	if (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ") : " << function << ", "
			<< file << " : " << line << std::endl;
		return false;
	}

	return true;
}

std::string LoadShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	std::string whole, line;
	while (getline(stream, line)) {
		whole = whole + line + '\n';
	}

	return whole;
}

unsigned int CompileShader(unsigned int type, const std::string& source) {
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

	GLCall(unsigned int id = f->glCreateShader(type));
	const char* src = source.c_str();
	GLCall(f->glShaderSource(id, 1, &src, 0));
	GLCall(f->glCompileShader(id));

	int result;
	GLCall(f->glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(f->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char *message = new char[length * sizeof(char)];
		GLCall(f->glGetShaderInfoLog(id, length, &length, message));

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

	GLCall(unsigned int program = f->glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(f->glAttachShader(program, vs));
	GLCall(f->glAttachShader(program, fs));
	GLCall(f->glLinkProgram(program));
	GLCall(f->glValidateProgram(program));

	GLCall(f->glDeleteShader(vs));
	GLCall(f->glDeleteShader(fs));

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
			GLCall(f->glDeleteBuffers(1, &vbo));
			GLCall(f->glDeleteBuffers(1, &ibo));
			GLCall(f->glDeleteProgram(program));
		}
	}

private:
	void initializeGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		GLCall(std::cout << f->glGetString(GL_VERSION) << std::endl);

		std::string vertexShader = LoadShader("res/shaders/Basic.vertex");
		std::string fragmentShader = LoadShader("res/shaders/Basic.fragment");

		program = CreateShaderProgram(vertexShader, fragmentShader);

		GLCall(f->glGenBuffers(1, &vbo));
		GLCall(f->glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GLCall(f->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));

		GLCall(f->glEnableVertexAttribArray(0));
		GLCall(f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

		GLCall(f->glGenBuffers(1, &ibo));
		GLCall(f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW));
	}

	void paintGL() {
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

		GLCall(f->glClear(GL_COLOR_BUFFER_BIT));

		GLCall(f->glUseProgram(program));
		GLCall(f->glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GLCall(f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	}
};

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	OpenGL openGL;
	openGL.show();

	return a.exec();
}
