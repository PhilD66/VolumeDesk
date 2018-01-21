#version 410 core

layout (location = 0) out vec4 colour;

in VS_OUT
{
    vec3 lColour;
} fs_in;

void main(void)
{
    colour = vec4(fs_in.lColour, 1.0);
}

