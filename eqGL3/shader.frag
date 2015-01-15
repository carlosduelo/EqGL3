#version 150

in vec3 Color;
out vec4 outputF;

void main()
{
    outputF = vec4(Color, 1.0) + vec4(0.0, 0.0, 1.0, 0.0);

}
