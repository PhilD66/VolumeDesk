#include "stdafx.h"
#include "CDrawableObject.h"


CDrawableObject::CDrawableObject()
{
	// Default settings for objects
	lightingEnabled = 1;
	setColour(0.5f, 0.5f, 0.5f);
}


CDrawableObject::~CDrawableObject()
{
}

GLfloat * CDrawableObject::getColour()
{
	return colour;
}

void CDrawableObject::setColour(GLfloat r, GLfloat g, GLfloat b)
{
	colour[0] = r;
	colour[1] = g;
	colour[2] = b;
}
