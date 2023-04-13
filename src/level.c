#include "level.h"

/* wall walls[6] = { */
/* 	{Line(Vector(64, 128), Vector(128, 0)), -1}, */
/* 	{Line(Vector(128, 0), Vector(64, -128)), -1}, */
/* 	{Line(Vector(64, -128), Vector(-64, -128)), -1}, */
/* 	{Line(Vector(-64, -128), Vector(-128, 0)), -1}, */
/* 	{Line(Vector(-128, 0), Vector(-64, 128)), -1}, */
/* 	{Line(Vector(-64, 128), Vector(64, 128)), -1}, */
/* }; */

/* sector sectors[1] = { */
/* 	{0, 6} */
/* }; */

bool inside(wall *walls, sector s, vector point)
{
	// TODO: check if sector is valid
	int intersection_count = 0;
	for (int i = 0; i < s.wall_count; i++) {
		line wall = walls[s.wall_start + i].line;

		/* handle vertexes */
		if (wall.a.y == point.y) {
			if (wall.a.x < point.x && wall.b.y < point.y) {
					intersection_count++;
			}
			continue;
		} else if (wall.b.y == point.y) {
			if (wall.b.x < point.x && wall.a.y < point.y) {
					intersection_count++;
			}
			continue;
		}

		double min_x = wall.a.x < wall.b.x ? wall.a.x : wall.b.x;
		vector crossed = line_line_intersection(
			wall, Line(Vector(min_x, point.y), point));

		if (VectorIsValid(crossed)) {
			intersection_count++;
		}
	}
	return intersection_count % 2 == 1;
}
