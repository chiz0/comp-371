#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "Coordinates.h"
#include "Shape.h"
#include "Model.h"



vector<Shape> shapes;
vector<Shape> owChunks[30];
vector<Model> mobs[30];

vector<ivec3> grassHillShape{
			{0,0,0},
			{ 8, 1, 0 },
			{ 9, 1, 0 },
			{ 10, 1, 0 },
			{ 11, 2, 0 },
			{ 12, 3, 0 },
			{ 13, 3, 0 },
			{ 14, 3, 0 },
			{ 15, 3, 0 },
			{ 16, 4, 0 },
			{ 17, 4, 0 },
			{ 18, 4, 0 },
			{ 19, 4, 0 },
			{ 20, 4, 0 },
			{ -4, 1, 0 },
			{ -5, 1, 0 },
			{ -6, 1, 0 },
			{ -7, 1, 0 },
			{ -8, 1, 0 },
			{ -9, 1, 0 },
			{ -10, 2, 0 },
			{ -11, 2, 0 },
			{ -12, 2, 0 },
			{ -13, 3, 0 },
			{ -14, 3, 0 },
			{ -15, 3, 0 },
			{ -16, 3, 0 },
			{ -17, 3, 0 },
			{ -18, 3, 0 },
			{ -19, 4, 0 },
			{ -20, 4, 0 },

			{ 7, 1, 1 },
			{ 8, 1, 1 },
			{ 9, 1, 1 },
			{ 10, 2, 1 },
			{ 11, 2, 1 },
			{ 12, 2, 1 },
			{ 13, 3, 1 },
			{ 14, 3, 1 },
			{ 15, 3, 1 },
			{ 16, 4, 1 },
			{ 17, 4, 1 },
			{ 18, 4, 1 },
			{ 19, 4, 1 },
			{ 20, 4, 1 },
			{ -5, 1, 1 },
			{ -6, 1, 1 },
			{ -7, 1, 1 },
			{ -8, 1, 1 },
			{ -9, 2, 1 },
			{ -10, 2, 1 },
			{ -11, 2, 1 },
			{ -12, 3, 1 },
			{ -13, 3, 1 },
			{ -14, 3, 1 },
			{ -15, 3, 1 },
			{ -16, 3, 1 },
			{ -17, 3, 1 },
			{ -18, 4, 1 },
			{ -19, 4, 1 },
			{ -20, 4, 1 },

			{ 8, 1, 2},
			{ 9, 1, 2},
			{ 10, 1, 2},
			{ 11, 2, 2},
			{ 12, 3, 2},
			{ 13, 3, 2},
			{ 14, 3, 2},
			{ 15, 3, 2},
			{ 16, 3, 2},
			{ 17, 4, 2},
			{ 18, 4, 2},
			{ 19, 4, 2},
			{ 20, 4, 2},
			{ -5, 1, 2},
			{ -6, 1, 2},
			{ -7, 1, 2},
			{ -8, 1, 2},
			{ -9, 2, 2},
			{ -10, 2, 2},
			{ -11, 2, 2},
			{ -12, 3, 2},
			{ -13, 3, 2},
			{ -14, 3, 2},
			{ -15, 3, 2},
			{ -16, 3, 2},
			{ -17, 4, 2},
			{ -18, 4, 2},
			{ -19, 4, 2},
			{ -20, 4, 2},

			{ 8, 1, 3},
			{ 9, 1, 3},
			{ 10, 1, 3},
			{ 11, 2, 3},
			{ 12, 3, 3},
			{ 13, 3, 3},
			{ 14, 3, 3},
			{ 15, 3, 3},
			{ 16, 3, 3},
			{ 17, 4, 3},
			{ 18, 4, 3},
			{ 19, 4, 3},
			{ 20, 4, 3},
			{ -6, 1, 3},
			{ -7, 1, 3},
			{ -8, 2, 3},
			{ -9, 2, 3},
			{ -10, 2, 3},
			{ -11, 3, 3},
			{ -12, 3, 3},
			{ -13, 3, 3},
			{ -14, 3, 3},
			{ -15, 3, 3},
			{ -16, 4, 3},
			{ -17, 4, 3},
			{ -18, 4, 3},
			{ -19, 4, 3},
			{ -20, 4, 3},

			{ 8, 1, 4 },
			{ 9, 1, 4 },
			{ 10, 1, 4 },
			{ 11, 2, 4 },
			{ 12, 3, 4 },
			{ 13, 3, 4 },
			{ 14, 3, 4 },
			{ 15, 3, 4 },
			{ 16, 3, 4 },
			{ 17, 3, 4 },
			{ 18, 4, 4 },
			{ 19, 4, 4 },
			{ 20, 4, 4 },
			{ -7, 1, 4 },
			{ -8, 2, 4 },
			{ -9, 2, 4 },
			{ -10, 2, 4 },
			{ -11, 3, 4 },
			{ -12, 3, 4 },
			{ -13, 3, 4 },
			{ -14, 3, 4 },
			{ -15, 3, 4 },
			{ -16, 4, 4 },
			{ -17, 4, 4 },
			{ -18, 4, 4 },
			{ -19, 4, 4 },
			{ -20, 4, 4 },

			{ 7, 1, 5 },
			{ 8, 1, 5 },
			{ 9, 1, 5 },
			{ 10, 2, 5 },
			{ 11, 2, 5 },
			{ 12, 2, 5 },
			{ 13, 3, 5 },
			{ 14, 3, 5 },
			{ 15, 3, 5 },
			{ 16, 3, 5 },
			{ 17, 3, 5 },
			{ 18, 3, 5 },
			{ 19, 4, 5 },
			{ 20, 4, 5 },
			{ -5, 1, 5 },
			{ -6, 1, 5 },
			{ -7, 1, 5 },
			{ -8, 2, 5 },
			{ -9, 2, 5 },
			{ -10, 2, 5 },
			{ -11, 3, 5 },
			{ -12, 3, 5 },
			{ -13, 3, 5 },
			{ -14, 3, 5 },
			{ -15, 4, 5 },
			{ -16, 4, 5 },
			{ -17, 4, 5 },
			{ -18, 4, 5 },
			{ -19, 4, 5 },
			{ -20, 4, 5 },

			{ 6, 1, 6 },
			{ 7, 1, 6 },
			{ 8, 1, 6 },
			{ 9, 1, 6 },
			{ 10, 1, 6 },
			{ 11, 2, 6 },
			{ 12, 3, 6 },
			{ 13, 3, 6 },
			{ 14, 3, 6 },
			{ 15, 3, 6 },
			{ 16, 3, 6 },
			{ 17, 3, 6 },
			{ 18, 3, 6 },
			{ 19, 4, 6 },
			{ 20, 4, 6 },
			{ -6, 1, 6},
			{ -7, 2, 6},
			{ -8, 2, 6},
			{ -9, 2, 6},
			{ -10, 2, 6},
			{ -11, 2, 6},
			{ -12, 3, 6},
			{ -13, 3, 6},
			{ -14, 3, 6},
			{ -15, 4, 6},
			{ -16, 4, 6},
			{ -17, 4, 6},
			{ -18, 4, 6},
			{ -19, 4, 6},
			{ -20, 4, 6},

			{ 7, 1, 7 },
			{ 8, 1, 7 },
			{ 9, 1, 7 },
			{ 10, 1, 7 },
			{ 11, 2, 7 },
			{ 12, 2, 7 },
			{ 13, 3, 7 },
			{ 14, 3, 7 },
			{ 15, 3, 7 },
			{ 16, 3, 7 },
			{ 17, 3, 7 },
			{ 18, 3, 7 },
			{ 19, 3, 7 },
			{ 20, 4, 7 },
			{ -7, 1, 7},
			{ -8, 1, 7},
			{ -9, 2, 7},
			{ -10, 2, 7},
			{ -11, 2, 7},
			{ -12, 3, 7},
			{ -13, 3, 7},
			{ -14, 3, 7},
			{ -15, 4, 7},
			{ -16, 4, 7},
			{ -17, 4, 7},
			{ -18, 4, 7},
			{ -19, 4, 7},
			{ -20, 4, 7},

			{ 6, 1, 8 },
			{ 7, 1, 8 },
			{ 8, 1, 8 },
			{ 9, 1, 8 },
			{ 10, 1, 8 },
			{ 11, 2, 8 },
			{ 12, 2, 8 },
			{ 13, 2, 8 },
			{ 14, 3, 8 },
			{ 15, 3, 8 },
			{ 16, 3, 8 },
			{ 17, 3, 8 },
			{ 18, 3, 8 },
			{ 19, 4, 8 },
			{ 20, 4, 8 },
			{ -6, 1, 8},
			{ -7, 1, 8},
			{ -8, 1, 8},
			{ -9, 1, 8},
			{ -10, 2, 8},
			{ -11, 2, 8},
			{ -12, 2, 8},
			{ -13, 3, 8},
			{ -14, 3, 8},
			{ -15, 4, 8},
			{ -16, 4, 8},
			{ -17, 4, 8},
			{ -18, 4, 8},
			{ -19, 4, 8},
			{ -20, 4, 8},

			{ 5, 1, 9 },
			{ 6, 1, 9 },
			{ 7, 1, 9 },
			{ 8, 1, 9 },
			{ 9, 1, 9 },
			{ 10, 2, 9 },
			{ 11, 2, 9 },
			{ 12, 2, 9 },
			{ 13, 2, 9 },
			{ 14, 3, 9 },
			{ 15, 3, 9 },
			{ 16, 3, 9 },
			{ 17, 3, 9 },
			{ 18, 4, 9 },
			{ 19, 4, 9 },
			{ 20, 4, 9 },
			{ -7, 1, 9},
			{ -8, 1, 9},
			{ -9, 1, 9},
			{ -10, 2, 9},
			{ -11, 2, 9},
			{ -12, 3, 9},
			{ -13, 3, 9},
			{ -14, 3, 9},
			{ -15, 3, 9},
			{ -16, 4, 9},
			{ -17, 4, 9},
			{ -18, 4, 9},
			{ -19, 4, 9},
			{ -20, 4, 9},

			{ 4, 1, 10 },
			{ 5, 1, 10 },
			{ 6, 1, 10 },
			{ 7, 1, 10 },
			{ 8, 1, 10 },
			{ 9, 1, 10 },
			{ 10, 2, 10 },
			{ 11, 2, 10 },
			{ 12, 2, 10 },
			{ 13, 3, 10 },
			{ 14, 3, 10 },
			{ 15, 3, 10 },
			{ 16, 3, 10 },
			{ 17, 3, 10 },
			{ 18, 3, 10 },
			{ 19, 4, 10 },
			{ 20, 4, 10 },
			{ -8, 1, 10},
			{ -9, 1, 10},
			{ -10, 1, 10},
			{ -11, 2, 10},
			{ -12, 3, 10},
			{ -13, 3, 10},
			{ -14, 3, 10},
			{ -15, 3, 10},
			{ -16, 4, 10},
			{ -17, 4, 10},
			{ -18, 4, 10},
			{ -19, 4, 10},
			{ -20, 4, 10},

			{ 5, 1, 11 },
			{ 6, 1, 11 },
			{ 7, 1, 11 },
			{ 8, 1, 11 },
			{ 9, 2, 11 },
			{ 10, 2, 11 },
			{ 11, 2, 11 },
			{ 12, 3, 11 },
			{ 13, 3, 11 },
			{ 14, 3, 11 },
			{ 15, 3, 11 },
			{ 16, 3, 11 },
			{ 17, 3, 11 },
			{ 18, 4, 11 },
			{ 19, 4, 11 },
			{ 20, 4, 11 },
			{ -7, 1, 11},
			{ -8, 1, 11},
			{ -9, 1, 11},
			{ -10, 2, 11},
			{ -11, 2, 11},
			{ -12, 2, 11},
			{ -13, 3, 11},
			{ -14, 3, 11},
			{ -15, 3, 11},
			{ -16, 4, 11},
			{ -17, 4, 11},
			{ -18, 4, 11},
			{ -19, 4, 11},
			{ -20, 4, 11},

			
			{ 5, 1, 12 },
			{ 6, 1, 12 },
			{ 7, 1, 12 },
			{ 8, 1, 12 },
			{ 9, 2, 12 },
			{ 10, 2, 12 },
			{ 11, 2, 12 },
			{ 12, 3, 12 },
			{ 13, 3, 12 },
			{ 14, 3, 12 },
			{ 15, 3, 12 },
			{ 16, 3, 12 },
			{ 17, 4, 12 },
			{ 18, 4, 12 },
			{ 19, 4, 12 },
			{ 20, 4, 12 },
			{ -8, 1, 12},
			{ -9, 1, 12},
			{ -10, 1, 12},
			{ -11, 2, 12},
			{ -12, 3, 12},
			{ -13, 3, 12},
			{ -14, 3, 12},
			{ -15, 3, 12},
			{ -16, 3, 12},
			{ -17, 4, 12},
			{ -18, 4, 12},
			{ -19, 4, 12},
			{ -20, 4, 12},

			{ 6, 1, 13 },
			{ 7, 1, 13 },
			{ 8, 2, 13 },
			{ 9, 2, 13 },
			{ 10, 2, 13 },
			{ 11, 3, 13 },
			{ 12, 3, 13 },
			{ 13, 3, 13 },
			{ 14, 3, 13 },
			{ 15, 3, 13 },
			{ 16, 4, 13 },
			{ 17, 4, 13 },
			{ 18, 4, 13 },
			{ 19, 4, 13 },
			{ 20, 4, 13 },
			{ -8, 1, 13},
			{ -9, 1, 13},
			{ -10, 1, 13},
			{ -11, 2, 13},
			{ -12, 3, 13},
			{ -13, 3, 13},
			{ -14, 3, 13},
			{ -15, 3, 13},
			{ -16, 3, 13},
			{ -17, 4, 13},
			{ -18, 4, 13},
			{ -19, 4, 13},
			{ -20, 4, 13},

			{ 7, 1, 14 },
			{ 8, 2, 14 },
			{ 9, 2, 14 },
			{ 10, 2, 14 },
			{ 11, 3, 14 },
			{ 12, 3, 14 },
			{ 13, 3, 14 },
			{ 14, 3, 14 },
			{ 15, 3, 14 },
			{ 16, 4, 14 },
			{ 17, 4, 14 },
			{ 18, 4, 14 },
			{ 19, 4, 14 },
			{ 20, 4, 14 },
			{ -8, 1, 14},
			{ -9, 1, 14},
			{ -10, 1, 14},
			{ -11, 2, 14},
			{ -12, 3, 14},
			{ -13, 3, 14},
			{ -14, 3, 14},
			{ -15, 3, 14},
			{ -16, 3, 14},
			{ -17, 3, 14},
			{ -18, 4, 14},
			{ -19, 4, 14},
			{ -20, 4, 14},

			{ 5, 1, 15 },
			{ 6, 1, 15 },
			{ 7, 1, 15 },
			{ 8, 2, 15 },
			{ 9, 2, 15 },
			{ 10, 2, 15 },
			{ 11, 3, 15 },
			{ 12, 3, 15 },
			{ 13, 3, 15 },
			{ 14, 3, 15 },
			{ 15, 4, 15 },
			{ 16, 4, 15 },
			{ 17, 4, 15 },
			{ 18, 4, 15 },
			{ 19, 4, 15 },
			{ 20, 4, 15 },
			{ -7, 1, 15},
			{ -8, 1, 15},
			{ -9, 1, 15},
			{ -10, 2, 15},
			{ -11, 2, 15},
			{ -12, 2, 15},
			{ -13, 3, 15},
			{ -14, 3, 15},
			{ -15, 3, 15},
			{ -16, 3, 15},
			{ -17, 3, 15},
			{ -18, 3, 15},
			{ -19, 4, 15},
			{ -20, 4, 15},

			{ 7, 1, 16},
			{ 8, 2, 16},
			{ 9, 2, 16},
			{ 10, 2, 16},
			{ 11, 3, 16},
			{ 12, 3, 16},
			{ 13, 3, 16},
			{ 14, 3, 16},
			{ 15, 3, 16},
			{ 16, 4, 16},
			{ 17, 4, 16},
			{ 18, 4, 16},
			{ 19, 4, 16},
			{ 20, 4, 16},
			{ -8, 1, 16},
			{ -9, 1, 16},
			{ -10, 1, 16},
			{ -11, 2, 16},
			{ -12, 3, 16},
			{ -13, 3, 16},
			{ -14, 3, 16},
			{ -15, 3, 16},
			{ -16, 3, 16},
			{ -17, 3, 16},
			{ -18, 4, 16},
			{ -19, 4, 16},
			{ -20, 4, 16},

			{ 7, 1, 17 },
			{ 8, 1, 17 },
			{ 9, 2, 17 },
			{ 10, 2, 17 },
			{ 11, 2, 17 },
			{ 12, 3, 17 },
			{ 13, 3, 17 },
			{ 14, 3, 17 },
			{ 15, 4, 17 },
			{ 16, 4, 17 },
			{ 17, 4, 17 },
			{ 18, 4, 17 },
			{ 19, 4, 17 },
			{ 20, 4, 17 },
			{ -7, 1, 17 },
			{ -8, 1, 17 },
			{ -9, 1, 17 },
			{ -10, 1, 17 },
			{ -11, 2, 17 },
			{ -12, 2, 17 },
			{ -13, 3, 17 },
			{ -14, 3, 17 },
			{ -15, 3, 17 },
			{ -16, 3, 17 },
			{ -17, 3, 17 },
			{ -18, 3, 17 },
			{ -19, 3, 17 },
			{ -20, 4, 17 },

			{ 6, 1, 18 },
			{ 7, 1, 18 },
			{ 8, 1, 18 },
			{ 9, 1, 18 },
			{ 10, 2, 18 },
			{ 11, 2, 18 },
			{ 12, 2, 18 },
			{ 13, 3, 18 },
			{ 14, 3, 18 },
			{ 15, 4, 18 },
			{ 16, 4, 18 },
			{ 17, 4, 18 },
			{ 18, 4, 18 },
			{ 19, 4, 18 },
			{ 20, 4, 18 },
			{ -6, 1, 18 },
			{ -7, 1, 18 },
			{ -8, 1, 18 },
			{ -9, 1, 18 },
			{ -10, 1, 18 },
			{ -11, 2, 18 },
			{ -12, 2, 18 },
			{ -13, 2, 18 },
			{ -14, 3, 18 },
			{ -15, 3, 18 },
			{ -16, 3, 18 },
			{ -17, 3, 18 },
			{ -18, 3, 18 },
			{ -19, 4, 18 },
			{ -20, 4, 18 },

			{ 7, 1, 19 },
			{ 8, 1, 19 },
			{ 9, 1, 19 },
			{ 10, 2, 19 },
			{ 11, 2, 19 },
			{ 12, 3, 19 },
			{ 13, 3, 19 },
			{ 14, 3, 19 },
			{ 15, 3, 19 },
			{ 16, 4, 19 },
			{ 17, 4, 19 },
			{ 18, 4, 19 },
			{ 19, 4, 19 },
			{ 20, 4, 19 },
			{ -5, 1, 19 },
			{ -6, 1, 19 },
			{ -7, 1, 19 },
			{ -8, 1, 19 },
			{ -9, 1, 19 },
			{ -10, 2, 19 },
			{ -11, 2, 19 },
			{ -12, 2, 19 },
			{ -13, 2, 19 },
			{ -14, 3, 19 },
			{ -15, 3, 19 },
			{ -16, 3, 19 },
			{ -17, 3, 19 },
			{ -18, 4, 19 },
			{ -19, 4, 19 },
			{ -20, 4, 19 }

};

