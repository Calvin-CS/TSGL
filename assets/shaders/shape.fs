#version 330 core
out vec4 FragColor;

in vec4 color;

// texture samplers
// uniform sampler2D texture1;

void main()
{
	FragColor = color;
}