#version 150

uniform mat4 EQ_modelMatrix;
uniform mat4 EQ_viewMatrix;
uniform mat4 EQ_projMatrix;

in vec4 position;
in vec3 color;

out vec3 Color;

void main()
{
    Color = color;
    gl_Position = ( EQ_projMatrix * EQ_viewMatrix * EQ_modelMatrix ) * position;
}
