#include "ClearColor.h"
#include "ui_ClearColor.h"

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

void ClearColor::colorChanged()
{
	ui->openGLWidget->SetClearColor(ui->verticalSlider->value() / 255.0f,
		ui->verticalSlider_2->value() / 255.0f,
		ui->verticalSlider_3->value() / 255.0f,
		ui->verticalSlider_4->value() / 255.0f);

	ui->openGLWidget->update();
}