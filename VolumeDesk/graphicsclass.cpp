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
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
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
//	float * pVertexData = NULL;
	volume.generateRandom( 12 );
	volume.generateFaces( 0.5 );

	/*
	int * pDims = testVol.getDimensions();
	pVertexData = testVol.getValues();
	nFaces = pCubeLUT->getFaceCount(pVertexData, threshold, pDims[0], pDims[1], pDims[2]);
	*/

	/*
	float threshold = 1200.0;
	float * pVertexData = NULL;
	int dimensions[3];
	int * pDims = &dimensions[0];
	FileReader  skullReader;
	if (skullReader.LoadSkullDemo(&pDims[0], &pDims[1], &pDims[2], &pVertexData) == 0) {
	// "Error while reading skull demo data"
	return FALSE;
	} */

	/*
	dimensions[0] = pDims[0];
	dimensions[1] = pDims[1];
	dimensions[2] = pDims[2];
	*/

	// nFaces = pCubeLUT->getFaceCount(pVertexData, threshold, pDims[0], pDims[1], pDims[2]);

	// Total space required for buffer = axis vertices + solid vertices plus normals.
	//int nFloats = (nFaces * 3 * 4) + (nFaces * 3 * 4) + 36;

	// Total space required for buffer = axis vertices + solid vertices + normals.
	int nFloats = axis.getNumberOfFloats() + (2 * volume.getNumberOfFloats());


	// printf("3D data requires %d floats to store vertex and normal data.\r\n", nFloats);


	float * pAllValues = new float[nFloats];

	int copysize1 = sizeof(float) * axis.getNumberOfFloats();
	memcpy(pAllValues, axis.getVertices(), copysize1);

	//pCubeLUT->renderVolume(pVertexData, threshold, pDims[0], pDims[1], pDims[2], &pAllValues[axis.getNumberOfFloats()], &pAllValues[axis.getNumberOfFloats() + (vertices * 4)]);
	//delete[] pVertexData;

	int copysize2 = sizeof(float) * 2 * volume.getNumberOfFloats();
	memcpy(&pAllValues[copysize1], volume.getVertices(), copysize2 );

	int vertices = volume.getNumberOfVertices();

	volume.releaseFaces();

	GLuint buffer;
	m_OpenGL->glGenBuffers(1, &buffer);
	m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, buffer);
	m_OpenGL->glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * nFloats,
		pAllValues,
		GL_STATIC_DRAW);

	// We've loaded the array to OpenGL so we can dispose of it now.
	//delete[] pAllValues;

	// Declare fixed positions of 'in' parameters in the vertex shader.
	GLint position_index = 0;
	GLint object_colour_index = 1;
	GLint normals_index = 2;
	light_enable_index = m_OpenGL->glGetUniformLocation((GLuint)program, "enable_lighting");

	m_OpenGL->glVertexAttribPointer(position_index, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	m_OpenGL->glEnableVertexAttribArray(position_index);
	m_OpenGL->glVertexAttribPointer(normals_index, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * ((vertices * 4))));
	m_OpenGL->glEnableVertexAttribArray(normals_index);

	enableLighting = 0;

	//viewTransform = glm::translate(viewTransform, glm::vec3(0.0, 0.0, 0.65));

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

	enableLighting = 0;
//	m_OpenGL->glUniform1i(light_enable_index, enableLighting);

	// Now commence instructing the render pipline for each of the drawable objects.

	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	object_colour[0] = 1.0f;
	object_colour[1] = 1.0f;
	object_colour[2] = 0.0f;
	m_OpenGL->glVertexAttrib4fv(object_colour_index, object_colour);

	glLineWidth(2.0);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)0);
	*/

	m_OpenGL->glUniform1i(light_enable_index, axis.getLightingEnabled());
	m_OpenGL->glVertexAttrib4fv(object_colour_index, axis.getColour());
	//modelTransform = axis.getModelTransform();
	m_OpenGL->glUniformMatrix4fv(uniform_x_transform, 1, GL_FALSE, glm::value_ptr(modelTransform));
	axis.instructRenderer();

	/////////////////////

	// First, update the transformation for the object inside the 'cube' scaling down to fit the expected size

	/* int largestDim = (dimensions[0] > (dimensions[1] > dimensions[2] ? dimensions[1] : dimensions[2]) ? dimensions[0] : (dimensions[1] > dimensions[2] ? dimensions[1] : dimensions[2]));
	float xscaleobject = 1.0f / (float)dimensions[0];
	float yscaleobject = 1.0f / (float)dimensions[1];
	float zscaleobject = 1.0f / (float)dimensions[2];
	glm::mat4 appliedModelTransform = modelTransform;
	appliedModelTransform = glm::translate(appliedModelTransform, glm::vec3(-0.5, -0.5, -0.5));
	appliedModelTransform = glm::scale(appliedModelTransform, glm::vec3(xscaleobject, yscaleobject, zscaleobject));
	m_OpenGL->glUniformMatrix4fv(uniform_x_transform, 1, GL_FALSE, glm::value_ptr(appliedModelTransform)); */

	glm::mat4 appliedModelTransform = volume.getModelTransform(&modelTransform);
	m_OpenGL->glUniformMatrix4fv(uniform_x_transform, 1, GL_FALSE, glm::value_ptr(appliedModelTransform));

	/*
	object_colour[0] = 0.75f;
	object_colour[1] = 0.75f;
	object_colour[2] = 0.95f;
	m_OpenGL->glVertexAttrib4fv(object_colour_index, object_colour);
	*/

	//enableLighting = 1;
	m_OpenGL->glUniform1i(light_enable_index, volume.getLightingEnabled());
	m_OpenGL->glVertexAttrib4fv(object_colour_index, volume.getColour());
	volume.instructRenderer();
	/*
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 9, nFaces * 3);
	//glDrawArrays(GL_TRIANGLES, 9, 4 * 3);
	*/

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

