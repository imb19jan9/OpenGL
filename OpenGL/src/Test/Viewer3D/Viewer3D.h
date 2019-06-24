#pragma once

#include <QWidget>

namespace Ui { class Viewer3D; };

class Viewer3D : public QWidget
{
	Q_OBJECT
private:
	QWidget* menu;

public:
	Viewer3D(QWidget *menu_);
	~Viewer3D();

private:
	void closeEvent(QCloseEvent* e) { menu->setVisible(true); }
	
private slots:
	void LoadModel();
	void GizmoChanged();

private:
	Ui::Viewer3D *ui;
};
