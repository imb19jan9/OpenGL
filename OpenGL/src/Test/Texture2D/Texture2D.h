#pragma once

#include <QWidget>




namespace Ui { class Texture2D; };

class Texture2D : public QWidget
{
	Q_OBJECT
private:
	QWidget *menu;

public:
	Texture2D(QWidget *menu_);
	~Texture2D();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private:
	Ui::Texture2D *ui;
};
