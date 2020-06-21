#pragma once

#include <cstdio>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"


#define print_vec3(vec) printf("[%.2f,%.2f,%.2f]",vec.x, vec.y, vec.z);
#define print_vec2(vec) printf("[%.2f,%.2f]",vec.x, vec.y);
#define print_vec3(msg, vec) printf("%s [%.2f,%.2f,%.2f]",msg, vec.x, vec.y, vec.z);
#define print_vec2(msg, vec) printf("%s [%.2f,%.2f]",msg, vec.x, vec.y);