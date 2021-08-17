#version 330 core
in vec2 TexCoords;
in vec4 ParticleColour;
out vec4 color;

uniform sampler2D sprite;

void main()
{
    color = (texture(sprite, TexCoords) * ParticleColour);
}  