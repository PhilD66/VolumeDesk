#pragma once
#include "CDrawableObject.h"

class CVolumeObject :
	public CDrawableObject
{
protected:
	int nNodes;			// Number of data points in the volume. Must be = (dimensions[0] * dimensions[1] * dimensions[2]).
	int nVertices;
	int nIndices;
	int nFaces;
	int dimensions[3];
	float * pNodeValues;
	GLint indices[1];
	float * pCoordValues;

public:
	CVolumeObject();
	~CVolumeObject();

	int getNumberOfFloats();
	int getNumberOfVertices();
	int getNumberOfIndices();
	GLfloat * getVertices();
	GLint * getIndices();
	void generateFaces(float threshold);
	void releaseFaces();

	float ** getNodesPtr();
	int * getXDimPtr();
	int * getYDimPtr();
	int * getZDimPtr();

	// Implementations of base class CDrawableObject
	void instructRenderer();
	glm::mat4 getModelTransform(glm::mat4 *pModelTransform);

};

