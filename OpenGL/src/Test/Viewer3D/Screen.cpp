#include "Screen.h"

#include <QGLViewer/manipulatedCameraFrame.h>

namespace VIEWER3D
{
	static const int MODELS_OFFSET = 100;

	QVector4D INTToColor(int num_)
	{
		int r = num_ / 65536;
		int g = (num_ - r * 65536) / 256;
		int b = (num_ - r * 65536 - g * 256);

		return QVector4D(r / 255.0, g / 255.0, b / 255.0, 1.0);
	}

	int ColorToINT(QVector4D color_)
	{
		color_ = color_ * 255.0;
		int num = color_[2] + color_[1] * 256 + color_[0] * 256 * 256;

		return num;
	}

	Screen::Screen(QWidget *parent)
		: QGLViewer(parent),
		gizmoType(TRANSLATE),
		fbo(0),
		phong(0),
		solidColor(0)
	{
		gizmo[0] = new GizmoMove;
		gizmo[1] = new GizmoRotate;
		gizmo[2] = new GizmoScale;
	}

	Screen::~Screen()
	{
		for (int i = 0; i < 3; i++)
			delete gizmo[i];

		delete fbo;
		delete phong;
		delete solidColor;
	}

	void Screen::AddModel(Model3D * model_)
	{
		makeCurrent();
		model_->Init();
		models.push_back(model_);
	}

	void Screen::SetGizmoType(GizmoType gizmoType_)
	{
		gizmoType = gizmoType_;
		gizmo[gizmoType]->AdjustScale(*camera());

		qglviewer::Vec pos(0, 0, 0);
		for (std::list<Model3D*>::iterator it = selecteds.begin();
			it != selecteds.end();
			it++)
			pos += (*it)->CenterOfMass();
		pos /= selecteds.size();

		gizmo[gizmoType]->AdjustPosition(pos);
	}

	void Screen::init()
	{
		QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
			versionFunctions<QOpenGLFunctions_4_5_Core>();

		f->glDisable(GL_LIGHT0);
		f->glDisable(GL_LIGHTING);
		f->glDisable(GL_COLOR_MATERIAL);

		setSceneRadius(50);
		showEntireScene();
		camera()->frame()->setSpinningSensitivity(1000);

		setMouseTracking(true);

		for (int i = 0; i < 3; i++)
			gizmo[i]->Init();

		phong = new PhongShader;
		solidColor = new SolidColorShader;
	}

	void Screen::draw()
	{
		QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
			versionFunctions<QOpenGLFunctions_4_5_Core>();

		f->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		QMatrix4x4 proj, view;
		camera()->getModelViewMatrix(view.data());
		camera()->getProjectionMatrix(proj.data());

		for (int i = 0; i < models.size(); i++) {
			if (std::find(selecteds.begin(), selecteds.end(),
				models[i]) != selecteds.end())
				models[i]->SetColor(QVector4D(0.0, 1.0, 0.0, 1.0));
			else
				models[i]->SetColor(QVector4D(0.0, 0.0, 0.0, 1.0));

			phong->Predraw(view, proj, *models[i]);
			models[i]->Draw();
		}

		if (!selecteds.empty())
			gizmo[gizmoType]->Draw(view, proj);

		fbo->Bind();
		f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < models.size(); i++) {
			QVector4D color = INTToColor(i + MODELS_OFFSET);
			models[i]->SetColor(color);
			solidColor->Predraw(view, proj, *models[i]);
			models[i]->Draw();
		}

		fbo->Unbind();
	}

	void Screen::resizeGL(int width, int height)
	{
		QGLViewer::resizeGL(width, height);

		gizmo[gizmoType]->AdjustScale(*camera());

		delete fbo;
		fbo = new FBO(width, height);
	}

	void Screen::mousePressEvent(QMouseEvent *e_)
	{
		gizmo[gizmoType]->MousePressed(e_->pos(), *camera());

		if (!gizmo[gizmoType]->IsHover()) {
			makeCurrent();
			QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
				versionFunctions<QOpenGLFunctions_4_5_Core>();
			fbo->Bind();
			float rgb[3];
			QPoint cursor(e_->pos());
			f->glReadPixels(cursor.x(), height() - cursor.y(), 1, 1, GL_RGB, GL_FLOAT, rgb);
			fbo->Unbind();

			QVector4D color(rgb[0], rgb[1], rgb[2], 1.0);
			int idx = ColorToINT(color) - MODELS_OFFSET;
			if (idx >= 0)
				ToggleSelection(idx);
		}

		QGLViewer::mousePressEvent(e_);
		update();
	}

	void Screen::mouseMoveEvent(QMouseEvent *e_)
	{
		QPoint cursor(e_->pos());
		gizmo[gizmoType]->MouseMoved(cursor, *camera());
		//FIXME : should not depend on QGLviewer functions.
		//need to handle mouse interaction with my own implementation.
		if (gizmo[gizmoType]->IsHover()) {
			setManipulatedFrame(&gizmo[gizmoType]->GetFrame());
			if (gizmoType == TRANSLATE)
				setMouseBinding(Qt::NoModifier, Qt::LeftButton, FRAME,
					QGLViewer::MouseAction::TRANSLATE);
			else if (gizmoType == ROTATE)
				setMouseBinding(Qt::NoModifier, Qt::LeftButton, FRAME,
					QGLViewer::MouseAction::ROTATE);
			else if (gizmoType == SCALE)
				std::cout << "Scale gizmo is not ready" << std::endl;
		}
		else {
			setManipulatedFrame(0);
			setMouseBinding(Qt::NoModifier, Qt::LeftButton, CAMERA,
				QGLViewer::MouseAction::ROTATE);
		}

		QGLViewer::mouseMoveEvent(e_);
		update();
	}

	void Screen::mouseReleaseEvent(QMouseEvent *e_)
	{
		gizmo[gizmoType]->MouseReleased(e_->pos(), *camera());

		QGLViewer::mouseReleaseEvent(e_);
	}

	void Screen::wheelEvent(QWheelEvent * e_)
	{
		QGLViewer::wheelEvent(e_);
		gizmo[gizmoType]->AdjustScale(*camera());
		update();
	}

	void Screen::ToggleSelection(int idx_)
	{
		if (std::find(selecteds.begin(), selecteds.end(),
			models[idx_]) == selecteds.end()) {
			selecteds.push_back(models[idx_]);
			gizmo[TRANSLATE]->Followed(models[idx_]->GetFrame());
			gizmo[ROTATE]->Followed(models[idx_]->GetFrame());
			gizmo[SCALE]->Followed(models[idx_]->GetFrame());
		}
		else {
			selecteds.remove(models[idx_]);
			gizmo[TRANSLATE]->UnFollowed(models[idx_]->GetFrame());
			gizmo[ROTATE]->UnFollowed(models[idx_]->GetFrame());
			gizmo[SCALE]->UnFollowed(models[idx_]->GetFrame());
		}

		qglviewer::Vec pos(0, 0, 0);
		for (std::list<Model3D*>::iterator it = selecteds.begin();
			it != selecteds.end();
			it++)
			pos += (*it)->CenterOfMass();
		pos /= selecteds.size();
		gizmo[gizmoType]->AdjustPosition(pos);
	}
}