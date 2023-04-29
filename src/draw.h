#pragma once

#include <SDL2/SDL.h>
#include "common.h"
#include "u_math.h"

#define Color(_r, _g, _b) ((color){_r, _g, _b, 255})
#define Colora(_r, _g, _b, _a) ((color){_r, _g, _b, _a})

typedef SDL_Color color;

void draw_line(Uint32 *pixels, line, color);
void draw_pixel(Uint32 *pixels, int pixel_x, int pixel_y, color);
void draw_rect(Uint32 *pixels, vector p0, vector p1, color);
void draw_triangle(Uint32 *pixels, vector a, vector b, vector c, color);
void draw_quad(Uint32 *pixels,
	       vector top_left,
	       vector top_right,
	       vector bottom_right,
	       vector bottom_left,
	       color c);
void draw_wall(Uint32 *pixels,
	       vector top_left,
	       vector top_right,
	       vector bottom_right,
	       vector bottom_left,
	       color c);
void draw_background(SDL_Surface *surface, color);
