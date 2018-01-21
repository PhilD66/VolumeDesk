#pragma once

#include "CDrawableObject.h"

class TestVolumeMaker :
	public CDrawableObject
{
protected:
    int nValues;
    int dimensions[3];
    float * pNodeValues;

	GLint indices[1];
	GLfloat colour[3] = { 0.75f, 0.75f, 0.95f };
	int nVertices;
	int nIndices;
	int nFaces;
	float * pCoordValues;

public:
    TestVolumeMaker();
    ~TestVolumeMaker();
    void generateCube( int size );
    void generateRandom( int size );
//    int * getDimensions();
//    float * getValues();
    void testPrintValues();
	void generateFaces( float threshold );
	void releaseFaces();

	// Implementations of base class CDrawableObject
	int getNumberOfFloats();
	int getNumberOfVertices();
	GLfloat * getVertices();
	int getNumberOfIndices();
	GLint * getIndices();
	void instructRenderer();
	glm::mat4 getModelTransform(glm::mat4 *pModelTransform);
	GLfloat * getColour();

};
