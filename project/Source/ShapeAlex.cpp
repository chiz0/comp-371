#include "Shape.h"
#include "stb_image.h"

class ShapeAlex : public Shape {
public:
	void DrawGlow() {
		unsigned char* textureData = stbi_load(filepath, &width, &height, &channels, 0);
	}

private:
	int width, height, channels;
	char* filepath = "../Assets/Textures/brick.jpg";
};