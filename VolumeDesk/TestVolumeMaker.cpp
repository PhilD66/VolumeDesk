#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "TestVolumeMaker.h"

TestVolumeMaker::TestVolumeMaker()
{
}

TestVolumeMaker::~TestVolumeMaker()
{
}

void TestVolumeMaker::generateCube( int size )
{
	if (pNodeValues != NULL) {
		delete [] pNodeValues;
	}

	dimensions[0] = size;
	dimensions[1] = size;
	dimensions[2] = size;
	nNodes = dimensions[0] * dimensions[1] * dimensions[2];
	pNodeValues = new float[nNodes];

	for (int z = 0; z < dimensions[2]; z++) {
        for (int y = 0; y < dimensions[1]; y++) {
            for (int x = 0; x < dimensions[0]; x++) {
                if ( (z > 2 && z < (dimensions[2] - 2)) && (x > 2 && x < (dimensions[0] - 2)) && (y > 2 && y < (dimensions[1] - 2)) ) {
                    pNodeValues[((z * dimensions[0] * dimensions[1]) + (y * dimensions[0])) + x] = 1.0;
                } else {
                    pNodeValues[((z * dimensions[0] * dimensions[1]) + (y * dimensions[0])) + x] = 0.0;
                }
            }
        }
    }
}

void TestVolumeMaker::generateRandom( int size )
{
	if (pNodeValues != NULL) {
		delete [] pNodeValues;
	}

	dimensions[0] = size;
	dimensions[1] = size;
	dimensions[2] = size;
	nNodes = dimensions[0] * dimensions[1] * dimensions[2];
	pNodeValues = new float[nNodes];
	
	srand((unsigned int)time(NULL));
    for (int z = 0; z < dimensions[2]; z++) {
        for (int y = 0; y < dimensions[1]; y++) {
            for (int x = 0; x < dimensions[0]; x++) {
                if ( (z > 2 && z < (dimensions[2] - 2)) && (x > 2 && x < (dimensions[0] - 2)) && (y > 2 && y < (dimensions[1] - 2)) ) {
                    pNodeValues[((z * dimensions[0] * dimensions[1]) + (y * dimensions[0])) + x] = 0.35f + ((float)(rand() % 100) / 200.0f);
                } else {
                    pNodeValues[((z * dimensions[0] * dimensions[1]) + (y * dimensions[0])) + x] = 0.0f;
                }
            }
        }
    }
}

void TestVolumeMaker::testPrintValues()
{
    for (int x = 0; x < dimensions[0]; x++) {
        for (int y = 0; y < dimensions[1]; y++) {
            for (int z = 0; z < dimensions[2]; z++) {
                printf("%1.1f, ", pNodeValues[((x * dimensions[1] * dimensions[2]) + (y * dimensions[2])) + z]);
            }
            printf("\r\n");
        }
    }
}


void TestVolumeMaker::instructRenderer()
{
	glDisable(GL_BLEND);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 9, nFaces * 3);
}

glm::mat4 TestVolumeMaker::getModelTransform(glm::mat4 *pModelTransform)
{
	// Generate a proportional scale based on getting the largest dimension down to fit the rendering cube of 1.0 squared.
	float largestDim = (float)(dimensions[0] > (dimensions[1] > dimensions[2] ? dimensions[1] : dimensions[2]) ? dimensions[0] : (dimensions[1] > dimensions[2] ? dimensions[1] : dimensions[2]));
	float scaleobject = 1.0f / largestDim;
	glm::mat4 appliedModelTransform = *pModelTransform;
	appliedModelTransform = glm::translate(appliedModelTransform, glm::vec3(-0.5, -0.5, -0.5));
	appliedModelTransform = glm::scale(appliedModelTransform, glm::vec3(scaleobject, scaleobject, scaleobject));
	return appliedModelTransform;
}

