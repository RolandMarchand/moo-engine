#pragma once

#include "common.h"

#define VECTOR_INVALID Vector(NAN, NAN)
#define Vector(_x, _y) (vector){.w = (_x), .h = (_y)}
#define Line(_ax, _ay, _bx, _by) (line){.p1 = Vector((_ax), (_ay)), .p2 = Vector((_bx), (_by))}
#define Linev(_a, _b) (line){.p1 = (_a), .p2 = (_b)}
#define VectorCompare(_v1, _v2) (memcmp(&(_v1), &(_v2), sizeof(vector)) == 0)
#define LineCompare(_l1, _l2) (memcmp(&(_l1), &(_l2), sizeof(line)) == 0)
#define VectorIsValid(_v) !VectorCompare((_v), VECTOR_INVALID)
#define LineIsValid(_l) (VectorIsValid((_l).x) && VectorIsValid((_l).y))

#define VECTOR_ZERO Vector(0, 0)
#define VECTOR_UP Vector(0, 1)
#define VECTOR_DOWN Vector(0, -1)
#define VECTOR_LEFT Vector(-1, 0)
#define VECTOR_RIGHT Vector(1, 0)

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
vector vector_rotate(vector, double angle_rad);
vector vector_rotate_degrees(vector, double angle_deg);
