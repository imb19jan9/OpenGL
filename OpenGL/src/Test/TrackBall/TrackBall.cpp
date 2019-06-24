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