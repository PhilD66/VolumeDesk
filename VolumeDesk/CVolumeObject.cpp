#include "stdafx.h"
#include "CVolumeObject.h"
#include "CubeLUT.h"


CVolumeObject::CVolumeObject()
{
	nNodes = 0;
	dimensions[0] = 0;
	dimensions[1] = 0;
	dimensions[2] = 0;
	nVertices = 0;
	nIndices = 0;
	pNodeValues = NULL;
	pCoordValues = NULL;
	setColour(0.5f, 0.5f, 0.75f);
}


CVolumeObject::~CVolumeObject()
{
	if (pNodeValues != NULL) {
		delete[] pNodeValues;
	}

	if (pCoordValues != NULL) {
		delete[] pCoordValues;
	}
}

int CVolumeObject::getNumberOfFloats()
{
	return nVertices * 4;
}

int CVolumeObject::getNumberOfVertices()
{
	return nVertices;
}

int CVolumeObject::getNumberOfIndices()
{
	return nIndices;
}

GLfloat * CVolumeObject::getVertices()
{
	return pCoordValues;
}

GLint * CVolumeObject::getIndices()
{
	return &indices[0];
}

float ** CVolumeObject::getNodesPtr()
{
	return &pNodeValues;
}

int * CVolumeObject::getXDimPtr()
{
	return &dimensions[0];
}

int * CVolumeObject::getYDimPtr()
{
	return &dimensions[1];
}

int * CVolumeObject::getZDimPtr()
{
	return &dimensions[2];
}

void CVolumeObject::releaseFaces()
{
	if (pCoordValues == NULL)
	{
		AfxThrowMemoryException();
	}
	else
	{
		delete[] pCoordValues;
		pCoordValues = NULL;
	}
}

void CVolumeObject::generateFaces(float threshold)
{
	if (pCoordValues != NULL)
	{
		AfxThrowMemoryException();
	}

	CCubeFaceLUT	cubeLut;
	nFaces = cubeLut.getFaceCount(pNodeValues, threshold, dimensions[0], dimensions[1], dimensions[2]);
	nVertices = nFaces * 3;

	// Make space for the vertices and the normals.
	int sizeOfCoordBuffer = 2 * getNumberOfFloats();
	pCoordValues = new float[sizeOfCoordBuffer];

	cubeLut.renderVolume(pNodeValues, threshold, dimensions[0], dimensions[1], dimensions[2], &pCoordValues[0], &pCoordValues[getNumberOfFloats()]);
}

void CVolumeObject::instructRenderer()
{
	glDisable(GL_BLEND);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 9, nFaces * 3);
}

glm::mat4 CVolumeObject::getModelTransform(glm::mat4 *pModelTransform)
{
	// Generate a proportional scale based on getting the largest dimension down to fit the rendering cube of 1.0 squared.
	float largestDim = (float)(dimensions[0] > (dimensions[1] > dimensions[2] ? dimensions[1] : dimensions[2]) ? dimensions[0] : (dimensions[1] > dimensions[2] ? dimensions[1] : dimensions[2]));
	float xScale = scale[0] / largestDim;
	float yScale = scale[1] / largestDim;
	float zScale = scale[2] / largestDim;
	glm::mat4 appliedModelTransform = *pModelTransform;
	appliedModelTransform = glm::translate(appliedModelTransform, glm::vec3(-0.5, -0.5, -0.5));
	appliedModelTransform = glm::scale(appliedModelTransform, glm::vec3(xScale, yScale, zScale));
	return appliedModelTransform;
}


