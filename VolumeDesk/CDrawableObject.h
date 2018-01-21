#pragma once

// Include GL here because the derived classes will all need it.
#include <gl\gl.h>
#include <math.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

class CDrawableObject
{
protected:
	int lightingEnabled;

public:
	CDrawableObject();
	~CDrawableObject();
	virtual int getNumberOfFloats() = 0;
	virtual int getNumberOfVertices() = 0;
	virtual GLfloat * getVertices() = 0;
	virtual int getNumberOfIndices() = 0;
	virtual GLint * getIndices() = 0;
	virtual void instructRenderer() = 0;
	virtual glm::mat4 getModelTransform(glm::mat4 *pModelTransform) = 0;
	virtual GLfloat * getColour() = 0;
	GLint getLightingEnabled() { return lightingEnabled; }
};

