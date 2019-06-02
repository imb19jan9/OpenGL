#pragma once

#include <QWidget>
#include <QOpenGLWidget>

namespace Ui { class ClearColor; };

class Screen : public QOpenGLWidget
{
private:
	float r, g, b, a;

public:
	Screen(QWidget* parent = 0) : QOpenGLWidget(parent), 
		r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

	void SetClearColor(float r_, float g_, float b_, float a_);

private:
	void paintGL();
};

class ClearColor : public QWidget
{
	Q_OBJECT
private:
	QWidget *menu;

public:
	ClearColor(QWidget *menu_);
	~ClearColor();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private slots:
	void colorChanged();

private:
	Ui::ClearColor *ui;
};
