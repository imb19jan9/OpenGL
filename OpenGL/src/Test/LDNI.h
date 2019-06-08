#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <opencv2\opencv.hpp>

#include "TriMesh.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "FBO.h"

namespace Ui { class LDNI; };

class LDNI : public QWidget
{
	Q_OBJECT
private:
	QWidget *menu;

public:
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

public:
	LDNI(QWidget *menu_);
	~LDNI();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private slots:
	void LoadMesh();
	void Run();

private:
	Ui::LDNI *ui;
};
