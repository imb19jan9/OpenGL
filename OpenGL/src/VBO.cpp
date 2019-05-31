#include "VBO.h"

#include <QOpenGLFunctions_3_3_Core>

VBO::VBO(const void * data_, unsigned int size_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glGenBuffers(1, &id);
	f->glBindBuffer(GL_ARRAY_BUFFER, id);
	f->glBufferData(GL_ARRAY_BUFFER, size_, data_, GL_STATIC_DRAW);
}

VBO::~VBO()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glDeleteBuffers(1, &id);
}

void VBO::Bind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindBuffer(GL_ARRAY_BUFFER, 0);
}
