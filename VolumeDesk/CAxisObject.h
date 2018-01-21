#pragma once

#include "CDrawableObject.h"

class CAxisObject :
	public CDrawableObject
{

protected:
	int nVertices;
	int nIndices;
	int nFaces;

	// Define the corners of the cube centered on 0.0 with edges 1.0 long.
	GLfloat axisCubeVals[36] = { 0.5,  0.5,  0.5,  1.0,
								-0.5,  0.5,  0.5,  1.0,
								-0.5, -0.5,  0.5,  1.0,
								0.5, -0.5,  0.5,  1.0,
								0.5,  0.5, -0.5,  1.0,
								-0.5,  0.5, -0.5,  1.0,
								-0.5, -0.5, -0.5,  1.0,
								0.5, -0.5, -0.5,  1.0,
								0.0,  0.0,  0.0,  1.0 };
	// Define the index table that OpenGL will use to sequence the vertices when line drawing the cube.
	int indices[24] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };
	GLfloat colour[3] = { 1.0f, 1.0f, 0.0f };

public:
	CAxisObject();
	~CAxisObject();

	int getNumberOfFloats();
	int getNumberOfVertices();
	GLfloat * getVertices();
	int getNumberOfIndices();
	GLint * getIndices();
	void instructRenderer();
	glm::mat4 getModelTransform(glm::mat4 *pModelTransform);
	GLfloat * getColour();

};

