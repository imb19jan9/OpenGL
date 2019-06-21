#pragma once

#include <QMatrix4x4>
#include <QGLViewer/manipulatedFrame.h>
#include <QGLViewer/camera.h>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "ShaderProgram.h"
#include "TriMesh.h"

class GizmoFrame;
class GizmoFrameConstraint : public qglviewer::Constraint
{
private:
	GizmoFrame* gizmoFrame;
	qglviewer::AxisPlaneConstraint *constraint;

public:
	GizmoFrameConstraint(GizmoFrame* frame_);
	~GizmoFrameConstraint();

	void WRTWorld();
	void WRTLocal();

	void SetTranslationConstraint(qglviewer::AxisPlaneConstraint::Type type_,
		const qglviewer::Vec& direction_);
	void SetRotationConstraint(qglviewer::AxisPlaneConstraint::Type type_,
		const qglviewer::Vec& direction_);

private:
	virtual void constrainTranslation(qglviewer::Vec &translation,
		qglviewer::Frame *const frame);
	virtual void constrainRotation(qglviewer::Quaternion &rotation,
		qglviewer::Frame *const frame);
};

class GizmoFrame : public qglviewer::ManipulatedFrame
{
private:
	std::vector<qglviewer::Frame*> followers;
	GizmoFrameConstraint constraint;

public:
	GizmoFrame();
	~GizmoFrame();

	void Followed(qglviewer::Frame& frame_);
	void UnFollowed(qglviewer::Frame& frame_);

	void TranslateFollowers(qglviewer::Vec t_);
	void RotateFollowers(qglviewer::Vec axis_, qglviewer::Vec pos_, float angle_);

	void SetLocalConstraint() {
		constraint.WRTLocal();
	}
	void SetWorldConstraint() {
		constraint.WRTWorld();
	}

	void MoveOnlyXAxis() {
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(1, 0, 0));
	}
	void MoveOnlyYAxis() {
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 1, 0));
	}
	void MoveOnlyZAxis() {
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 0, 1));
	}
	void MoveFree() {
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::FREE,
			qglviewer::Vec(0, 0, 1));
	}

	void RotateOnlyXAxis() {
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(1, 0, 0));
	}
	void RotateOnlyYAxis() {
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 1, 0));
	}
	void RotateOnlyZAxis() {
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 0, 1));
	}
	void RotateFree() {
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::FREE,
			qglviewer::Vec(0, 0, 1));
	}
};

class Gizmo
{
public:
	GizmoFrame frame;

	VAO* vao;
	VBO* vbo;
	IBO* ibo;

	ShaderProgram* prog;

	TriMesh mesh;
	float screenFactor;
	bool dragging;

public:
	Gizmo();
	~Gizmo();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_) = 0;

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_) = 0;
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_) = 0;
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_) = 0;

	virtual void AdjustScale(const qglviewer::Camera& cam_) = 0;
	void AdjustPosition(qglviewer::Vec pos_) { frame.setPosition(pos_); }

	virtual inline bool IsHover() = 0;

	inline qglviewer::ManipulatedFrame& GetFrame() { return frame; }

	void Followed(qglviewer::Frame& frame_) {
		frame.Followed(frame_);
	}
	void UnFollowed(qglviewer::Frame& frame_) {
		frame.UnFollowed(frame_);
	}
};

class GizmoMove : public Gizmo
{
private:
	enum TranslateType {
		X_AXIS, Y_AXIS, Z_AXIS, NONE
	};
	TranslateType translateType;

public:
	GizmoMove();
	~GizmoMove();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_);

	virtual void AdjustScale(const qglviewer::Camera& cam_);

	virtual inline bool IsHover() { return translateType != NONE; }

private:
	void Create();

	void UpdateConstraint(QPoint p_, const qglviewer::Camera & cam_);
};

class GizmoRotate : public Gizmo
{
private:
	enum RotateType {
		X_AXIS, Y_AXIS, Z_AXIS, NONE
	};
	RotateType rotateType;

public:
	GizmoRotate();
	~GizmoRotate();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_);

	virtual void AdjustScale(const qglviewer::Camera& cam_);

	virtual inline bool IsHover() { return rotateType != NONE; }

private:
	void Create();

	void UpdateConstraint(QPoint p_, const qglviewer::Camera & cam_);
};

class GizmoScale : public Gizmo
{
public:
	GizmoScale();
	~GizmoScale();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_);

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_);

	virtual void AdjustScale(const qglviewer::Camera& cam_);

	virtual inline bool IsHover() { return true; }
};