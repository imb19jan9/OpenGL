#include "ClearColor.h"

#include "ui_ClearColor.h"

#include <QOpenGLFunctions_3_3_Core>

ClearColor::ClearColor(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::ClearColor;
	ui->setupUi(this);

	connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()));
	connect(ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()));
	connect(ui->verticalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()));
	connect(ui->verticalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()));
}

ClearColor::~ClearColor()
{
	delete ui;
}

void ClearColor::Screen::SetClearColor(float r_, float g_, float b_, float a_)
{
	r = r_;
	g = g_;
	b = b_;
	a = a_;
}

void ClearColor::Screen::paintGL()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_3_3_Core>();

	f->glClearColor(r, g, b, a);
	f->glClear(GL_COLOR_BUFFER_BIT);
}

void ClearColor::colorChanged()
{
	ui->openGLWidget->SetClearColor(ui->verticalSlider->value() / 255.0f,
		ui->verticalSlider_2->value() / 255.0f,
		ui->verticalSlider_3->value() / 255.0f,
		ui->verticalSlider_4->value() / 255.0f);

	ui->openGLWidget->update();
}