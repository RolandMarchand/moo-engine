#include "vector.h"
#include "assert.h"

void test_intersection();

int main(void)
{
 	test_intersection();
}

void test_intersection()
{
	line l1, l2;
	vector intersection;

	l1 = Linev(Vector(0, 0), Vector(4, 4));
	l2 = Linev(Vector(0, 4), Vector(4, 0));
	intersection = line_line_intersection(l1, l2);
	assert(VectorIsValid(intersection));

	l1 = Linev(Vector(0, 0), Vector(4, 4));
	l2 = Linev(Vector(4, 0), Vector(8, 4));
	intersection = line_line_intersection(l1, l2);
	assert(!VectorIsValid(intersection));

	l1 = Linev(Vector(0, 0), Vector(4, 4));
	l2 = Linev(Vector(1, 0), Vector(5, 2));
	intersection = line_line_intersection(l1, l2);
	assert(!VectorIsValid(intersection));

	l1 = Linev(Vector(0, 0), Vector(4, 4));
	l2 = Linev(Vector(0, 0), Vector(4, 4));
	intersection = line_line_intersection(l1, l2);
	assert(!VectorIsValid(intersection));

	l1 = Linev(Vector(0, 0), Vector(4, 4));
	l2 = Linev(Vector(0, 8), Vector(8, 0));
	intersection = line_line_intersection(l1, l2);
	assert(VectorIsValid(intersection));
}
