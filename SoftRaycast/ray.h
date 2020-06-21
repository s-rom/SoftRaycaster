#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/exponential.hpp"
#include <vector>
#include "sphere.h"
#include "plane.h"
#include "geometry_scene.h"
#include <cstdio>

struct ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    float t_min, t_max;

    bool t_in_range_not_inclusive(float t)
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

    bool intersect_sphere(const sphere& s, glm::vec2 & intersection)
    {
        glm::vec3 oc = origin - s.center;
        float a = glm::dot(direction, direction);
        float b = glm::dot(oc, direction) * 2;
        float c = glm::dot(oc, oc) - glm::pow(s.radius, 2);

        float in_sqrt = glm::pow(b, 2) - 4 * a * c;

        if (in_sqrt > 0)
        {
            intersection.x = (-b + glm::sqrt(in_sqrt)) / (2 * a);
            intersection.y = (-b - glm::sqrt(in_sqrt)) / (2 * a);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool intersect_plane(const plane& plane, float & solution)
    {
        float n_dot_pO = glm::dot(plane.point - origin, plane.normal);
        float n_dot_dir = glm::dot(plane.normal, direction);

        if (n_dot_dir != 0) // 1 solution
        {
            solution = n_dot_pO / n_dot_dir;
            printf("Ray / Plane : %.2f\n", solution);
            return true;
        }
        else
        {
            return false;
        }
    }


    float compute_lighting(geometry_scene& scene, glm::vec3& p, glm::vec3 &normal)
    {
        glm::vec3 direction;
        float intensity = 0;

        for (light& l : scene.lights)
        {

            if (l.type == AMBIENT)
            {
                intensity += l.intensity;
            }
            else
            {
                if (l.type == POINT) direction = l.origin - p;
                else direction = l.direction;

                float n_dot_dir = glm::dot(normal, direction);
                if (n_dot_dir > 0) 
                {
                    float dot_mod = (glm::length(normal) * glm::length(direction));
                    intensity += l.intensity * (n_dot_dir / dot_mod);
                }
            }
        }

        return intensity;
    }

    glm::vec3 trace_scene(geometry_scene & scene, glm::vec3 & back_color)
    {
        float closest_t = t_max;
        sphere* closest_sphere = nullptr;
        glm::vec3* closest_color = nullptr;
        glm::vec3* selected_normal = nullptr;
        glm::vec3 aux;
        glm::vec2 sol;

        for (sphere& s : scene.spheres)
        {

            if (!intersect_sphere(s, sol)) continue;


            float t1 = sol.x;
            float t2 = sol.y;

            glm::vec3 p1 = get_point(t1);
            glm::vec3 p2 = get_point(t2);


            if (t_in_range_not_inclusive(t1) && t1 < closest_t)
            {
                closest_t = t1;
                closest_color = &s.color;
                closest_sphere = &s;
                aux = glm::normalize(p1 - s.center);
                selected_normal = &aux;
            }

            if (t_in_range_not_inclusive(t2) && t2 < closest_t)
            {
                closest_sphere = &s;
                closest_t = t2;
                closest_color = &s.color;
                aux = glm::normalize(p2 - s.center);
                selected_normal = &aux;
            }
        }


        if (closest_color != nullptr)
        {
            glm::vec3 point = get_point(closest_t);
            float intensity = compute_lighting(scene, point, *selected_normal);
            //printf("Intensity: %.2f\n", intensity);
            
            //return glm::normalize(point - closest_sphere->center) * 255.0f;
            return (*closest_color) * intensity;
        }
        return back_color;
    }
};


    