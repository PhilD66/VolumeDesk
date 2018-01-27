#pragma once

#include "CVolumeObject.h"

class TestVolumeMaker :
	public CVolumeObject
{
protected:

public:
    TestVolumeMaker();
    ~TestVolumeMaker();
    void generateCube( int size );
    void generateRandom( int size );
    void testPrintValues();

	// Implementations of base class CDrawableObject
	void instructRenderer();
	glm::mat4 getModelTransform(glm::mat4 *pModelTransform);

};
