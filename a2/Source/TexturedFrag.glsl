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
uniform bool texToggle;

uniform samplerCube depthMap;
uniform vec3 viewPos;
uniform float farPlane;
uniform bool shadows;

in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 vertexUV;

out vec4 FragColor;



float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPosition;

    // use the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;

    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= farPlane;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;  
	
    // display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / farPlane), 1.0);    
        
    return shadow;
}

    
void main()
{
    vec3 normal = normalize(Normal);
	vec3 baseColour = vec3(vertexColor.r, vertexColor.g, vertexColor.b);
	vec3 viewDirection = normalize(cameraPosition - FragPos);
	vec3 lightTotal = vec3(1.0, 1.0, 1.0);

	// calculate shadow
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