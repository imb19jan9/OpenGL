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
#include "Line.h"
#include "Axis.h"

namespace Ui { class TrackBall; };

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

		Axis axis;
		Tetrahedron tetra;
		Sphere sphere;
		float radius;

		QPoint clicked, cursor;
		Sphere moving, fixed;
		Line toMoving, toFixed;
		bool dragging;

		LineStrip path;

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
		void mouseReleaseEvent(QMouseEvent* e_);

		QVector3D OnTrackBall(QPoint p_);

		void CreatePath(QVector3D start_, QVector3D end_);
		QVector3D Slerp(const QVector3D & from, const QVector3D & to, float alpha);
	};

public:
	TrackBall(QWidget *menu_);
	~TrackBall();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private:
	Ui::TrackBall *ui;
};
