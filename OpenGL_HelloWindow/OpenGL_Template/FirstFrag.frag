#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D customTexture;
uniform sampler2D customTexture2;

void main()
{
	FragColor = mix(texture(customTexture, TexCoord), texture(customTexture2, TexCoord), 0.2);	// 0.5 is lerp factor. 0.0 is all first texture, 1.0 is all second texture, 0.5 is 50/50 blend.
}
