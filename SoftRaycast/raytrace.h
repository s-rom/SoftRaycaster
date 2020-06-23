#pragma once

#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/exponential.hpp"
#include <vector>
#include "sphere.h"
#include "plane.h"
#include "geometry_scene.h"
#include "ray.h"
#include <cstdio>

#define EPSILON 0.03

bool intersect_sphere(ray& r, const sphere& s, std::vector<float>& solutions)
{
    glm::vec3 oc = r.origin - s.center;
    float a = glm::dot(r.direction, r.direction);
    float b = glm::dot(oc, r.direction) * 2;
    float c = glm::dot(oc, oc) - glm::pow(s.radius, 2);

    float in_sqrt = glm::pow(b, 2) - 4 * a * c;

    if (in_sqrt > 0)
    {
        solutions.push_back((-b + glm::sqrt(in_sqrt)) / (2 * a));
        solutions.push_back((-b - glm::sqrt(in_sqrt)) / (2 * a));
        return true;
    }
    else
    {
        return false;
    }
}

bool intersect_plane(ray& r, const plane& plane, float& solution)
{
    float n_dot_pO = glm::dot(plane.point - r.origin, plane.normal);
    float n_dot_dir = glm::dot(plane.normal, r.direction);

    if (n_dot_dir != 0) // 1 solution
    {
        solution = n_dot_pO / n_dot_dir;
        return true;
    }
    else
    {
        return false;
    }
}


sphere* closest_sphere_intersection(ray& r, std::vector<sphere>& spheres, float& t)
{
    std::vector<float> solutions(2);
    sphere* closest = nullptr;
    t = std::numeric_limits<float>::max();

    for (sphere& s : spheres)
    {
        solutions.clear();
        if (!intersect_sphere(r, s, solutions)) continue;

        for (float sol : solutions)
        {
            glm::vec3 p = r.get_point(sol);
            if (r.t_in_range_exclusive(sol) && sol < t)
            {
                t = sol;
                closest = &s;
            }
        }
    }

    return closest;
}

glm::vec3 reflect(const glm::vec3 &L, const glm::vec3 &normal)
{
    return (2.0f * normal * glm::dot(normal, L)) - L;
}

float compute_lighting(geometry_scene & scene, glm::vec3& p, glm::vec3& view, glm::vec3& normal, int specular)
{
    glm::vec3 direction;
    float intensity = 0;

    for (light& l : scene.lights)
    {

        if (l.type == AMBIENT)
        {
            // Ambient light does not cause shadows
            intensity += l.intensity;
        }
        else
        {
            ray shadow_ray;
           
            if (l.type == POINT)
            {
                direction = l.origin - p;
                // for t = 1, P' = P + direction (P+direction == Plight)
                shadow_ray.t_max = 1;
            }
            else
            {
                direction = l.direction;
                // directional lights are always shadowed because they are
                // infinitely away
                shadow_ray.t_max = std::numeric_limits<float>::infinity();
            }

            // Compute if p is shadowed by any sphere
            shadow_ray.origin = p;
            shadow_ray.t_min = EPSILON;
            shadow_ray.direction = direction;
            float solution;
            sphere* closest_sphere = closest_sphere_intersection(shadow_ray, scene.spheres, solution);
            if (closest_sphere != nullptr)
                continue;


            // Diffuse
            float n_dot_dir = glm::dot(normal, direction);
            if (n_dot_dir > 0)
            {
                float dot_mod = (glm::length(normal) * glm::length(direction));
                intensity += l.intensity * (n_dot_dir / dot_mod);
            }

            // Specular
            if (specular != -1)
            {
                glm::vec3 R = reflect(direction, normal);
                float R_dot_view = glm::dot(R, view);
                if (R_dot_view > 0)
                {
                    float length_R_view = glm::length(R) * glm::length(view);
                    float cos_alpha = R_dot_view / length_R_view;
                    intensity += l.intensity * glm::pow(cos_alpha, specular);
                }
            }
        }
    }

    return intensity;
}



glm::vec3 trace_scene_recursive(ray& r, geometry_scene& scene, glm::vec3& back_color, int depth, int max_depth)
{
    float closest_t;
    sphere* closest_sphere = closest_sphere_intersection(r, scene.spheres, closest_t);

    if (closest_sphere == nullptr)
        return back_color;

    glm::vec3 point = r.get_point(closest_t);
    glm::vec3 normal = glm::normalize(point - closest_sphere->center);
    glm::vec3 view = -r.direction;

    float intensity = compute_lighting(scene, point, view, normal, closest_sphere->specular);
    glm::vec3 color = closest_sphere->color * (float) glm::clamp(intensity, 0.0f, 1.0f);

    float& refl = closest_sphere->reflective;
    if (depth >= max_depth || refl <= 0)
        return color;

    ray reflect_ray;
    reflect_ray.origin = point;
    reflect_ray.t_min = EPSILON;
    reflect_ray.t_max = std::numeric_limits<float>::infinity();
    reflect_ray.direction = reflect(view, normal);
    glm::vec3 reflected_color = trace_scene_recursive(reflect_ray, scene, back_color, depth + 1, max_depth);
    return (color * (1 - refl)) + (reflected_color * refl);
}

glm::vec3 trace_scene(ray& r, geometry_scene& scene, glm::vec3& back_color, int max_depth)
{
    return trace_scene_recursive(r, scene, back_color, 0, max_depth);
}
