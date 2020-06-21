#include <iostream>
#include <vector>
#include <SDL.h>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "geometry_scene.h"
#include "sphere.h"
#include "ray.h"
#include "util.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
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
void render_scene(const render_context& context, SDL_Renderer* renderer, geometry_scene& scene);

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



void render_scene(const render_context& context, SDL_Renderer* renderer, geometry_scene & scene)
{
	glm::vec3 back_color = { 255, 255, 255 };
	glm::vec3 origin = { 0, 0, 0 };
	glm::vec3 color = back_color;
	ray r;
	
    int half_w = context.screen_width/2;
    int half_h = context.screen_height/2;

	for (int cx = -half_w; cx < half_w; cx++)
	{
		for (int cy = -half_h; cy < half_h; cy++)
		{

			glm::vec3 viewport_point = canvas_to_viewport(cx, cy, context);
           
			r.origin = origin;
			r.direction = viewport_point - r.origin;
			r.t_min = 1;
			r.t_max = std::numeric_limits<float>::infinity();

			int sx = context.canvas_width / 2 + cx;
			int sy = context.canvas_height / 2 - cy - 1;

			if (sx < 0 || sx >= context.canvas_width ||
				sy < 0 || sy >= context.canvas_height) continue;


			color = r.trace_scene(scene, back_color);
			
			
			SDL_Color colr = { color.x, color.y, color.z };
     
            draw_pixel(renderer, sx, sy, colr);
		}
	}

}

void save_renderer_state_as_BMP(SDL_Renderer * renderer, const char * file_name)
{
	SDL_Surface* sshot = SDL_CreateRGBSurface
	(
		0, 
		CANVAS_WIDTH, 
		CANVAS_HEIGHT, 
		32, 
		0x00ff0000, 
		0x0000ff00, 
		0x000000ff, 
		0xff000000
	);
	
	SDL_RenderReadPixels
	(
		renderer, 
		NULL,
		SDL_PIXELFORMAT_ARGB8888, 
		sshot->pixels, 
		sshot->pitch
	);

	SDL_SaveBMP(sshot, file_name);
	SDL_FreeSurface(sshot);
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
	const float aspect_ratio = (float) CANVAS_WIDTH / (float) CANVAS_HEIGHT;
	context.viewport = { aspect_ratio , 1 };
	context.distance = 1;
	context.screen_width = SCREEN_WIDTH;
	context.screen_height = SCREEN_HEIGHT;

	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);


	geometry_scene scene;

	scene.spheres.push_back({ .center = {0, 0, 13}, .radius = 1, .color = {194, 14, 14} });
	scene.spheres.push_back({ .center = {-2, -0.5, 5}, .radius = 0.5, .color = {7, 168, 23} });
	scene.spheres.push_back({ .center = {1, -0.2, 2}, .radius = 0.2, .color = {49, 68, 235} });
	scene.spheres.push_back({ .center = {0.4, -0.4, 3}, .radius = 0.4, .color = {76, 50, 168} });
	//scene.planes.push_back({ .normal = {0,1,0}, .point = {0,-1,0}, .color = {255, 255, 0} });
	scene.spheres.push_back({ .center = {0, -5001, 0}, .radius = 5000, .color = {255, 255, 0} });


	/*
	scene.spheres.push_back({ .center = {0, -1, 3}, .radius = 1, .color = {255, 0, 0} });
	scene.spheres.push_back({ .center = {2, 0, 4}, .radius = 1, .color = {0, 0, 255} });
	scene.spheres.push_back({ .center = {-2, 0, 4}, .radius = 1, .color = {0, 255, 0} });
	*/

	scene.lights.push_back({ .intensity = 0.2, .type = AMBIENT });
	scene.lights.push_back({ .origin = {2, 1 , 0}, .intensity = 0.6, .type = POINT });
	scene.lights.push_back({ .direction = {1, 4, 4}, .intensity = 0.2, .type = DIRECTIONAL });

	
	render_scene(context, renderer, scene);
	std::cout << "Done rendering!\n";
	SDL_RenderPresent(renderer);
	save_renderer_state_as_BMP(renderer, "diffuse.bmp");



	/*do
	{
		SDL_Event e;

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				break;
		}
	} while (true);*/

	return 0;
}


