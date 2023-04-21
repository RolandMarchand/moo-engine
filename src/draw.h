#pragma once

#include <SDL2/SDL.h>
#include "common.h"
#include "u_math.h"

#define Color(_r, _g, _b) ((struct color){ .r = _r, .g = _g, .b = _b})

struct color {
	union {
		Uint8 red;
		Uint8 r;
	};
	union {
		Uint8 green;
		Uint8 g;
	};
	union {
		Uint8 blue;
		Uint8 b;
	};
};

void draw_line(Uint32 *pixels, line, SDL_Color);
void draw_pixel(Uint32 *pixels, int pixel_x, int pixel_y, SDL_Color c);
void draw_rect(Uint32 *pixels, vector p0, vector p1, SDL_Color);
void draw_triangle(Uint32 *pixels, vector a, vector b, vector c, SDL_Color);
void draw_quad(Uint32 *pixels, vector p1, vector p2, vector p3, vector p4, SDL_Color c);
