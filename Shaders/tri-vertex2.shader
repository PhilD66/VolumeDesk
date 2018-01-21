#version 420 core                                                 
                 
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 object_colour;
layout(location = 2) in vec3 normal;
uniform int enable_lighting;
uniform mat4 frustum;
uniform mat4 modelTrx;
uniform mat4 viewTrx;

void main(void)                                                   
{                                                                 
    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  
                                   vec4(-0.25, -0.25, 0.5, 1.0),  
                                   vec4( 0.25,  0.25, 0.5, 1.0)); 
                                                                  
	mat4 viewModelTrx = viewTrx * modelTrx;
	vec4 worldPos = viewModelTrx * vertices[gl_VertexID];

    gl_Position = frustum * worldPos;
}      
