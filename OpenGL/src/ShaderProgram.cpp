#include "ShaderProgram.h"

#include <QOpenGLFunctions_3_3_Core>
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string & vsFilePath_, const std::string & fsFilePath_)
{
	std::string vs = LoadShader(vsFilePath_);
	std::string fs = LoadShader(fsFilePath_);

	id = CreateShaderProgram(vs, fs);
}

ShaderProgram::~ShaderProgram()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glDeleteProgram(id);
}

void ShaderProgram::Bind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glUseProgram(id);
}

void ShaderProgram::Unbind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glUseProgram(0);
}

void ShaderProgram::SetUniform1i(const std::string & name_, int value_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glUniform1i(GetUniformLocation(name_), value_);
}

void ShaderProgram::SetUniform4f(const std::string & name_, float v0_, float v1_, float v2_, float v3_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glUniform4f(GetUniformLocation(name_), v0_, v1_, v2_, v3_);
}

void ShaderProgram::SetUniformMat4f(const std::string & name_, float * mat_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glUniformMatrix4fv(GetUniformLocation(name_), 1, GL_FALSE, mat_);
}

std::string ShaderProgram::LoadShader(const std::string & filepath_)
{
	std::ifstream stream(filepath_);

	std::string whole, line;
	while (getline(stream, line)) {
		whole = whole + line + '\n';
	}

	return whole;
}

unsigned int ShaderProgram::CompileShader(unsigned int type_, const std::string & source_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	unsigned int id = f->glCreateShader(type_);
	const char* src = source_.c_str();
	f->glShaderSource(id, 1, &src, 0);
	f->glCompileShader(id);

	int result;
	f->glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		f->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char *message = new char[length];
		f->glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile shader!" << std::endl;
		std::cout << message << std::endl;

		delete message;

		return 0;
	}

	return id;
}

unsigned int ShaderProgram::CreateShaderProgram(std::string vs_, std::string fs_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	unsigned int program = f->glCreateProgram();
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vs_);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs_);
	f->glAttachShader(program, vertexShader);
	f->glAttachShader(program, fragmentShader);
	f->glLinkProgram(program);
	f->glValidateProgram(program);

	f->glDeleteShader(vertexShader);
	f->glDeleteShader(fragmentShader);

	return program;
}

int ShaderProgram::GetUniformLocation(const std::string & name_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	if (uniformLocationCache.find(name_) != uniformLocationCache.end())
		return uniformLocationCache[name_];

	int location = f->glGetUniformLocation(id, name_.c_str());
	uniformLocationCache[name_] = location;
	return location;
}
