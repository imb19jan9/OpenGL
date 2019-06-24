#pragma once

#include <QOpenGLWidget>

namespace CLEARCOLOR
{
	class Screen : public QOpenGLWidget
	{
	private:
		float r, g, b, a;

	public:
		Screen(QWidget* parent = 0) : QOpenGLWidget(parent),
			r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

		void SetClearColor(float r_, float g_, float b_, float a_);

	private:
		void paintGL();
	};
}
