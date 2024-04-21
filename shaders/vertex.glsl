#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform vec3 random;
uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * transform * vec4(aPos + random / 10, 1.0);
	vertexColor = vec4(0.0, 0.0, 1.0, 1.0);
	TexCoord = aTexCoord;
}
