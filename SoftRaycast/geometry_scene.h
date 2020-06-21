#pragma once
#include <vector>
#include "sphere.h"
#include "plane.h"
#include "light.h"


struct geometry_scene
{
	std::vector<sphere> spheres;
	std::vector<plane> planes;
	std::vector<light> lights;
};