vector<ivec3> treeTrunk{
	{ 0, 0, 0 },
	{ 0, 1, 0 },
	{ 0, 2, 0 },
	{ 0, 3, 0 },
	{ 0, 4, 0 }
};

vector <ivec3> treeLeaves{
	{ 0, 0, 0 },
	{ 1, 0, 0 },
	{ -1, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 0, -1 },
	{ 1, 0, 1 },
	{ 1, 0, -1 },
	{ -1, 0, 1 },
	{ -1, 0, -1 },
	{ 2, 0, 1 },
	{ 2, 0, 0 },
	{ 2, 0, -1 },
	{ -2, 0, 1 },
	{ -2, 0, 0 },
	{ -2, 0, -1 },
	{ 1, 0, 2 },
	{ 0, 0, 2 },
	{ -1, 0, 2 },
	{ 1, 0, -2 },
	{ 0, 0, -2 },
	{ -1, 0, -2 },

	{ 1, -1, 0 },
	{ -1, -1, 0 },
	{ 0, -1, 1 },
	{ 0, -1, -1 },
	{ 1, -1, 1 },
	{ 1, -1, -1 },
	{ -1, -1, 1 },
	{ -1, -1, -1 },
	{ 2, -1, 1 },
	{ 2, -1, 0 },
	{ 2, -1, -1 },
	{ -2, -1, 1 },
	{ -2, -1, 0 },
	{ -2, -1, -1 },
	{ 1, -1, 2 },
	{ 0, -1, 2 },
	{ -1, -1, 2 },
	{ 1, -1, -2 },
	{ 0, -1, -2 },
	{ -1, -1, -2 },

	{ 0, 1,  0 },
	{ 1, 1,  0 },
	{ -1, 1,  0 },
	{ 0, 1,  1 },
	{ 0, 1,  -1 },
	{ 1, 1,  1 },
	{ 1, 1,  -1 },
	{ -1, 1,  1 },
	{ -1, 1,  -1 },

	{ 0, 2,  0 },
	{ 1, 2,  0 },
	{ -1, 2,  0 },
	{ 0, 2,  1 },
	{ 0, 2,  -1 }
};

