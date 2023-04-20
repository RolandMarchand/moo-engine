#pragma once

#include "math.h"
#include "vector.h"

#define PI 3.141592654

#define min(_x, _y) (_x < _y ? _x : _y)
#define max(_x, _y) (_x > _y ? _x : _y)
#define clamp(_x, _min, _max) (min(max(_x, _min), _max))
#define swap(_a, _b)				\
	do {					\
		__typeof__((_a)) _x = (_a);	\
		_a = (_b);			\
		_b = (_x);			\
	} while (0)
