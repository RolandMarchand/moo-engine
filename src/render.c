#include "render.h"
#include "draw.h"
#include "engine.h"

static line get_frustum_left(double furthest_x);
static line get_frustum_right(double furthest_x);
static void draw_absolute(Uint32 *pixels, color fg);
static void draw_relative(Uint32 *pixels, color fg);

static void draw_absolute(Uint32 *pixels, color fg)
{
	line wall = Line(-5, 20, 5, 3);
	vector origin = Vector(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
	wall.a = vector_sub(wall.a, Player.position);
	wall.b = vector_sub(wall.b, Player.position);
	wall.a = vector_add(wall.a, origin);
	wall.b = vector_add(wall.b, origin);
	draw_line(pixels, wall, fg);
	vector new_cam = vector_mul(Player.camera, 20);
	draw_line(pixels, Linev(origin, vector_add(origin, new_cam)),
		  fg);
	draw_rect(pixels,
		  Vector(SCREEN_WIDTH / 2.0 - 10, SCREEN_HEIGHT / 2.0 - 10),
		  Vector(SCREEN_WIDTH / 2.0 + 10, SCREEN_HEIGHT / 2.0 + 10),
		  fg);
}

static void draw_relative(Uint32 *pixels, color fg)
{
	line wall = Line(-5, 20, 5, 3);
	vector origin = Vector(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
	wall.a = vector_sub(wall.a, Player.position);
	wall.b = vector_sub(wall.b, Player.position);
	wall.a = vector_rotate(wall.a, vector_angle_to(Player.camera, VECTOR_UP));
	wall.b = vector_rotate(wall.b, vector_angle_to(Player.camera, VECTOR_UP));
	wall.a = vector_add(wall.a, origin);
	wall.b = vector_add(wall.b, origin);
	draw_line(pixels, wall, fg);
	draw_line(pixels,
		  Linev(origin, vector_add(origin, Vector(0, -20))),
		  fg);
	draw_rect(pixels,
		  Vector(SCREEN_WIDTH / 2.0 - 10, SCREEN_HEIGHT / 2.0 - 10),
		  Vector(SCREEN_WIDTH / 2.0 + 10, SCREEN_HEIGHT / 2.0 + 10),
		  fg);
}

void draw_map(Uint32 *pixels, color fg)
{
	if (Context.config.map == CONFIG_MAP_RELATIVE) {
		draw_relative(pixels, fg);
	} else {
		draw_absolute(pixels, fg);
	}
	
}

void draw_view(Uint32 *pixels, color fg)
{
	line wall = Line(-5, 20, 5, 3);
	wall = wall_to_local(wall);
//	wall = clip_wall(wall);
	if (!LineIsValid(wall)) {
		return;
	}
	vector euclidean_a = vector_sub(wall.a, Player.position);
	double angle_a = fabs(vector_angle_to(euclidean_a, Player.camera));
	double distance_a = sin(angle_a);

	vector euclidean_b = vector_sub(wall.b, Player.position);
	
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

static line get_frustum_left(double furthest_x)
{
	return Linev(VECTOR_ZERO,
		     vector_from_polar(fabs(furthest_x) * M_SQRT2,
				       deg2rad(90 + (FOV / 2))));
}

static line get_frustum_right(double furthest_x)
{
	return Linev(VECTOR_ZERO,
		     vector_from_polar(fabs(furthest_x) * M_SQRT2,
				       deg2rad(90 - (FOV / 2))));
}

line clip_wall(line wall)
{
	/* Backside */
	if (vector_angle_to(wall.a, wall.b) >= 0) {
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
		line frustum_left = get_frustum_left(wall.a.x);
		wall.a = lines_intersect_at(wall, frustum_left);
		if (angle2 > deg2rad(FOV / 2)) {
			line frustum_right = get_frustum_right(wall.b.x);
			wall.b = lines_intersect_at(wall, frustum_right);
		}
	} else if (angle1 > deg2rad(FOV / 2)) {
		if (angle2 > deg2rad(FOV / 2)) {
			return LINE_INVALID;
		}
		line frustum_left = get_frustum_left(wall.a.x);
		wall.a = lines_intersect_at(wall, frustum_left);
	} else if (angle2 < deg2rad(-FOV / 2)) {
		line frustum_right = get_frustum_right(wall.b.x);
		wall.b = lines_intersect_at(wall, frustum_right);
	} else if (angle2 > deg2rad(FOV / 2)) {
		line frustum_right = get_frustum_right(wall.b.x);
		wall.b = lines_intersect_at(wall, frustum_right);
	}
	return wall;
}

void animate_rainbow(Uint32 *pixels, double delta)
{
#define ANIMATION_SPEED 100
	static double forward = 0;
	static color row[SCREEN_WIDTH * 2];
	static bool row_set;
	if (!row_set) {
		row_set = true;
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			double t = i * 1.0 / SCREEN_WIDTH;
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
			row[i] = Color(r1 * 255, g1 * 255, b1 * 255);
		}
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			row[i + SCREEN_WIDTH] = row[i];
		}
	}
	for (int h = 0; h < SCREEN_HEIGHT; h++) {
		for (int w = 0; w < SCREEN_WIDTH; w++) {
			draw_pixel(pixels, w, h, row[(h + w + (int)(forward)) %  SCREEN_WIDTH * 2]);
		}
	}
	forward += delta * ANIMATION_SPEED;
	forward = fmod(forward, SCREEN_WIDTH);
#undef ANIMATION_SPEED
}
