#pragma once

#include "draw.h"

#define FOV (90.0)

void draw_view(Uint32 *pixels, color);
void engine_render(void);
void animate_rainbow(Uint32 *pixels, double delta);
/* Make wall local in relation to global variable Player */
line wall_to_local(line wall);
/* Clip wall outside FOV. Wall must be made local with wall_to_local() */
line clip_wall(line wall);
