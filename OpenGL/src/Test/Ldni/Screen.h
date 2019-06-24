#pragma once

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <opencv2\opencv.hpp>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "FBO.h"

namespace LDNI
{
	class Screen : public QOpenGLWidget
	{
	private:
		VAO* vao;
		VBO* vbo;
		IBO* ibo;
		ShaderProgram* program;
		FBO* fbo;

		QMatrix4x4 model, view, proj;

		QVector3D eye, center, up;

		TriMesh mesh;

		int width, height;
		std::vector<cv::Mat> layers;

	public:
		Screen(QWidget* parent = 0);
		~Screen();

		void LoadMesh(const std::string& filePath_);

		void RunLDNI();

	private:
		void initializeGL();
		void paintGL();
	};
}