#version 330 core

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 lightColour;
uniform float ambientLight;
uniform float diffuseLight;
uniform float specularLight;
uniform float shininess;
uniform sampler2D textureSampler;

in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 vertexUV;

out vec4 FragColor;
    
void main()
{
    vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPos);

	vec3 viewDirection = normalize(cameraPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);

	vec3 diffusion = max(dot(normal, lightDirection), 0.0) * lightColour * diffuseLight;

	vec3 specular = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * specularLight * lightColour;

	vec3 baseColour = vec3(vertexColor.r, vertexColor.g, vertexColor.b);
	vec3 lightTotal = (ambientLight + diffusion + specular) * baseColour;
	
	vec4 textureColor = texture( textureSampler, vertexUV );

    FragColor = vec4(lightTotal, 1.0f) * textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
}