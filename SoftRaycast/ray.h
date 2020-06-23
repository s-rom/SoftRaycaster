#pragma once
#include "glm/vec3.hpp"


struct ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    float t_min, t_max;

    bool t_in_range_exclusive(float t)
    {
        return t < t_max && t > t_min;
    }

    bool t_in_range_inclusive(float t)
    {
        return t <= t_max && t >= t_min;
    }


    glm::vec3 get_point(float t)
    {
        return origin + t * direction;
    }
};


    