////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "graphicsclass.h"
#include "FileReader.hpp"
#include "CShader.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "CAxisObject.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GraphicsClass::GraphicsClass()
{
	m_OpenGL = 0;
	invModelXAxis = glm::vec3(1.0, 0.0, 0.0);
	invModelYAxis = glm::vec3(0.0, 1.0, 0.0);
	tempTransform = glm::mat4(1.0);
	pVolume = NULL;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
	pVolume = NULL;
}


GraphicsClass::~GraphicsClass()
{
	if (pVolume != NULL)
	{
		delete pVolume;
	}
}


bool GraphicsClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	// Store a pointer to the OpenGL class object.
	m_OpenGL = OpenGL;

	// Vertex shader source code. This draws the cube that bounds the volume to be rendered.
	CShader vertexShader;
	if (vertexShader.LoadFile("C:\\Users\\Phil\\source\\repos\\VolumeDesk\\Shaders\\vertex.shader") == 0) {
		// Throw exception "Failed to load vertex shader file."
		return FALSE;
	}

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	CShader fragmentShader;
	if (fragmentShader.LoadFile("C:\\Users\\Phil\\source\\repos\\VolumeDesk\\Shaders\\fragment.shader") == 0) {
		// Throw exception "Failed to load vertex shader file."
		return FALSE;
	}

	program = (PFNGLCREATEPROGRAMPROC)m_OpenGL->glCreateProgram();
	char const * pFragmentSh[] = { fragmentShader.getShaderCode() };
	GLuint fs = m_OpenGL->glCreateShader(GL_FRAGMENT_SHADER);
	m_OpenGL->glShaderSource(fs, 1, pFragmentSh, NULL);
	m_OpenGL->glCompileShader(fs);

	char const * pVertexSh[] = { vertexShader.getShaderCode() };
	GLuint vs = m_OpenGL->glCreateShader(GL_VERTEX_SHADER);
	m_OpenGL->glShaderSource(vs, 1, pVertexSh, NULL);
	m_OpenGL->glCompileShader(vs);

	m_OpenGL->glAttachShader((GLuint)program, vs);
	m_OpenGL->glAttachShader((GLuint)program, fs);

	m_OpenGL->glLinkProgram((GLuint)program);

	m_OpenGL->glGenVertexArrays(1, &vao);
	m_OpenGL->glBindVertexArray(vao);

	m_OpenGL->glUseProgram((GLuint)program);

	m_OpenGL->glDeleteShader(fs);
	m_OpenGL->glDeleteShader(vs);

	// Obtain the position of key variables within the shader(s) 
	uniform_frustum = m_OpenGL->glGetUniformLocation((GLuint)program, "frustum");
	uniform_x_transform = m_OpenGL->glGetUniformLocation((GLuint)program, "modelTrx");
	uniform_viewtrx = m_OpenGL->glGetUniformLocation((GLuint)program, "viewTrx");

	// Generate a buffer for the indices
	m_OpenGL->glGenBuffers(1, &elementbuffer);
	m_OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	m_OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), axis.getIndices(), GL_STATIC_DRAW);

