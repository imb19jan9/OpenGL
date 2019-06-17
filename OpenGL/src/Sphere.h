#pragma once

#include <QMatrix4x4>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "TriMesh.h"

class Sphere
{
private:
	VAO *vao;
	VBO *vbo;
	IBO *ibo;

	ShaderProgram *prog;

	float radius;
	int sectorCount, stackCount;

	TriMesh mesh;

	QVector3D pos;
	QVector4D color;

public:
	Sphere(int sectorCount_, int stackCount_);
	Sphere(float radius_, int sectorCount_, int stackCount_);
	~Sphere();

	void Init();
	void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

	inline void SetPosition(QVector3D pos_) { pos = pos_; }
	inline void SetRadius(float radius_) { radius = radius_; }
	inline void SetColor(QVector4D color_) { color = color_; }

private:
	void BuildMesh();
};