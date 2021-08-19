#include "Sun.h"

Sun::Sun(int sunTexture, int moonTexture) : _position(vec3(0.0f)), _voxel(Voxel(vec3(0.0f))), state(DAY), _sunTexture(sunTexture), _moonTexture(moonTexture) {
}

void Sun::draw(GLenum* renderingMode, ShaderManager* shaderProgram)
{
	vec3 lightDiff;
	int texture;
	vec3 lightColour;
	float lightScale;
	float skyLightScale;

	switch (state) {
	case DAWN:
		lightScale = progress / (WORLD_SIZE / 10);
		skyLightScale = lightScale * 0.5 + 0.5;
		lightDiff = LIGHT_COLOUR_DAY - LIGHT_COLOUR_NIGHT;
		lightColour = LIGHT_COLOUR_NIGHT + lightScale * lightDiff;
		texture = _sunTexture;
		glClearColor(worldSkyColours[0].x * skyLightScale, worldSkyColours[0].y * skyLightScale, worldSkyColours[0].z * skyLightScale, worldSkyColours[0].a);
		break;

	case DUSK:
		lightScale = 1 - ((progress - WORLD_SIZE / 2) / (WORLD_SIZE / 10));
		skyLightScale = lightScale * 0.5 + 0.5;
		lightDiff = LIGHT_COLOUR_DAY - LIGHT_COLOUR_NIGHT;
		lightColour = LIGHT_COLOUR_NIGHT + lightScale * lightDiff;
		texture = _sunTexture;
		glClearColor(worldSkyColours[0].x * skyLightScale, worldSkyColours[0].y * skyLightScale, worldSkyColours[0].z * skyLightScale, worldSkyColours[0].a);
		break;

	case DAY:
		texture = _sunTexture;
		lightColour = LIGHT_COLOUR_DAY;

		glClearColor(worldSkyColours[0].x, worldSkyColours[0].y, worldSkyColours[0].z, worldSkyColours[0].a);
		break;

	case NIGHT:
		texture = _moonTexture;
		lightColour = LIGHT_COLOUR_NIGHT;
		glClearColor(worldSkyColours[0].x * 0.5f, worldSkyColours[0].y * 0.5f, worldSkyColours[0].z * 0.5f, worldSkyColours[0].a);
		break;

	case NETHER:
		lightColour = LIGHT_COLOUR_NETHER;
		glClearColor(worldSkyColours[1].x, worldSkyColours[1].y, worldSkyColours[1].z, worldSkyColours[1].a);
		break;

	case END:
		lightColour = LIGHT_COLOUR_END;
		glClearColor(worldSkyColours[2].x, worldSkyColours[2].y, worldSkyColours[2].z, worldSkyColours[2].a);
		break;

	case NONE:
	default:
		cout << "Invalid sun state!";
	}

	shaderProgram->setVec3("lightPosition", _position);
	shaderProgram->setVec3("lightColour", lightColour);

	if (state == DAWN || state == DAY || state == DUSK || state == NIGHT) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		shaderProgram->setBool("ignoreLighting", true);
		shaderProgram->setVec3("colour", vec3(1.0f));
		shaderProgram->setInt("textureSampler", 0);
		_voxel.anchorMatrix = translate(mat4(1.0f), getSunPosition()) * scale(mat4(1.0f), vec3(10.0f));
		_voxel.draw(renderingMode, shaderProgram);
		shaderProgram->setBool("ignoreLighting", false);
	}
}

void Sun::update(vector<ScheduledEvent>* eventQueue, double dt)
{
	if (progress <= WORLD_SIZE / 10) {
		state = DAWN;
	} else if (progress <= (WORLD_SIZE / 10) * 5) {
		state = DAY;
	}
	else if (progress <= (WORLD_SIZE / 10) * 6) {
		state = DUSK;
	}
	else if (progress <= WORLD_SIZE) {
		state = NIGHT;
	} else if (progress <= WORLD_SIZE * 2) {
		state = NETHER;
	} else {
		state = END;
	}

	switch (state) {
	case DAWN:
	case DUSK:
	case DAY:
		_position = vec3(WORLD_SIZE / 4 - progress, cos(radians((progress / (WORLD_SIZE / 2) * 180.0f) - 90.0f)) * 30.0f + 25.0f, 0.0f);
		break;
	case NIGHT:
		_position = vec3(WORLD_SIZE / 4 - (progress - WORLD_SIZE / 2), cos(radians((progress - WORLD_SIZE / 2) / (WORLD_SIZE / 2) * 180.0f) - 90.0f) * 30.0f + 25.0f, 0.0f);
		break;
	case NETHER:
		_position = vec3(0.0f, -5.0f, 0.0f);
		break;
	case END:
		_position = vec3(0.0f, 10.0f, 0.0f);
		break;
	case NONE:
	default:
		cout << "Invalid sun state!";
	}
}

vec3 Sun::getLightPosition()
{
	return _position;
}

vec3 Sun::getSunPosition()
{
	return vec3(_position.x, _position.y, _position.z - SUN_Z_OFFSET);
}
