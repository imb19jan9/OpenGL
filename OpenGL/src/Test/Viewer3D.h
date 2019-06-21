#pragma once

#include <QWidget>
#include <QGLViewer/qglviewer.h>
#include <QMouseEvent>
#include <QWheelEvent>

#include "Model3D.h"
#include "Gizmo.h"
#include "FBO.h"

namespace Ui { class Viewer3D; };

class Viewer3D : public QWidget
{
	Q_OBJECT
private:
	QWidget* menu;

public:
	class Screen : public QGLViewer
	{
	public:
		enum GizmoType {
			TRANSLATE, ROTATE, SCALE
		};

	private:
		std::vector<Model3D*> models;
		std::list<Model3D*> selecteds;

		Gizmo* gizmo[3];
		GizmoType gizmoType;

		FBO* fbo;
		PhongShader* phong;
		SolidColorShader* solidColor;

	public:
		Screen(QWidget *parent = 0);
		~Screen();

		void AddModel(Model3D* model_);

		void SetGizmoType(GizmoType gizmoType_);

	private:
		virtual void init();
		virtual void preDraw() {}
		virtual void draw();
		virtual void postDraw() {}
		virtual void resizeGL(int width, int height);

		virtual void mousePressEvent(QMouseEvent *e_);
		virtual void mouseMoveEvent(QMouseEvent *e_);
		virtual void mouseReleaseEvent(QMouseEvent *e_);

		virtual void wheelEvent(QWheelEvent *e_);

		void ToggleSelection(int idx_);
	};

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
