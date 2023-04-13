#include "level.h"

wall walls[9];

int main(void)
{
	/* hexagon room */
	walls[0] = (wall){Line(Vector(64, 128), Vector(128, 0)), -1};
	walls[1] = (wall){Line(Vector(128, 0), Vector(64, -128)), -1};
	walls[2] = (wall){Line(Vector(64, -128), Vector(-64, -128)), -1};
	walls[3] = (wall){Line(Vector(-64, -128), Vector(-128, 0)), -1};
	walls[4] = (wall){Line(Vector(-128, 0), Vector(-64, 128)), -1};
	walls[5] = (wall){Line(Vector(-64, 128), Vector(64, 128)), -1};

	/* triangle room */
	walls[6] = (wall){Line(Vector(0, 64), Vector(64, 0)), -1};
	walls[7] = (wall){Line(Vector(64, 0), Vector(-64, 0)), -1};
	walls[8] = (wall){Line(Vector(-64, 0), Vector(0, 64)), -1};

	sector hexagon = {0, 6};
	assert(!inside(walls, hexagon, Vector(1000, 1000)));
	assert(!inside(walls, hexagon, Vector(-1000, 1000)));
	assert(!inside(walls, hexagon, Vector(1000, -1000)));
	assert(!inside(walls, hexagon, Vector(-1000, -1000)));
	assert(inside(walls, hexagon, Vector(0, 64)));
	assert(inside(walls, hexagon, Vector(0, 0)));

	sector triangle = {6, 3};
	assert(!inside(walls, triangle, Vector(64, 64)));
	assert(!inside(walls, triangle, Vector(0, 64)));
	assert(!inside(walls, triangle, Vector(128, 0)));
	assert(!inside(walls, triangle, Vector(0, 0)));
	assert(inside(walls, triangle, Vector(0, 32)));
}
