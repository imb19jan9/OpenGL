#pragma once

#include <string>
#include <QImage>

class Texture {
private:
	unsigned int id;
	std::string filePath;
	QImage img;
	int bytePerPixel;

public:
	Texture(const std::string filePath_);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return img.width(); }
	inline int GetHeight() const { return img.height(); }
};