vector<ivec3> rockFormation{
	{ 0, 0, 0 },
	{ 0, 1, 0 },
	{ 0, 2, 0 },
	{ -1, 0, 0 },
	{ -1, 1, 0 },
	{ 0, 0, 1 },
	{ 0, 0, -1 },
	{ 1, 0, 0 },
};

vector<ivec3> cutPlanks{
	{ 0, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 0, -1 },
};

vector<ivec3> cave {
	{ 0, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 0, 2 },
	{ 1, 0, 0 },
	{ 2, 0, 0 },
	{ 3, 0, 0 },
	{ 1, 0, 1 },
	{ 1, 0, 2 },
	{ 1, 0, 3 },
	{ 2, 0, 1 },
	{ 2, 0, 2 },
	{ 2, 0, 3 },
	{ 3, 0, 1 },
	{ 3, 0, 2 },
	{ 1, 0, -1 },
	{ 0, 0, 4 },
	{ 1, 1, -2 },
	{ 1, 2, -2 },
	{ 1, 3, -1 },
	{ -1, 0, 3 },
	{ -1, 1, 2 },
	{ -1, 2, 2 },
	{ -1, 3, 1 },
	{ 0, 1, 3 },
	{ 0, 2, 3 },
	{ 0, 3, 2 },
	{ 0, 4, 1 },
	{ 0, 4, 0 },
	{ 1, 1, 4 },
	{ 1, 2, 4 },
	{ 1, 3, 3 },
	{ 1, 3, 3 },
	{ 1, 4, 3 },
	{ 1, 4, 2 },
	{ 1, 4, 1 },
	{ 1, 4, 0 },
	{ 2, 1, -2 },
	{ 2, 2, -1 },
	{ 2, 3, 0 },
	{ 2, 4, 1 },
	{ 2, 3, 2 },
	{ 2, 2, 3 },
	{ 2, 1, 4 },
	{ 3, 1, -1 },
	{ 3, 2, 0 },
	{ 3, 3, 1 },
	{ 3, 2, 2 },
	{ 3, 1, 3 },
	{ 4, 1, 0 },
	{ 4, 2, 1 },
	{ 4, 1, 2 },
}; 

