#pragma once

#include <QWidget>
namespace Ui { class Ldni; };

class Ldni : public QWidget
{
	Q_OBJECT
private:
	QWidget *menu;

public:
	Ldni(QWidget *menu_);
	~Ldni();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }

private slots:
	void LoadMesh();
	void Run();

private:
	Ui::Ldni *ui;
};
