#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QMouseEvent>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "Sphere.h"
#include "CheckerBoard.h"

namespace Ui { class TrackBall; };

class TrackBall : public QWidget
{
	Q_OBJECT
private:
	QWidget *menu;

public:
	class Screen : public QOpenGLWidget
	{
	private:
		QMatrix4x4 view, proj;
		QQuaternion prevQ, currQ;

		Sphere sphere;
		CheckerBoard board;

		QPoint clicked, cursor;

		QVector3D pivot;
		float camDistance;

	public:
		Screen(QWidget *parent = 0);
		~Screen();

	private:
		void initializeGL();
		void paintGL();
		void mousePressEvent(QMouseEvent* e_);
		void mouseMoveEvent(QMouseEvent* e_);

		QVector3D OnTrackBall(QPoint p_);
	};

public:
	TrackBall(QWidget *menu_);
	~TrackBall();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private:
	Ui::TrackBall *ui;
};
