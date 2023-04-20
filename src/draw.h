#pragma once

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

void draw_pixel(Uint32 *pixels, size_t index, struct color c);
void draw_rect(Uint32 *pixels, vector p0, vector p1, struct color);
