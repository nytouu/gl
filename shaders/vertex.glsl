#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform vec3 random;

void main()
{
    gl_Position = vec4(aPos + random / 10, 1.0);
	vertexColor = vec4(0.0, 0.0, 1.0, 1.0);
	TexCoord = aTexCoord;
}
