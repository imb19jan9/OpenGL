#pragma once

#include <qmatrix4x4.h>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"

class Line
{
private:
	VAO* vao;
	VBO* vbo;

	ShaderProgram* prog;

	std::vector<float> vertices;

	QVector4D color;

public:
	Line();
	~Line();

	void Init();
	void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

	void Clear();
	void AddSegment(QVector3D start_, QVector3D end_);

	inline void SetColor(QVector4D color_) { color = color_; }

private:
	void Update();
};

class LineStrip
{
private:
	VAO* vao;
	VBO* vbo;

	ShaderProgram* prog;

	std::vector<float> vertices;

	QVector4D color;

public:
	LineStrip();
	~LineStrip();

	void Init();
	void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

	void Clear();
	void Add(QVector3D point_);

	inline void SetColor(QVector4D color_) { color = color_; }

private:
	void Update();
};