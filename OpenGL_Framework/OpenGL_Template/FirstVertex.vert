#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform float xOffset;

void main()
{
	gl_Position = vec4(xOffset + aPos.x - 0.5, aPos.yz, 1.0);
	TexCoord = aTexCoord;
	vertexColor = vec4(aCol, 1.0);
}
