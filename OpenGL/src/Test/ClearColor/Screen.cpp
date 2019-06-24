#include "Screen.h"

#include <QOpenGLFunctions_4_5_Core>
#include <iostream>

namespace CLEARCOLOR
{
	void Screen::SetClearColor(float r_, float g_, float b_, float a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	void Screen::paintGL()
	{
		QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
			versionFunctions<QOpenGLFunctions_4_5_Core>();

		f->glClearColor(r, g, b, a);
		f->glClear(GL_COLOR_BUFFER_BIT);
	}
}