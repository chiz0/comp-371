#version 330 core

uniform vec3 lightPos;

in vec3 vertexColor;
in vec3 Normal;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
}
