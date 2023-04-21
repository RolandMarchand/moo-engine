#pragma once

#include <SDL2/SDL.h>
#include "math.h"
#include "vector.h"

#define PI 3.141592654

#define clamp(_x, _min, _max) (SDL_min(SDL_max(_x, _min), _max))
#define swap(_a, _b)				\
	do {					\
		__typeof__((_a)) _x = (_a);	\
		_a = (_b);			\
		_b = (_x);			\
	} while (0)
