#include "vector.h"

vector lines_intersect_at(line l1, line l2)
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

vector vector_div(vector a, double scalar)
{
	return Vector(a.x / scalar, a.y / scalar);
}

double vector_dot(vector a, vector b)
{
	return a.x * b.x + a.y * b.y;
}

double vector_cross(vector a, vector b)
{
	return a.x * b.y - a.y * b.x;
}

double vector_length(vector a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

double vector_angle_to(vector from, vector to)
{
	return atan2(vector_cross(from, to), vector_dot(from, to));
}

vector vector_projection(vector a, vector b)
{
	return vector_mul(a, vector_dot(a, b) / pow(vector_length(b), 2));
}

vector vector_normalize(vector a) {
	double length = a.x * a.x + a.y * a.y;
	if (length == 0) {
		return a;
	}
	length = sqrt(length);
	return Vector(a.x / length, a.y / length);
}

vector vector_rotate(vector v, double angle_rad)
{
	double sine = sin(angle_rad);
	double cosi = cos(angle_rad);
	return Vector(v.x * cosi - v.y * sine,
		      v.x * sine + v.y * cosi);
}

vector vector_rotate_degrees(vector v, double angle_deg)
{
	double angle_rad = angle_deg * M_PI / 180.0;
	return vector_rotate(v, angle_rad);
}

vector vector_from_angle(double angle_rad)
{
	return Vector(cos(angle_rad), sin(angle_rad));
}

vector vector_from_polar(double length, double angle_rad)
{
	return Vector(length * cos(angle_rad), length * sin(angle_rad));
}
