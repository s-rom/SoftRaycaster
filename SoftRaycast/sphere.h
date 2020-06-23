#pragma once
#include "glm/vec3.hpp"
#include <string>
#include <sstream>

struct sphere
{
	glm::vec3 center;
	float radius;
	glm::vec3 color;
	int specular;
	float reflective;

	std::string to_string()
	{
		std::stringstream stream;
		stream << "Center: [" << center.x <<
			"," << center.y <<
			"," << center.z <<
			"]  r = " << radius << ", color = [" << 
			color.x << ", "<< 
			color.y << ", "<<
			color.z <<"]";
		return stream.str();
	}

};