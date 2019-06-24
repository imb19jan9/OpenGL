#pragma once

#include <QWidget>

namespace Ui { class ClearColor; };

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
