#version 330 core

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 lightColour;
uniform float ambientLight;
uniform float diffuseLight;
uniform float specularLight;
uniform float shininess;
uniform bool ignoreLighting;
uniform sampler2D textureSampler;
uniform samplerCube depthMap;
uniform bool texToggle;


uniform mat4 orthProjection;
uniform mat4 camView;
uniform vec3 camPos;
uniform bool shadows;
uniform float farPlane;




in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 vertexUV;

out vec4 FragColor;



// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPosition;
    float currentDepth = length(fragToLight);
    
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(camPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
        
    return shadow;
} 


    
void main()
{
    vec3 normal = normalize(Normal);
	vec3 baseColour = vec3(vertexColor.r, vertexColor.g, vertexColor.b);
	vec3 viewDirection = normalize(cameraPosition - FragPos);
	vec3 lightTotal = vec3(1.0, 1.0, 1.0);

	float shadow = shadows ? ShadowCalculation(FragPos) : 0.0;

	if (ignoreLighting == false) {
		vec3 lightDirection = normalize(lightPosition - FragPos);

		vec3 reflectDirection = reflect(-lightDirection, normal);

		vec3 diffusion = max(dot(normal, lightDirection), 0.0) * lightColour * diffuseLight;

		vec3 specular = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * specularLight * lightColour;

		lightTotal = (ambientLight + (1.0 - shadow) * (diffusion + specular)) * baseColour;
	}

	
	

	vec4 textureColor = texture( textureSampler, vertexUV );
	if(texToggle==true)
	{
		FragColor = vec4(lightTotal, 1.0f) * textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
	}
	else
	{
		FragColor = vec4(lightTotal, 1.0f) * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
	}
}