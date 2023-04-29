#include "draw.h"

static void draw_top_triangle(Uint32 *pixels, vector v1, vector v2, vector v3, color color);
static void draw_bottom_triangle(Uint32 *pixels, vector v1, vector v2, vector v3, color color);

void draw_line(Uint32 *pixels, line _line, color color) {
	// check if line is at all on screen
	line cline;
	cline.a.x = clamp(_line.a.x, 0, SCREEN_WIDTH);
	cline.a.y = clamp(_line.a.y, 0, SCREEN_HEIGHT);
	cline.b.x = clamp(_line.b.x, 0, SCREEN_WIDTH);
	cline.b.y = clamp(_line.b.y, 0, SCREEN_HEIGHT);

	if (cline.a.x > cline.b.x) { swap(cline.a.x, cline.b.x); }
	if (cline.a.y > cline.b.y) { swap(cline.a.y, cline.b.y); }

	if (cline.a.x >= SCREEN_WIDTH
	    || cline.a.y >= SCREEN_HEIGHT
	    || cline.b.x < 0
	    || cline.b.y < 0) {
		return;
	}

	/* use DDA algorithm to draw line */
	double dx = _line.b.x - _line.a.x;
	double dy = _line.b.y - _line.a.y;

	double step = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

	dx /= step;
	dy /= step;

	double x = _line.a.x, y = _line.a.y;
	for (int i = 0; i <= step; i++) {
		draw_pixel(pixels, x, y, color);
		x += dx;
		y += dy;
	}
}

void draw_top_triangle(Uint32 *pixels, vector v1, vector v2, vector v3, color color)
{
	double invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
	double invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

	double curx1 = v3.x;
	double curx2 = v3.x;

	for (int scanline_y = v3.y; scanline_y > v1.y; scanline_y--) {
		draw_line(pixels, Line(curx1, scanline_y, curx2, scanline_y), color);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void draw_bottom_triangle(Uint32 *pixels, vector v1, vector v2, vector v3, color color)
{
	double invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
	double invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

	double curx1 = v1.x;
	double curx2 = v1.x;

	for (int scanline_y = v1.y; scanline_y <= v2.y; scanline_y++) {
		draw_line(pixels, Line(curx1, scanline_y, curx2, scanline_y), color);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void draw_triangle(Uint32 *pixels, vector v1, vector v2, vector v3, color color)
{
	if (v1.y > v2.y) {
		swap(v1, v2);
	}
	if (v2.y > v3.y) {
		swap(v2, v3);
	}
	if (v1.y > v3.y) {
		swap(v1, v3);
	}

	if (v2.y == v3.y) {
		draw_bottom_triangle(pixels, v1, v2, v3, color);
	}
	else if (v1.y == v2.y) {
		draw_top_triangle(pixels, v1, v2, v3, color);
	} else {
		vector v4 = Vector(v1.x + ((v2.y - v1.y) / (v3.y - v1.y)) * (v3.x - v1.x), v2.y);
		draw_bottom_triangle(pixels, v1, v2, v4, color);
		draw_top_triangle(pixels, v2, v4, v3, color);
	}
}

void draw_pixel(Uint32 *pixels, int pixel_x, int pixel_y, color c)
{
	if (pixel_x < 0 || pixel_y < 0 ||
	    pixel_x >= SCREEN_WIDTH || pixel_y >= SCREEN_HEIGHT) {
		return;
	}
	pixels[pixel_y * SCREEN_WIDTH + pixel_x] = SDL_MapRGB(
		SDL_AllocFormat(FORMAT), c.r, c.g, c.b);
}

void draw_rect(Uint32 *pixels, vector p0, vector p1, color c)
{
	p0.x = clamp(p0.x, 0, SCREEN_WIDTH);
	p0.y = clamp(p0.y, 0, SCREEN_HEIGHT);
	p1.x = clamp(p1.x, 0, SCREEN_WIDTH);
	p1.y = clamp(p1.y, 0, SCREEN_HEIGHT);

	if (p0.x > p1.x) { swap(p0.x, p1.x); }
	if (p0.y > p1.y) { swap(p0.y, p1.y); }

	for (int y = p0.y; y < p1.y; y++) {
		for (int x = p0.x; x < p1.x; x++) {
			draw_pixel(pixels, x, y, c);
		}
	}
}

void draw_quad(Uint32 *pixels,
	       vector top_left,
	       vector top_right,
	       vector bottom_right,
	       vector bottom_left,
	       color c)
{
	draw_triangle(pixels, top_left, top_right, bottom_left, c);
	draw_triangle(pixels, top_right, bottom_right, bottom_left, c);
}

void draw_wall(Uint32 *pixels,
	       vector top_left,
	       vector top_right,
	       vector bottom_right,
	       vector bottom_left,
	       color c)
{
	vector center_offset = Vector(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
	draw_quad(pixels,
		  vector_add(top_left, center_offset),
		  vector_add(top_right, center_offset),
		  vector_add(bottom_right, center_offset),
		  vector_add(bottom_left, center_offset),
		  c);
}

void draw_background(SDL_Surface *surface, color c)
{
	Uint32 bg_color = SDL_MapRGB(surface->format, c.r, c.g, c.b);
	SDL_FillRect(surface, NULL, bg_color);
}
