#include <iostream>

#include <SDL.h>

void drawChess(SDL_Renderer* r, int dimension);
void draw_pixel(SDL_Renderer * r, int x, int y, SDL_Color color);

int main(int argc, char ** argv)
{
	const int WIDTH = 600;
	const int HEIGHT = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow
	(
		"SoftRaycaster",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH,
		HEIGHT,
		0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer
	(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
	);



	do
	{
		SDL_Event e;

		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		
		drawChess(renderer, 10);
		SDL_RenderPresent(renderer);
	}
	while (true);

	return 0;
}

void draw_pixel(SDL_Renderer *r, int x, int y, SDL_Color color)
{
	SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(r, x, y);
}

void drawChess(SDL_Renderer * r, int dimension) 
{
	SDL_Rect viewport;
	SDL_RenderGetViewport(r, &viewport);
	float squareSide = (float)viewport.w / (float) dimension;

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++) 
		{
			SDL_Rect rect = 
			{
				j * squareSide,
				i * squareSide,
				squareSide,
				squareSide,
			};

			int red, green, blue;
			red = green = blue = (i % 2 == j % 2) * 255;

		
			SDL_SetRenderDrawColor(r, red, green, blue, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(r, &rect);
		}
	}

	SDL_RenderDrawPoint(r, 0, 0);
}