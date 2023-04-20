#include "vector.h"

vector line_line_intersection(line l1, line l2)
{
	double x1 = l1.p1.x, x2 = l1.p2.x, x3 = l2.p1.x, x4 = l2.p2.x;
	double y1 = l1.p1.y, y2 = l1.p2.y, y3 = l2.p1.y, y4 = l2.p2.y;

	double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	/* collinear and overlapping */
	if (denominator == 0.0) {
		return VECTOR_INVALID;
	}

	double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4))
		/ denominator;
	double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3))
		/ denominator;

	return t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0
		? Vector(x1 + t * (x2 - x1), y1 + t * (y2 - y1))
		: VECTOR_INVALID;
}

vector vector_add(vector a, vector b)
{
	return Vector(a.x + b.x, a.y + b.y);
}

vector vector_sub(vector a, vector b)
{
	return Vector(a.x - b.x, a.y - b.y);
}

vector vector_mul(vector a, double scalar)
{
	return Vector(a.x * scalar, a.y * scalar);
}

double vector_dot(vector a, vector b)
{
	return a.x * b.x + a.y * b.y;
}

double vector_cross(vector a, vector b)
{
	return a.x * b.y - a.y * b.x;
}

double vector_magnitude(vector a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

vector vector_normalize(vector a)
{
	double magnitude = vector_magnitude(a);
	return Vector(a.x / magnitude, a.y / magnitude);
}

double vector_projection(line l, vector p)
{
	vector v1 = vector_sub(l.b, l.a);
	vector v2 = vector_sub(p, l.a);
	return vector_cross(v1, v2);
}

vector vector_rotate(vector v, double angle_rad)
{
	return Vector(v.x * cos(angle_rad) - v.y * sin(angle_rad),
		      v.x * sin(angle_rad) + v.y * cos(angle_rad));
}

vector vector_rotate_degrees(vector v, double angle_deg)
{
    double angle_rad = angle_deg * M_PI / 180.0;
    return vector_rotate(v, angle_rad);
}