vector<ivec3> generatePortal() {
	vector<ivec3> portal;
	for (int i = 0; i < 9; i++) {
		portal.push_back({ i,0,0 });
		portal.push_back({ i,8,0 });
		portal.push_back({ 0,i,0 });
		portal.push_back({ 8,i,0 });
	}
	return portal;
};

vector<ivec3> generateNetherTemple(Stage* stage) {
	vector<ivec3> netherTemple;
	netherTemple.push_back({ 0, 0, 0 });

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-14.0f, 4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 14));
	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-14.0f, 4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 15));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(14.0f, 4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 14));
	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(14.0f, 4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 15));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-14.0f, 15.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 14));
	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-14.0f, 15.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 15));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(14.0f, 15.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 14));
	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(14.0f, 15.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 0.0f, 19.0f)), 15));


	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(19.5f, -4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(0.0f, 10.0f, 19.0f)), 14));
	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(19.5f, -4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(0.0f, 10.0f, 19.0f)), 15));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-19.5f, -4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(0.0f, 10.0f, 19.0f)), 14));
	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-19.5f, -4.5f, 299.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(0.0f, 10.0f, 19.0f)), 15));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(14.0f, -5.5f, 279.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 10.0f, 0.0f)), 14));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-14.0f, -5.5f, 279.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 10.0f, 0.0f)), 14));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(14.0f, -5.5f, 318.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 10.0f, 0.0f)), 15));

	stage->attachModel(Model(MODEL_PATH_BLOCK,
		glm::translate(mat4(1.0f), vec3(-14.0f, -5.5f, 318.5f)) *                     //Position
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
		glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
		glm::scale(mat4(1.0f), vec3(6.5f, 10.0f, 0.0f)), 15));


	for (int i = 0; i < 15; i++) {
		netherTemple.push_back({ 7, i, 0 });
		netherTemple.push_back({ -7, i, 0 });
		netherTemple.push_back({ 7, i, 39 });
		netherTemple.push_back({ -7, i, 39 });
	}
	for (int i = 7; i < 21; i++) {
		netherTemple.push_back({ i, 14, 0 });
		netherTemple.push_back({ -i, 14, 0 });
		netherTemple.push_back({ i, 14, 39 });
		netherTemple.push_back({ -i, 14, 39 });
		if (i % 2 == 1) {
			netherTemple.push_back({ i, 15, 0 });
			netherTemple.push_back({ -i, 15, 0 });
			netherTemple.push_back({ i, 15, 39 });
			netherTemple.push_back({ -i, 15, 39 });
		}

		if (i % 6 == 3) {
			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(3 + i, 15.5f, 281.0f)) *                     //Position
				glm::rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 14));
			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(-3 - i, 15.5f, 281.0f)) *                     //Position
				glm::rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 14));

			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(-i, 15.5f, 318.0f)) *                     //Position
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 15));
			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(i, 15.5f, 318.0f)) *                     //Position
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 15));
		}
	}

	for (int i = 1; i < 39; i++) {
		netherTemple.push_back({ 7, 14, i });
		netherTemple.push_back({ -7, 14, i });
		netherTemple.push_back({ 7, 3, i });
		netherTemple.push_back({ -7, 3, i });

		if (i % 2 == 0) {
			netherTemple.push_back({ 7, 15, i });
			netherTemple.push_back({ -7, 15, i });
			netherTemple.push_back({ 7, 4, i });
			netherTemple.push_back({ -7, 4, i });
		}

		if (i % 6 == 3) {

			stage->attachModel(Model(MODEL_PATH_BLAZE,
				glm::translate(mat4(1.0f), vec3(-12.0f, 8.5f, 280.5f + i)) *                     //Position
				glm::rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(5)), 15));
			stage->attachModel(Model(MODEL_PATH_BLAZE,
				glm::translate(mat4(1.0f), vec3(12.0f, 8.5f, 280.5f + i)) *                     //Position
				glm::rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(5)), 15));
		}

		if (i % 10 == 1) {

			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(-10.0f, 15.5f, 281.5f + i)) *                     //Position
				glm::rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 15));
			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(8.0f, 15.5f, 281.5f + i)) *                     //Position
				glm::rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 15));


			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(-10.0f, 4.5f, 281.5f + i)) *                     //Position
				glm::rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 15));
			stage->attachModel(Model(MODEL_PATH_WITHERSKELETON,
				glm::translate(mat4(1.0f), vec3(8.0f, 4.5f, 281.5f + i)) *                     //Position
				glm::rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 1.0f, 0.0f)) *          //Orientation
				glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
				glm::scale(mat4(1.0f), vec3(0.5f)), 15));
		}
	}
	return netherTemple;
}