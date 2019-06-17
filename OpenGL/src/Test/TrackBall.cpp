#include "TrackBall.h"
#include "ui_TrackBall.h"

TrackBall::TrackBall(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::TrackBall();
	ui->setupUi(this);
}

TrackBall::~TrackBall()
{
	delete ui;
}

TrackBall::Screen::Screen(QWidget *parent)
	: QOpenGLWidget(parent),
	sphere(20, 50, 25),
	board(10, 10),
	pivot(50, 50, 0),
	camDistance(150)
{
	sphere.SetColor(QVector4D(1.0, 0.0, 0.0, 1.0));
	sphere.SetPosition(pivot);
}

TrackBall::Screen::~Screen()
{

}

void TrackBall::Screen::initializeGL()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glEnable(GL_DEPTH_TEST);

	sphere.Init();
	board.Init();

	QMatrix4x4 r, t1, t2;
	r.rotate(currQ);
	t1.translate(0, 0, camDistance);
	t2.translate(pivot);
	view = (t2 * r * t1).inverted();
	proj.perspective(60, width() / (float)height(), 1, 1000);
}

void TrackBall::Screen::paintGL()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sphere.Draw(view, proj);
	board.Draw(view, proj);
}

void TrackBall::Screen::mousePressEvent(QMouseEvent * e_)
{
	if (e_->button() != Qt::LeftButton)
		return;

	clicked = e_->pos();
	clicked.setY(height() - 1 - clicked.y());

	prevQ = currQ;
}

void TrackBall::Screen::mouseMoveEvent(QMouseEvent * e_)
{
	cursor = e_->pos();
	cursor.setY(height() - 1 - cursor.y());

	QVector3D op1 = OnTrackBall(clicked);
	QVector3D op2 = OnTrackBall(cursor);
	QQuaternion delta = QQuaternion::rotationTo(op2, op1);

	currQ = delta * prevQ;

	QMatrix4x4 r, t1, t2;
	r.rotate(currQ);
	t1.translate(0, 0, camDistance);
	t2.translate(pivot);
	view = (t2 * r * t1).inverted();

	update();
}
QVector3D TrackBall::Screen::OnTrackBall(QPoint p_)
{
	QVector3D v(p_.x() / (float)(width() - 1) * 2.0 - 1,
		p_.y() / (float)(height() - 1) * 2.0 - 1,
		0);

	float dd = v[0] * v[0] + v[1] * v[1];
	v[2] = dd < 0.5 ? sqrt(1 - dd) : 0.5 / sqrt(dd);
	v.normalize();

	return v;
}