////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openglclass.h"
#include "CubeLUT.h"
#include "CAxisObject.h"
#include "TestVolumeMaker.h"

/////////////
// GLOBALS //
/////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown();
	bool Frame();
	void OnResizeWindow(int screenWidth, int screenHeight);
	void OnMouseMove(int xMove, int yMove);

private:
	CCubeFaceLUT *pCubeLUT;
	PFNGLCREATEPROGRAMPROC program;
	GLuint	vao;
	glm::mat4 viewTransform;
	glm::mat4 modelTransform;
	glm::mat4 tempTransform;
	int windowWidth;
	int windowHeight;
	glm::vec3 invModelXAxis;
	glm::vec3 invModelYAxis;
	GLuint uniform_frustum;
	GLuint uniform_x_transform;
	GLuint uniform_viewtrx;

	// Declare fixed positions of 'in' parameters in the vertex shader.
	GLint position_index = 0;
	GLint object_colour_index = 1;
	GLint normals_index = 2;
	GLint light_enable_index = 99;
	GLint enableLighting = 0;
	int dimensions[3];
	int *pDims;
	int nFaces = 0;
	GLuint elementbuffer;
	GLuint buffer;
	CAxisObject	axis;
	TestVolumeMaker volume;

	bool Render();

private:
	OpenGLClass* m_OpenGL;
};

#endif