#version 330 core

uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform float ambientLight;
uniform float diffuseLight;

in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPos);

	vec3 diffusion = max(dot(normal, lightDirection), 0.0) * lightColour * diffuseLight;
	vec3 baseColour = vec3(vertexColor.r, vertexColor.g, vertexColor.b);
	vec3 lightTotal = (ambientLight + diffusion) * baseColour;
	
	FragColor = vec4(lightTotal, 1.0f);
}
