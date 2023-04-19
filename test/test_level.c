#include "level.h"

extern sector_idx_t LastKnownSector;

void test_inside(void);
void test_update_sector(void);

int main(void)
{
	test_update_sector();
	test_inside();
}

void test_inside(void)
{
	struct wall walls[] = {
		/* hexagon room */
		{Line(Vector(64, 128), Vector(128, 0)), INVALID_SECTOR},
		{Line(Vector(128, 0), Vector(64, -128)), INVALID_SECTOR},
		{Line(Vector(64, -128), Vector(-64, -128)), INVALID_SECTOR},
		{Line(Vector(-64, -128), Vector(-128, 0)), INVALID_SECTOR},
		{Line(Vector(-128, 0), Vector(-64, 128)), INVALID_SECTOR},
		{Line(Vector(-64, 128), Vector(64, 128)), INVALID_SECTOR},
		/* triangle room */
		{Line(Vector(0, 64), Vector(64, 0)), INVALID_SECTOR},
		{Line(Vector(64, 0), Vector(-64, 0)), INVALID_SECTOR},
		{Line(Vector(-64, 0), Vector(0, 64)), -1}
	};
	struct sector two_rooms[] = {
		{0, 6}, /* hexagon */
		{6, 3} /* triangle */
	};
	initialize_level(walls, two_rooms, 9, 2);

	/* hexagon */
	assert(!inside(0, Vector(1000, 1000)));
	assert(!inside(0, Vector(-1000, 1000)));
	assert(!inside(0, Vector(1000, -1000)));
	assert(!inside(0, Vector(-1000, -1000)));
	assert(inside(0, Vector(0, 64)));
	assert(inside(0, Vector(0, 0)));

	/* triangle */
	assert(!inside(1, Vector(64, 64)));
	assert(!inside(1, Vector(0, 64)));
	assert(!inside(1, Vector(128, 0)));
	assert(!inside(1, Vector(0, 0)));
	assert(inside(1, Vector(0, 32)));

	terminate_level();
}

void test_update_sector(void)
{
/*
  o---+---+
  | 0 | 1 |
  +---+---+
  | 3 | 2 |
  +---+---+
*/
	struct wall walls[] = {
		/* 0 */
		{Line(Vector(0, 0), Vector(64, 0)), INVALID_SECTOR},
		{Line(Vector(64, 0), Vector(64, -64)), 1},
		{Line(Vector(64, -64), Vector(0, -64)), 3},
		{Line(Vector(0, -64), Vector(0, 0)), INVALID_SECTOR},
		/* 1 */
		{Line(Vector(64, 0), Vector(128, 0)), INVALID_SECTOR},
		{Line(Vector(128, 0), Vector(128, -64)), INVALID_SECTOR},
		{Line(Vector(128, -64), Vector(64, -64)), 2},
		{Line(Vector(64, -64), Vector(64, 0)), 0},
		/* 2 */
		{Line(Vector(64, -64), Vector(128, -64)), 1},
		{Line(Vector(128, -64), Vector(128, -128)), INVALID_SECTOR},
		{Line(Vector(128, -128), Vector(64, -128)), INVALID_SECTOR},
		{Line(Vector(64, -128), Vector(64, -64)), 3},
		/* 3 */
		{Line(Vector(0, -64), Vector(64, -64)), 0},
		{Line(Vector(64, -64), Vector(64, -128)), 2},
		{Line(Vector(64, -128), Vector(0, -128)), INVALID_SECTOR},
		{Line(Vector(0, -128), Vector(0, -64)), INVALID_SECTOR},
	};
	struct sector sectors[] = {
		{0, 4},
		{4, 4},
		{8, 4},
		{12, 4}
	};
	initialize_level(walls, sectors, 16, 4);

	assert(LastKnownSector == INVALID_SECTOR);
	update_sector(Vector(32, -32));
	assert(LastKnownSector == 0);
	update_sector(Vector(32, -32));
	assert(LastKnownSector == 0);
	update_sector(Vector(96, -32));
	assert(LastKnownSector == 1);
	update_sector(Vector(32, -96));
	assert(LastKnownSector == 3);
	update_sector(Vector(-128, 128));
	assert(LastKnownSector == INVALID_SECTOR);

	terminate_level();
}
