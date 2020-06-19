#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/exponential.hpp"
#include <vector>
#include "sphere.h"

struct ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    float t_min, t_max;

    bool t_in_range(float t)
    {
        return t <= t_max && t >= t_min;
    }

    glm::vec3 get_point(float t)
    {
        return origin + t * direction;
    }

    void intersect_sphere(const sphere& s, glm::vec2* intersection)
    {
        glm::vec3 oc = origin - s.center;
        float a = glm::dot(direction, direction);
        float b = glm::dot(oc, direction) * 2;
        float c = glm::dot(oc, oc) - glm::pow(s.radius, 2);

        float in_sqrt = glm::pow(b, 2) - 4 * a * c;

        if (in_sqrt > 0)
        {
            intersection->x = (-b + in_sqrt) / (2 * a);
            intersection->y = (-b - in_sqrt) / (2 * a);
        }
        else
        {
            intersection = nullptr;
        }

    }

    glm::vec3 trace_spheres(std::vector<sphere> spheres, glm::vec3 & back_color)
    {
        float closest_t = t_max;
        sphere* closest_sphere = nullptr;
        glm::vec2* i = &(glm::vec2());

        for (sphere& s : spheres)
        {

            intersect_sphere(s, i);
            if (i == nullptr) continue; 


            float t1 = i->x;
            float t2 = i->y;

            glm::vec3 p1 = get_point(t1);
            glm::vec3 p2 = get_point(t2);

            std::cout << "t1: " << t1 << ", P = [" << p1.x << ", "<<p1.y << ", " << p1.z << "]\n";
            std::cout << "t2: " << t2 << ", P = [" << p2.x << "," << p2.y << ", " << p2.z << "]\n";



            if (t_in_range(t1) && t1 < closest_t)
            {
                closest_t = t1;
                closest_sphere = &s;
            }

            if (t_in_range(t2) && t2 < closest_t)
            {
                closest_t = t2;
                closest_sphere = &s;
            }
        }

        if (closest_sphere != nullptr)
        {
            return closest_sphere->color;
            std::cout << "closest_sphere: " << closest_sphere->to_string() << "\n";
        }
        else
            return back_color;
    }


};


