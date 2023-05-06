#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_keycode.h>

#include "vector.h"

#define PLAYER_WALK_SPEED (100.0)
#define PLAYER_ROTATION_SPEED (2.0)
#define FOV (90.0)

/* First element in config enums is default */

typedef enum {
	CONFIG_MAP_ABSOLUTE = 1,
	CONFIG_MAP_RELATIVE = 2,
} ConfigMap;

typedef enum {
	CONFIG_VIEW_MAP = 1,
	CONFIG_VIEW_3D = 2,
} ConfigView;

struct config {
	ConfigMap map;
	ConfigView view;
};

extern struct context {
	SDL_Window *window;
	SDL_Surface *render;
	TTF_Font *font;
	SDL_Event event;

	Uint64 frame;
	double delta;

	Uint64 fps;
	bool show_fps;

	struct config config;
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
