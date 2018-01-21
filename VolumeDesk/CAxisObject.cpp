#include "stdafx.h"
#include "CAxisObject.h"


CAxisObject::CAxisObject()
{
	nVertices = 9;
	nIndices = 24;
	lightingEnabled = 0;
}


CAxisObject::~CAxisObject()
{
}


int CAxisObject::getNumberOfFloats()
{
	return nVertices * 4;
}

int CAxisObject::getNumberOfVertices()
{
	return nVertices;
}

GLfloat * CAxisObject::getVertices()
{
	return &axisCubeVals[0];
}

int CAxisObject::getNumberOfIndices()
{
	return nIndices;
}

GLint * CAxisObject::getIndices()
{
	return &indices[0];
}

void CAxisObject::instructRenderer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(2.0);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)0);
}

glm::mat4 CAxisObject::getModelTransform(glm::mat4 *pModelTransform)
{
	return glm::mat4(*pModelTransform);
}

GLfloat * CAxisObject::getColour()
{
	return colour;
}
