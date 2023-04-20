#include "draw.h"
#include <SDL2/SDL.h>

void draw_pixel(Uint32 *pixels, size_t index, struct color c)
{
	pixels[index] = SDL_MapRGB(
		SDL_AllocFormat(FORMAT), c.r, c.g, c.b);
}

void draw_rect(Uint32 *pixels, vector p0, vector p1, struct color c)
{
	p0.x = clamp(p0.x, 0, SCREEN_WIDTH);
	p0.y = clamp(p0.y, 0, SCREEN_HEIGHT);
	p1.x = clamp(p1.x, 0, SCREEN_WIDTH);
	p1.y = clamp(p1.y, 0, SCREEN_HEIGHT);

	if (p0.x > p1.x) { swap(p0.x, p1.x); }
	if (p0.y > p1.y) { swap(p0.y, p1.y); }

	for (int y = p0.y; y < p1.y; y++) {
		for (int x = p0.x; x < p1.x; x++) {
			draw_pixel(pixels, y * SCREEN_WIDTH + x, c);
		}
	}
}
