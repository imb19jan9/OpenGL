#include "Screen.h"

#include <QMouseEvent>

namespace TRACKBALL
{
	Screen::Screen(QWidget *parent)
		: QOpenGLWidget(parent),
		axis(25),
		tetra(15),
		sphere(50, 25),
		radius(50.f),
		moving(3, 50, 25),
		fixed(3, 50, 25),
		dragging(false),
		camDistance(150)
	{
		tetra.SetColor(QVector4D(1.0, 0.0, 1.0, 1.0));

		sphere.SetRadius(radius);
		sphere.SetColor(QVector4D(1.0, 0.0, 0.0, 0.1));

		moving.SetColor(QVector4D(0.0, 1.0, 0.0, 1.0));
		fixed.SetColor(QVector4D(1.0, 1.0, 0.0, 1.0));
		toMoving.SetColor(QVector4D(0.0, 0.0, 0.0, 1.0));
		toFixed.SetColor(QVector4D(0.0, 0.0, 0.0, 1.0));

		path.SetColor(QVector4D(0.0, 1.0, 1.0, 1.0));

		setMouseTracking(true);
	}

	Screen::~Screen()
	{

	}

	void Screen::initializeGL()
	{
		QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
			versionFunctions<QOpenGLFunctions_4_5_Core>();
		f->glEnable(GL_DEPTH_TEST);
		f->glEnable(GL_BLEND);
		f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		f->glEnable(GL_CULL_FACE);
		f->glCullFace(GL_BACK);

		axis.Init();

		tetra.Init();
		sphere.Init();

		moving.Init();
		fixed.Init();
		toMoving.Init();
		toFixed.Init();

		path.Init();

		QMatrix4x4 t1;
		t1.setColumn(3, QVector4D(0, 0, camDistance, 1));
		view = t1.inverted();
		proj.perspective(60, width() / (float)height(), 1, 1000);
	}

	void Screen::paintGL()
	{
		QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
			versionFunctions<QOpenGLFunctions_4_5_Core>();

		f->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		axis.Draw(view, proj);

		QMatrix4x4 identity;
		moving.Draw(identity, proj);
		toMoving.Draw(identity, proj);
		if (dragging) {
			fixed.Draw(identity, proj);
			toFixed.Draw(identity, proj);
			path.Draw(identity, proj);
		}

		tetra.Draw(view, proj);
		sphere.Draw(view, proj);
	}

	void Screen::mousePressEvent(QMouseEvent * e_)
	{
		if (e_->button() != Qt::LeftButton)
			return;

		makeCurrent();

		clicked = e_->pos();
		clicked.setY(height() - 1 - clicked.y());

		QVector3D op = OnTrackBall(clicked);
		QVector3D pos = op * radius + QVector3D(0, 0, -camDistance);
		fixed.SetPosition(pos);

		toFixed.Clear();
		toFixed.AddSegment(QVector3D(0, 0, -camDistance), pos);

		dragging = true;
		prevQ = currQ;
	}

	void Screen::mouseMoveEvent(QMouseEvent * e_)
	{
		update();
		makeCurrent();

		cursor = e_->pos();
		cursor.setY(height() - 1 - cursor.y());

		QVector3D op1 = OnTrackBall(clicked);
		QVector3D op2 = OnTrackBall(cursor);
		QVector3D pos1 = op1 * radius + QVector3D(0, 0, -camDistance);
		QVector3D pos2 = op2 * radius + QVector3D(0, 0, -camDistance);
		moving.SetPosition(pos2);

		toMoving.Clear();
		toMoving.AddSegment(QVector3D(0, 0, -camDistance), pos2);

		if (!dragging)
			return;

		QQuaternion delta = QQuaternion::rotationTo(op1, op2);

		currQ = delta * prevQ;

		QMatrix4x4 r, t1, t2;
		QMatrix3x3 rot = currQ.toRotationMatrix();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				r(i, j) = rot(i, j);
			}
		}
		t1.translate(0, 0, camDistance);
		view = t1.inverted() * r;

		CreatePath(op1, op2);
	}

	void Screen::mouseReleaseEvent(QMouseEvent * e_)
	{
		update();

		dragging = false;
	}

	QVector3D Screen::OnTrackBall(QPoint p_)
	{
		QVector3D v(p_.x() / (float)(width() - 1) * 2.0 - 1,
			p_.y() / (float)(height() - 1) * 2.0 - 1,
			0);

		float dd = v[0] * v[0] + v[1] * v[1];
		v[2] = dd < 0.5 ? sqrt(1 - dd) : 0.5 / sqrt(dd);
		v.normalize();

		return v;
	}

	void Screen::CreatePath(QVector3D start_, QVector3D end_)
	{
		path.Clear();

		QQuaternion delta = QQuaternion::rotationTo(start_, end_);

		const int PATH_COUNT = 30;
		for (int i = 0; i < PATH_COUNT; i++) {
			float a = i / (float)PATH_COUNT;
			QQuaternion qq = QQuaternion::slerp(QQuaternion(), delta, a);
			QVector3D vec = qq.rotatedVector(start_);
			vec.normalize();
			vec = vec * radius + QVector3D(0, 0, -camDistance);
			path.Add(vec);
		}
	}
}

