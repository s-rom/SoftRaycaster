#pragma once
#include "glm/vec3.hpp"
#include <vector>

enum LightType { POINT, AMBIENT, DIRECTIONAL };

struct light
{
	glm::vec3 origin;
	glm::vec3 direction;
	float intensity;
	LightType type;
};
