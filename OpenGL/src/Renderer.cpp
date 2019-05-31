#include "Renderer.h"

#include <QOpenGLFunctions_3_3_Core>

void Renderer::Clear() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glClearColor(0.0, 0.0, 0.0, 1.0);
	f->glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VAO & vao_, const IBO & ibo_, const ShaderProgram & program_)
{
	program_.Bind();
	vao_.Bind();
	ibo_.Bind();

	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glDrawElements(GL_TRIANGLES, ibo_.GetCount(), GL_UNSIGNED_INT, 0);
}
