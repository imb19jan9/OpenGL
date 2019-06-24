#pragma once

#include <QOpenGLWidget>
#include <QMatrix4x4>

#include "Line.h"
#include "Axis.h"
#include "Tetrahedron.h"
#include "Sphere.h"

namespace TRACKBALL
{
	class Screen : public QOpenGLWidget
	{
	private:
		QMatrix4x4 view, proj;
		QQuaternion prevQ, currQ;

		Axis axis;
		Tetrahedron tetra;
		Sphere sphere;
		float radius;

		QPoint clicked, cursor;
		Sphere moving, fixed;
		Line toMoving, toFixed;
		bool dragging;

		LineStrip path;

		float camDistance;

	public:
		Screen(QWidget *parent = 0);
		~Screen();

	private:
		void initializeGL();
		void paintGL();
		void mousePressEvent(QMouseEvent* e_);
		void mouseMoveEvent(QMouseEvent* e_);
		void mouseReleaseEvent(QMouseEvent* e_);

		QVector3D OnTrackBall(QPoint p_);

		void CreatePath(QVector3D start_, QVector3D end_);
	};
}