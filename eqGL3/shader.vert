#version 150

uniform mat4 modelMatrix;

in vec4 position;
in vec3 color;
in vec4 rotation;

out vec3 Color;

void main()
{
    Color = color;
    gl_Position = (rotation * modelMatrix) * position;
}
