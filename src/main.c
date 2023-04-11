#include <SDL2/SDL.h>
#include <SDL_pixels.h>
#include <SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define FORMAT SDL_PIXELFORMAT_RGB888

enum Error {
	OK = 0,
	ERROR
};

void animate_rainbow(Uint32 *pixels);

int world_map[MAP_WIDTH][MAP_HEIGHT] = {
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,2,0,0,0,0,2,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,3,3,0,0,0,1},
	{1,0,0,0,3,3,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,2,0,0,0,0,2,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1},
};

struct {
	SDL_Window *window;
	SDL_Surface *backbuffer;
	Uint32 *pixels;
	Uint64 tick;
} context;

int engine_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	memset(&context, 0, sizeof(context));
	context.window
		= SDL_CreateWindow("My SDL2 App",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   SCREEN_WIDTH, SCREEN_HEIGHT,
				   SDL_WINDOW_SHOWN);
	context.backbuffer
		= SDL_CreateRGBSurfaceWithFormat(0,
						 SCREEN_WIDTH,
						 SCREEN_HEIGHT,
						 32,
						 FORMAT);
	context.pixels = context.backbuffer->pixels;
	if (!context.window) {
		printf("Window could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	if (!context.backbuffer) {
		printf("Backbuffer could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	return OK;
}

void engine_quit()
{
	SDL_FreeSurface(context.backbuffer);
	SDL_DestroyWindow(context.window);
	SDL_Quit();
}

void engine_render()
{
	SDL_LockSurface(context.backbuffer);
	animate_rainbow(context.pixels);
	SDL_UnlockSurface(context.backbuffer);
	SDL_BlitSurface(context.backbuffer, NULL,
			SDL_GetWindowSurface(context.window), NULL);
	SDL_UpdateWindowSurface(context.window);
}

int main(void)
{
	engine_init();
	for (SDL_Event event; event.type != SDL_QUIT; SDL_PollEvent(&event)) {
		context.tick++;
		engine_render();
	}
	engine_quit();
}

void animate_rainbow(Uint32 *pixels)
{
#define ANIMATION_SPEED -0.003f
	for (int h = 0; h < SCREEN_HEIGHT; h++) {
		for (int w = 0; w < SCREEN_WIDTH; w++) {
			int index = h * SCREEN_WIDTH + w;
			float t = w * 1.0f / SCREEN_WIDTH;
			t += context.tick * ANIMATION_SPEED;
			t = fmod(fabs(t), 1.0f);
			float hue = 360.0f * t;
			float x = 1.0f - fabs(fmod((hue/60.0f), 2.0f) - 1.0f);
			float r1, g1, b1;
			if (hue < 60.0f) {
				r1 = 1.0f;
				g1 = x;
				b1 = 0.0f;
			} else if (hue < 120.0f) {
				r1 = x;
				g1 = 1.0f;
				b1 = 0.0f;
			} else if (hue < 180.0f) {
				r1 = 0.0f;
				g1 = 1.0f;
				b1 = x;
			} else if (hue < 240.0f) {
				r1 = 0.0f;
				g1 = x;
				b1 = 1.0f;
			} else if (hue < 300.0f) {
				r1 = x;
				g1 = 0.0f;
				b1 = 1.0f;
			} else {
				r1 = 1.0f;
				g1 = 0.0f;
				b1 = x;
			}
			pixels[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), (Uint8)(r1 * 255), (Uint8)(g1 * 255), (Uint8)(b1 * 255));
		} 
	}
#undef ANIMATION_SPEED
}
