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
	GLfloat colour[3];

public:
	CDrawableObject();
	~CDrawableObject();

	// Functions to be implemented in derived classes.
	virtual int getNumberOfFloats() = 0;
	virtual int getNumberOfVertices() = 0;
	virtual GLfloat * getVertices() = 0;
	virtual int getNumberOfIndices() = 0;
	virtual GLint * getIndices() = 0;
	virtual void instructRenderer() = 0;
	virtual glm::mat4 getModelTransform(glm::mat4 *pModelTransform) = 0;

	// Concrete functions
	GLint getLightingEnabled() { return lightingEnabled; }
	GLfloat * getColour();
	void setColour(GLfloat r, GLfloat g, GLfloat b);

};

