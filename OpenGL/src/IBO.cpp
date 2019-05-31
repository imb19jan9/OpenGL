#include "IBO.h"

#include <QOpenGLFunctions_3_3_Core>

IBO::IBO(const unsigned int * data_, unsigned int count_)
	: count(count_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glGenBuffers(1, &id);
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data_, GL_STATIC_DRAW);
}

IBO::~IBO()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glDeleteBuffers(1, &id);
}

void IBO::Bind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IBO::Unbind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
