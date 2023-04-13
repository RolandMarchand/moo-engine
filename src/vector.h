#pragma once

#include "common.h"

#define VECTOR_INVALID Vector(NAN, NAN)
#define Vector(x, y) (vector){.w = x, .h = y}
#define Line(a, b) (line){.p1 = a, .p2 = b}
#define VectorCompare(v1, v2) (memcmp(&v1, &v2, sizeof(vector)) == 0)
#define LineCompare(l1, l2) (memcmp(&l1, &l2, sizeof(line)) == 0)
#define VectorIsValid(v) !VectorCompare(v, VECTOR_INVALID)
#define LineIsValid(l) (VectorIsValid(l.x) && VectorIsValid(l.y))

typedef struct {
	union {
		struct {
			double u;
			double v;
		};
		struct {
			double x;
			double y;
		};
		struct {
			double width;
			double height;
		};
		struct {
			double w;
			double h;
		};
	};
} vector;

typedef struct {
	union {
		struct {
			vector a;
			vector b;
		};
		struct {
			vector x;
			vector y;
		};
		struct {
			vector p1;
			vector p2;
		};
		struct {
			vector from;
			vector to;
		};
	};
} line;

vector line_line_intersection(line, line);
vector vector_add(vector, vector);
vector vector_sub(vector, vector);
vector vector_mul(vector, double scalar);
vector vector_normalize(vector);
double vector_dot(vector, vector);
double vector_cross(vector, vector);
double vector_magnitude(vector);
double vector_projection(line, vector point);
vector vector_rotate(vector, double angle);
