#pragma once

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"

namespace TRACKBALL
{
	class Tetrahedron
	{
	private:
		VAO* vao;
		VBO* vbo;
		IBO* ibo;

		ShaderProgram *prog;

		TriMesh mesh;

		float scaleSize;
		QVector3D pos;
		QVector4D color;

	public:
		Tetrahedron(float scaleSize_);
		~Tetrahedron();

		void Init();
		void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

		inline void SetPosition(QVector3D pos_) { pos = pos_; }
		inline void SetColor(QVector4D color_) { color = color_; }

	private:
		void Create();
	};
}
