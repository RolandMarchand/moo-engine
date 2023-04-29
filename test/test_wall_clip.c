#include "render.h"
#include <float.h>

int main()
{
	line wall;
	line clip;

	/* All visible */
	wall = Line(-10, 500, 10, 500);
	clip = clip_wall(wall);
	assert(memcmp(&wall, &clip, sizeof(line)) == 0);

	/* Out of FOV */
	wall = Line(-10, -500, 10, -500);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	wall = Line(-10000, 10, -9000, 10);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	wall = Line(9000, 10, 10000, 10);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	/* Flipped side */
	wall = Line(10, 500, -10, 500);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	wall = Line(10, -500, -10, -500);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	wall = Line(-10000, -10, -9000, -10);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	wall = Line(9000, -10, 10000, -10);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	/* Parallel to the player's view */
	wall = Line(0, 10, 0, 100);
	clip = clip_wall(wall);
	assert(!LineIsValid(clip));

	/* Partially clipped */
	wall = Line(-20, 10, 0, 10);
	clip = clip_wall(wall);
	assert(LineIsValid(clip));
	assert(fabs(clip.a.x + 10) < FLT_EPSILON);
	assert(clip.a.y == wall.a.y);
	assert(clip.b.x == wall.b.x);
	assert(clip.b.y == wall.b.y);

	wall = Line(0, 10, 20, 10);
	clip = clip_wall(wall);
	assert(LineIsValid(clip));
	assert(clip.a.x == wall.a.x);
	assert(clip.a.y == wall.a.y);
	assert(fabs(clip.b.x - 10) < FLT_EPSILON);
	assert(clip.b.y == wall.b.y);

	wall = Line(-20, 10, 40, 30);
	clip = clip_wall(wall);
	assert(LineIsValid(clip));
	assert(fabs(clip.a.x + 12.5) < FLT_EPSILON);
	assert(fabs(clip.a.y - 12.5) < FLT_EPSILON);
	assert(fabs(clip.b.x - 25) < FLT_EPSILON);
	assert(fabs(clip.b.y - 25) < FLT_EPSILON);

	wall = Line(-160, -80, 0, 80);
	clip = clip_wall(wall);
	assert(LineIsValid(clip));
	assert(fabs(clip.a.x + 40) < FLT_EPSILON);
	assert(fabs(clip.a.y - 40) < FLT_EPSILON);
	assert(clip.b.x == wall.b.x);
	assert(clip.b.y == wall.b.y);
}
