#pragma once

#include <QOpenGLWidget>
#include <QMatrix4x4>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace TEXTURE2D
{
	class Screen : public QOpenGLWidget
	{
	private:
		VAO* vao;
		VBO* vbo;
		IBO* ibo;
		ShaderProgram* program;
		Texture* texture;

		QMatrix4x4 mvp;

		float positions[16] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
		};

		unsigned int indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

	public:
		Screen(QWidget *parent = 0);
		~Screen();

	private:
		void initializeGL();
		void paintGL();
	};
}