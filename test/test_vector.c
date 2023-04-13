#include "vector.h"
#include "assert.h"

void test_intersection();
void test_projection();

int main(void)
{
	test_projection();
 	test_intersection();
}

void test_projection()
{
	vector v;
	double projection;
	line l = Line(Vector(0, -1), Vector(0, 1));

	v = Vector(-1, 0);
	projection = vector_projection(l, v);
	assert(projection > 0);

	v = Vector(1, 0);
	projection = vector_projection(l, v);
	assert(projection < 0);

	v = Vector(0, 0);
	projection = vector_projection(l, v);
	assert(projection == 0);
}

void test_intersection()
{
	line l1, l2;
	vector intersection;

	l1 = Line(Vector(0, 0), Vector(4, 4));
	l2 = Line(Vector(0, 4), Vector(4, 0));
	intersection = line_line_intersection(l1, l2);
	assert(VectorIsValid(intersection));

	l1 = Line(Vector(0, 0), Vector(4, 4));
	l2 = Line(Vector(4, 0), Vector(8, 4));
	intersection = line_line_intersection(l1, l2);
	assert(!VectorIsValid(intersection));

	l1 = Line(Vector(0, 0), Vector(4, 4));
	l2 = Line(Vector(1, 0), Vector(5, 2));
	intersection = line_line_intersection(l1, l2);
	assert(!VectorIsValid(intersection));

	l1 = Line(Vector(0, 0), Vector(4, 4));
	l2 = Line(Vector(0, 0), Vector(4, 4));
	intersection = line_line_intersection(l1, l2);
	assert(!VectorIsValid(intersection));

	l1 = Line(Vector(0, 0), Vector(4, 4));
	l2 = Line(Vector(0, 8), Vector(8, 0));
	intersection = line_line_intersection(l1, l2);
	assert(VectorIsValid(intersection));
}