//	float threshold = 0.5;
	if (0)
	{
		TestVolumeMaker *pTestVolume = new TestVolumeMaker();

		pTestVolume->generateRandom(15);

		pTestVolume->generateFaces(0.5);

		pVolume = (CVolumeObject *)pTestVolume;
	}
	else
	{
		pVolume = new CVolumeObject();

		/*
		int size = 10;
		int dimensions[3];
		dimensions[0] = size;
		dimensions[1] = size;
		dimensions[2] = size;
		*pVolume->getXDimPtr() = size;
		*pVolume->getYDimPtr() = size;
		*pVolume->getZDimPtr() = size;

		float **pNodeValues = pVolume->getNodesPtr();
		*pNodeValues = new float[size * size * size];

		srand((unsigned int)time(NULL));
		for (int z = 0; z < dimensions[2]; z++) {
			for (int y = 0; y < dimensions[1]; y++) {
				for (int x = 0; x < dimensions[0]; x++) {
					if ((z > 2 && z < (dimensions[2] - 2)) && (x > 2 && x < (dimensions[0] - 2)) && (y > 2 && y < (dimensions[1] - 2))) {
						(*pNodeValues)[((z * dimensions[0] * dimensions[1]) + (y * dimensions[0])) + x] = 0.35f + ((float)(rand() % 100) / 200.0f);
					}
					else {
						(*pNodeValues)[((z * dimensions[0] * dimensions[1]) + (y * dimensions[0])) + x] = 0.0f;
					}
				}
			}
		}
		*/

		FileReader  skullReader;
		if (skullReader.LoadSkullDemo2(pVolume->getXDimPtr(), pVolume->getYDimPtr(), pVolume->getZDimPtr(), pVolume->getNodesPtr()) == 0) {
			// "Error while reading skull demo data"
			return FALSE;
		}

		// Compensate for the lower number of samples in the Z-axis. Nasty hard-coded hack for now!
		pVolume->setScale(1.0f, 1.0f, (256.0f / 113.0f));

		pVolume->generateFaces(1200.0);
	}

	// Total space required for buffer = axis vertices + solid vertices + normals.
	int buffSize1 = axis.getNumberOfFloats();
	int buffSize2 = 2 * pVolume->getNumberOfFloats();
	int nFloats = buffSize1 + buffSize2;

	// Allocate enough storage for all that...
	float * pAllValues = new float[nFloats];

	// Copy from the axis
	memcpy(pAllValues, axis.getVertices(), sizeof(float) * buffSize1);
	// Copy from the volume
	memcpy(&pAllValues[buffSize1], pVolume->getVertices(), sizeof(float) * buffSize2 );

	// Number 
	int vertices = 1 * pVolume->getNumberOfVertices();

	pVolume->releaseFaces();

	GLuint buffer;
	m_OpenGL->glGenBuffers(1, &buffer);
	m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, buffer);
	m_OpenGL->glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * nFloats,
		pAllValues,
		GL_STATIC_DRAW);

	// We've loaded the array to OpenGL so we can dispose of it now.
	delete[] pAllValues;

	// Declare fixed positions of 'in' parameters in the vertex shader.
	GLint position_index = 0;
	GLint object_colour_index = 1;
	GLint normals_index = 2;
	light_enable_index = m_OpenGL->glGetUniformLocation((GLuint)program, "enable_lighting");

	m_OpenGL->glVertexAttribPointer(position_index, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	m_OpenGL->glEnableVertexAttribArray(position_index);
	m_OpenGL->glVertexAttribPointer(normals_index, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * ((vertices * 4))));
	m_OpenGL->glEnableVertexAttribArray(normals_index);

	modelTransform = glm::mat4(1.0);
	viewTransform = glm::mat4(1.0);
	viewTransform = glm::translate(viewTransform, glm::vec3(0.0, 0.0, 0.65));

	RECT winRect;
	GetWindowRect(hwnd, &winRect);
	glViewport(0, 0, winRect.right - winRect.left, winRect.bottom - winRect.top);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the pointer to the OpenGL class object.
	m_OpenGL = 0;

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_DEPTH_CLAMP);

	GLfloat object_colour[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	m_OpenGL->glVertexAttrib4fv(object_colour_index, object_colour);

	GLfloat scale = 1.2f;
	GLfloat aspect_ratio = (float)windowWidth / (float)windowHeight;

	GLfloat frustum[16] = { scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale * aspect_ratio, 0.0f, 0.0f,
		0.0f, 0.0f, scale * aspect_ratio, scale,
		0.0f, 0.0f, 0.0f, 1.0f };

	m_OpenGL->glUniformMatrix4fv(uniform_frustum, 1, GL_FALSE, &frustum[0]);

	m_OpenGL->glUniformMatrix4fv(uniform_x_transform, 1, GL_FALSE, glm::value_ptr(modelTransform));

	// Set up the transformation for the view which is just identity for now.
	m_OpenGL->glUniformMatrix4fv(uniform_viewtrx, 1, GL_FALSE, glm::value_ptr(viewTransform));

	// Volume 
	glm::mat4 appliedModelTransform = pVolume->getModelTransform(&modelTransform);
	m_OpenGL->glUniformMatrix4fv(uniform_x_transform, 1, GL_FALSE, glm::value_ptr(appliedModelTransform));

	m_OpenGL->glUniform1i(light_enable_index, pVolume->getLightingEnabled());
	m_OpenGL->glVertexAttrib4fv(object_colour_index, pVolume->getColour());
	pVolume->instructRenderer();

	// Axis
	m_OpenGL->glUniform1i(light_enable_index, axis.getLightingEnabled());
	m_OpenGL->glVertexAttrib4fv(object_colour_index, axis.getColour());
	m_OpenGL->glUniformMatrix4fv(uniform_x_transform, 1, GL_FALSE, glm::value_ptr(modelTransform));
	axis.instructRenderer();

	// Present the rendered scene to the screen.
	m_OpenGL->EndScene();

	return true;
}

void GraphicsClass::OnResizeWindow(int screenWidth, int screenHeight)
{
	windowWidth = screenWidth;
	windowHeight = screenHeight;

	glViewport(0, 0, screenWidth, screenHeight);
}

void GraphicsClass::OnMouseMove(int xMove, int yMove)
{
	if (m_OpenGL != NULL) {
		TRACE("Mouse move: x = %d, y = %d\n", xMove, yMove);
		tempTransform = glm::rotate(tempTransform, (float)(-0.002 * 3.1415927 * (float)xMove), invModelYAxis);
		modelTransform = tempTransform * modelTransform;
		tempTransform = glm::mat4(1.0);
		tempTransform = glm::rotate(tempTransform, (float)(-0.002 * 3.1415927 * (float)yMove), invModelXAxis);
		modelTransform = tempTransform * modelTransform;
		tempTransform = glm::mat4(1.0);
	}
}

