#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform vec3 random;

void main()
{
    gl_Position = vec4(aPos + random / 10, 1.0);
	vertexColor = vec4(0.0, 0.0, 1.0, 1.0);
}
