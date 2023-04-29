#include "render.h"

void draw_view(Uint32 *pixels, color fg)
{
	Player.camera = VECTOR_DOWN;
	Player.position = VECTOR_ZERO;
	line wall = Line(-2000, 20, 2000, 40);
	wall = wall_to_local(wall);
	wall = clip_wall(wall);
	assert(LineIsValid(wall));

	wall = Line(-20, -20, 20, -20);
	wall = wall_to_local(wall);
	wall = clip_wall(wall);
	assert(!LineIsValid(wall));
}

line wall_to_local(line wall)
{
	double player_rotation = vector_angle_to(Player.camera, VECTOR_DOWN);
	wall.a = vector_sub(wall.a, Player.position);
	wall.b = vector_sub(wall.b, Player.position);
	wall.a = vector_rotate(wall.a, -player_rotation);
	wall.b = vector_rotate(wall.b, -player_rotation);
	return wall;
}

/*  */
line clip_wall(line wall)
{
	/* Backside */
	if (wall.a.x >= wall.b.x) {
		return LINE_INVALID;
	}

	/* Wall behind player */
	if (wall.a.y <= 0) {
		if (wall.b.y <= 0) {
			return LINE_INVALID;
		}
		line clip = Line(0, 0, wall.a.x, 0);
		wall.a = lines_intersect_at(wall, clip);
	} else if (wall.b.y <= 0) {
		line clip = Line(0, 0, wall.b.x, 0);
		wall.b = lines_intersect_at(wall, clip);
	}

	/* Clamp line at the FOV limits */
	double angle1 = vector_angle_to(wall.a, VECTOR_DOWN);
	double angle2 = vector_angle_to(wall.b, VECTOR_DOWN);
	if (angle1 < deg2rad(-FOV / 2)) {
		if (angle2 < deg2rad(-FOV / 2)) {
			return LINE_INVALID;
		}
		line angle_line_a = Linev(VECTOR_ZERO, vector_from_polar(-wall.a.x * M_SQRT2, deg2rad(-FOV / 2 - 90)));
		wall.a = lines_intersect_point(wall, angle_line_a);
		if (angle2 > deg2rad(FOV / 2)) {
			line angle_line_b = Linev(VECTOR_ZERO, vector_from_polar(wall.b.x * M_SQRT2, deg2rad(FOV / 2 - 90)));
			wall.b = lines_intersect_point(wall, angle_line_b);
		}
	} else if (angle1 > deg2rad(FOV / 2)) {
		if (angle2 > deg2rad(FOV / 2)) {
			return LINE_INVALID;
		}
		line angle_line_a = Linev(VECTOR_ZERO, vector_from_polar(wall.a.x * M_SQRT2, deg2rad(FOV / 2 - 90)));
		wall.a = lines_intersect_point(wall, angle_line_a);
	} else if (angle2 < deg2rad(-FOV / 2)) {
		line angle_line_b = Linev(VECTOR_ZERO, vector_from_polar(-wall.b.x * M_SQRT2, deg2rad(-FOV / 2 - 90)));
		wall.b = lines_intersect_point(wall, angle_line_b);
	} else if (angle2 > deg2rad(FOV / 2)) {
		line angle_line_b = Linev(VECTOR_ZERO, Vector(wall.b.x * M_SQRT2, deg2rad(FOV / 2 - 90)));
		wall.b = lines_intersect_point(wall, angle_line_b);
	}
	return wall;
}

void animate_rainbow(Uint32 *pixels)
{
#define ANIMATION_SPEED 0.1
	static double forward = 0;
	forward += Context.delta * ANIMATION_SPEED;
	forward = fmod(forward, 1.0);
	for (int h = 0; h < SCREEN_HEIGHT; h++) {
		for (int w = 0; w < SCREEN_WIDTH; w++) {
			double t = w * 1.0 / SCREEN_WIDTH;
			t += forward;
			t = fmod(fabs(t), 1.0);
			double hue = 360.0 * t;
			double x = 1.0 - fabs(fmod((hue/60.0), 2.0) - 1.0);
			double r1, g1, b1;
			if (hue < 60.0) {
				r1 = 1.0;
				g1 = x;
				b1 = 0.0;
			} else if (hue < 120.0) {
				r1 = x;
				g1 = 1.0;
				b1 = 0.0;
			} else if (hue < 180.0) {
				r1 = 0.0;
				g1 = 1.0;
				b1 = x;
			} else if (hue < 240.0) {
				r1 = 0.0;
				g1 = x;
				b1 = 1.0;
			} else if (hue < 300.0) {
				r1 = x;
				g1 = 0.0;
				b1 = 1.0;
			} else {
				r1 = 1.0;
				g1 = 0.0;
				b1 = x;
			}
			color c = {r1 * 255, g1 * 255, b1 * 255, 255};
			draw_pixel(pixels, w, h, c);
		} 
	}
#undef ANIMATION_SPEED
}
