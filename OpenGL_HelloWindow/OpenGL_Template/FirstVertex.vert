#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec4 vertexColor;

uniform float xOffset;

void main()
{
	gl_Position = vec4(xOffset + aPos.x - 0.5, aPos.yz, 1.0);
	vertexColor = vec4(aCol, 1.0);
}
