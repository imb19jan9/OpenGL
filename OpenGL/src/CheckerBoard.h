#pragma once

#include <QMatrix4x4>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "TriMesh.h"

class CheckerBoard
{
private:
	VAO *vao;
	VBO *vbo;
	IBO *ibo;
	ShaderProgram *prog;

	TriMesh mesh;

	int nx, ny;

public:
	CheckerBoard(int nx_, int ny_);
	~CheckerBoard();

	void Init();
	void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

private:
	void Create();
};