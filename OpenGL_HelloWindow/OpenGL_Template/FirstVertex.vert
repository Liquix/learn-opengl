#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 col;
// if x < 0 color = red
// if x == 0 color = green
// if x > 0 color = red
void main()
{
	gl_Position = vec4(aPos.xyz, 1.0);
	if(aPos.x < 0)	col = vec3(1.0, 0.0, 0.0);
	if(aPos.x == 0) col = vec3(0.0, 1.0, 0.0);
	if(aPos.x > 0)  col = vec3(0.0, 0.0, 1.0);
}
