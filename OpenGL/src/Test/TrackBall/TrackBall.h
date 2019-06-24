#pragma once

#include <QWidget>

namespace Ui { class TrackBall; };

class TrackBall : public QWidget
{
	Q_OBJECT
private:
	QWidget *menu;

public:
	TrackBall(QWidget *menu_);
	~TrackBall();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private:
	Ui::TrackBall *ui;
};
