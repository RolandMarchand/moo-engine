#include <SDL2/SDL.h>

#include "common.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define FORMAT SDL_PIXELFORMAT_RGB888

struct {
	SDL_Window *window;
	SDL_Surface *backbuffer;
	Uint64 tick;
} context;

void animate_rainbow(Uint32 *pixels);
void draw_view(Uint32 *pixels, int x, int y, double angle);

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
	/* draw_view(context.backbuffer->pixels, 5 * TILE_SIZE, 2 * TILE_SIZE, 0); */
	/* animate_rainbow(context.backbuffer->pixels); */
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

void draw_view(Uint32 *pixels, int x, int y, double angle)
{
	SDL_Rect bg = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Uint32 bg_color = SDL_MapRGB(context.backbuffer->format, 100, 100, 100);
	SDL_FillRect(context.backbuffer, &bg, bg_color);

	double fov = 90.0;
	double slice = fov / SCREEN_WIDTH;
	angle = angle - (fov / 2.0);
	for (int col = 0; col < SCREEN_WIDTH; col++) {
		
	}
}

void animate_rainbow(Uint32 *pixels)
{
#define ANIMATION_SPEED 0.003
	for (int h = 0; h < SCREEN_HEIGHT; h++) {
		for (int w = 0; w < SCREEN_WIDTH; w++) {
			int index = h * SCREEN_WIDTH + w;
			double t = w * 1.0 / SCREEN_WIDTH;
			t += context.tick * ANIMATION_SPEED;
			t = fmod(fabs(t), 1.0);
			double hue = 360.0 * t;
			double x = 1.0 - fabs(fmod((hue/60.0), 2.0) - 1.0);
			double r1, g1, b1;
			if (hue < 60.0) {
				r1 = 1.0;
				g1 = x;
				b1 = 0.0;
			} else if (hue < 120.0) {
				r1 = x;
				g1 = 1.0;
				b1 = 0.0;
			} else if (hue < 180.0) {
				r1 = 0.0;
				g1 = 1.0;
				b1 = x;
			} else if (hue < 240.0) {
				r1 = 0.0;
				g1 = x;
				b1 = 1.0;
			} else if (hue < 300.0) {
				r1 = x;
				g1 = 0.0;
				b1 = 1.0;
			} else {
				r1 = 1.0;
				g1 = 0.0;
				b1 = x;
			}
			pixels[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), (Uint8)(r1 * 255), (Uint8)(g1 * 255), (Uint8)(b1 * 255));
		} 
	}
#undef ANIMATION_SPEED
}
