#version 410 core

in float Height;

out vec4 FragColor;

void main()
{
	float h = (Height + 16)/64.0f;
	FragColor = vec4(0.5, h, 0.7, 1.0);
}