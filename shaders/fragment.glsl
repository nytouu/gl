#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform vec4 ourColor; // we set this variable in the OpenGL code.
uniform sampler2D ourTexture;

void main()
{
	vec4 finalColor = vec4(
		vertexColor.x + ourColor.x,
		vertexColor.y + ourColor.y,
		vertexColor.z + ourColor.z,
		1.0
	);
	FragColor = texture(ourTexture, TexCoord) + finalColor;
}
