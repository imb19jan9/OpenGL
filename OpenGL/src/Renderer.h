#pragma once

#include "VAO.h"
#include "IBO.h"
#include "ShaderProgram.h"

class Renderer {
private:

public:
	void Clear() const;

	void Draw(const VAO& vao_, const IBO& ibo_, const ShaderProgram& program_);
};