#pragma once

#include <vector>
#include "Shape.h"

struct ControlState {
	std::vector<Shape>* shapes;
	int* focusedShape;
	bool* showTexture;
};
