#pragma once

#include <string>
#include <unordered_map>

class ShaderProgram {
private:
	unsigned int id;
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	ShaderProgram(const std::string& vsFilePath_, const std::string& fsFilePath_);
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name_, int value_);
	void SetUniform4f(const std::string& name_, float v0_, float v1_, float v2_, float v3_);

private:
	std::string LoadShader(const std::string& filepath_);
	unsigned int CompileShader(unsigned int type_, const std::string& source_);
	unsigned int CreateShaderProgram(std::string vs_, std::string fs_);

	int GetUniformLocation(const std::string& name_);
};