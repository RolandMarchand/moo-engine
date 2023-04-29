#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_keycode.h>

#include "vector.h"

#define PLAYER_WALK_SPEED (500.0)
#define PLAYER_ROTATION_SPEED (20.0)
#define FOV (90.0)

extern struct context {
	SDL_Window *window;
	SDL_Surface *render;
	TTF_Font *font;
	SDL_Event event;

	Uint64 frame;
	double delta;

	Uint64 fps;
	bool show_fps;
} Context;

extern struct player {
	vector position;
	vector camera;
	vector velocity;
} Player;

int engine_init(void);
void engine_quit(void);
void engine_update(void);
void engine_input(void);
void engine_physics(void);
void engine_render(void);
