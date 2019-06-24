#include "Texture2D.h"
#include "ui_Texture2D.h"

#include <QOpenGLFunctions_4_5_Core>

Texture2D::Texture2D(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::Texture2D();
	ui->setupUi(this);
}

Texture2D::~Texture2D()
{
	delete ui;
}

