#include "VAO.h"

#include <QOpenGLFunctions_3_3_Core>

VAO::VAO()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glDeleteVertexArrays(1, &id);
}

void VAO::AddBuffer(const VBO & vbo_, const VBOLayout & layout_)
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	Bind();
	vbo_.Bind();

	const auto& elements = layout_.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		f->glEnableVertexAttribArray(i);
		f->glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout_.GetStride(), (const void*)offset);
		offset += element.count * VBElement::GetSizeOfType(element.type);
	}
}

void VAO::Bind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindVertexArray(id);
}

void VAO::Unbind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindVertexArray(0);
}
