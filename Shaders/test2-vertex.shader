#version 420 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 object_colour;
layout(location = 2) in vec3 normal;
uniform int enable_lighting;
uniform mat4 frustum;
uniform mat4 modelTrx;
uniform mat4 viewTrx;

out VS_OUT
{
	vec3 lColour;
} vs_out;

void main(void)
{
	// Define light direction. TBD: pass in as a variable.            
	const vec3 lightPos = vec3(-20.0, 20.0, -20.0);
	const vec3 ambientLight = vec3(0.2, 0.2, 0.2);
	const vec3 diffuseLight = vec3(0.5, 0.7, 0.7);
	const vec3 specLight = vec3(0.6, 0.6, 0.6);
	const float specPower = 128.0;

	mat4 viewModelTrx = viewTrx * modelTrx;
	vec4 worldPos = viewModelTrx * position;

	if (enable_lighting == 1) {

		// Calculate the normal in view space                         
		vec3 N = mat3(viewModelTrx) * normal;
		// Calculate the view-space light vector                      
		vec3 L = lightPos - worldPos.xyx;
		// Calculate the view vector (negative of view-space pos)     
		vec3 V = -worldPos.xyz;

		// Normalize all three vectors                                
		N = normalize(N);
		L = normalize(L);
		V = normalize(V);

		// Calculate R by reflecting -L around the plane defined by N 
		vec3 R = reflect(-L, N);

		// Calculate the ambient, diffuse and specular contributions  
		vec3 ambient = ambientLight * vec3(object_colour);
		vec3 diffuse = max(dot(N, L), 0.0) * diffuseLight;
		vec3 specular = pow(max(dot(R, V), 0.0), specPower) * specLight;

		vs_out.lColour = ambient + diffuse + specular;
	}
	else {
		vs_out.lColour = vec3(object_colour);
	}

	gl_Position = frustum * worldPos;
}
