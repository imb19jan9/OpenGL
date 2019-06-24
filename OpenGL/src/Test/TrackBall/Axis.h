#pragma once

#include <QMatrix4x4>

#include "VAO.h"
#include "VBO.h"
#include "ShaderProgram.h"

class Axis
{
private:
	VAO* vaoLine, *vaoPoint;
	VBO* vboLine, *vboPoint;

	ShaderProgram* prog;

	float size;

	std::vector<float> lines;
	std::vector<float> points;

public:
	Axis();
	Axis(float size_);
	~Axis();

	void Init();
	void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

private:
	void Create();
};