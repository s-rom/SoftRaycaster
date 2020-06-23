#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

struct camera
{
	glm::vec3 origin;
	glm::vec3 orientation;
};