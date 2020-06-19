#include <iostream>
#include <vector>
#include <SDL.h>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "sphere.h"
#include "ray.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CANVAS_WIDTH = SCREEN_WIDTH;
const int CANVAS_HEIGHT = SCREEN_HEIGHT;

struct render_context 
{
	int screen_width;
	int screen_height;
	int canvas_width;
	int canvas_height;
	glm::vec2 viewport;
	float distance;
} context;

void draw_pixel(SDL_Renderer * r, int x, int y, SDL_Color color);
void render_spheres(const render_context& context, SDL_Renderer* renderer, std::vector<sphere> spheres);

void draw_pixel(SDL_Renderer *r, int x, int y, SDL_Color color)
{
	SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(r, x, y);
}

glm::vec3 canvas_to_viewport(float cx, float cy, const render_context & context)
{
	return glm::vec3
	{
		cx * (context.viewport.x / CANVAS_WIDTH),
		cy * (context.viewport.y / CANVAS_HEIGHT),
		context.distance
	};
}



void render_spheres(const render_context& context, SDL_Renderer* renderer, std::vector<sphere> spheres)
{
	glm::vec3 back_color = { 0, 0, 0 };
	glm::vec3 origin = { 0, 0, 0 };
	glm::vec3 color = back_color;

	for (int sx = 0; sx < context.screen_width; sx++)
	{
		for (int sy = 0; sy < context.screen_height; sy++)
		{
			float cx = sx - context.screen_width / 2;
			float cy = context.screen_height / 2 - sy;

			glm::vec3 viewport_point = canvas_to_viewport(cx, cy, context);


			ray r;
			r.origin = origin;
			r.direction = glm::normalize(viewport_point - r.origin);
			r.t_min = context.distance;
			r.t_max = std::numeric_limits<float>::infinity();

			std::cout << "ray: [" << origin.x << ", " << origin.y << ", "<< origin.z <<
				"] dir: [" << r.direction.x << ", " << r.direction.y <<", "<<r.direction.z << "] \n";


			color = r.trace_spheres(spheres, back_color);

		

			if (color.x != 0)
				std::cout << "color: " << color.x << "\n";

			SDL_Color colr = { color.x, color.y, color.z };
			draw_pixel(renderer, sx, sy, colr);
		}
	}

}



int main(int argc, char** argv)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow
	(
		"SoftRaycaster",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer
	(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | 
        SDL_RENDERER_PRESENTVSYNC | 
        SDL_RENDERER_TARGETTEXTURE
	);


	context.canvas_width = CANVAS_WIDTH;
	context.canvas_height = CANVAS_HEIGHT;
	context.distance = 1;
	context.viewport = { 1, 1 };
	context.screen_width = SCREEN_WIDTH;
	context.screen_height = SCREEN_HEIGHT;


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	

	std::vector<sphere> spheres;
	spheres.push_back({ .center = {0, -1, 3}, .radius = 1, .color = {255,0,0} });

	render_spheres(context, renderer, spheres);
	std::cout << "Done rendering!\n";

	SDL_RenderPresent(renderer);


	do
	{
		SDL_Event e;

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				break;
		}
	} while (true);

	return 0;
}


