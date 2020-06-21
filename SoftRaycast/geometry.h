#pragma once
#include "glm/vec3.hpp"
#include "ray.h"
#include <vector>
#include <string>

struct geometry 
{
	glm::vec3 color;
	virtual void ray_intersection(ray r, std::vector<float> & solutions) = 0;
	virtual std::string to_string() = 0;
};