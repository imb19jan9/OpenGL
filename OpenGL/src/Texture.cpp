#include "Texture.h"

#include <QOpenGLFunctions_3_3_Core>

Texture::Texture(const std::string filePath_)
	: filePath(filePath_)
{
	img.load(filePath.c_str());
	img = img.mirrored();
	img.convertToFormat(QImage::Format::Format_RGBA8888);

	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glGenTextures(1, &id);
	f->glBindTexture(GL_TEXTURE_2D, id);
	
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 
		img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
}

Texture::~Texture()
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int slot) const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind() const
{
	QOpenGLFunctions_3_3_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindTexture(GL_TEXTURE_2D, 0);
}
