#pragma once

#include <SDL2/SDL.h>
#include "math.h"
#include "vector.h"

#define clamp(_x, _min, _max) (SDL_min(SDL_max(_x, _min), _max))
#define swap(_a, _b)				\
	do {					\
		__typeof__((_a)) _x = (_a);	\
		_a = (_b);			\
		_b = (_x);			\
	} while (0)
#define deg2rad(_deg) ((_deg) * M_PI / 180.0)
#define rad2deg(_rad) ((_rad) * 180.0 / M_PI)
