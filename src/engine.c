#include "engine.h"
#include "draw.h"
#include "render.h"

struct context Context;
struct player Player;
int engine_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	memset(&Context, 0, sizeof(Context));
	Context.window
		= SDL_CreateWindow("My SDL2 App",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   SCREEN_WIDTH, SCREEN_HEIGHT,
				   SDL_WINDOW_SHOWN);
	Context.render
		= SDL_CreateRGBSurfaceWithFormat(0,
						 SCREEN_WIDTH,
						 SCREEN_HEIGHT,
						 32,
						 FORMAT);
	if (!Context.window) {
		printf("Window could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	if (!Context.render) {
		printf("Render surface could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}

	TTF_Init();
	Context.font = TTF_OpenFont("../res/font.ttf", 24);
	if (!Context.font) {
		fprintf(stderr, "Error: font cannot be loaded\n");
		exit(EXIT_FAILURE);
	}

	Player.camera = Vector(1, 0);

	return OK;
}

void engine_update(void)
{
	Context.frame++;

	/* delta */
	static Uint64 previous_time = 0;
	Uint64 current_time = SDL_GetTicks64();
	Context.delta = (current_time - previous_time) / 1000.0;
	previous_time = current_time;

	static double second = 0;
	static Uint64 previous_frame_cnt = 0;
	second += Context.delta;
	if (second > 1) {
		Context.fps = Context.frame - previous_frame_cnt;
		previous_frame_cnt = Context.frame;
		second = 0;
	}
}

void engine_input(void)
{
	SDL_PollEvent(&Context.event);
	switch (Context.event.type) {
	case SDL_KEYDOWN:
		if (Context.event.key.keysym.sym == SDLK_F1
		    && SDL_GetModState() & KMOD_LALT) {
			if (Context.event.key.repeat == 1) {
				break;
			}
		}
		if (Context.event.key.keysym.sym == SDLK_a) {
			Player.velocity =
				vector_add(Player.velocity, Vector(-1, 0));
		}
		if (Context.event.key.keysym.sym == SDLK_s) {
			Player.velocity =
				vector_add(Player.velocity, Vector(0, 1));
		}
		if (Context.event.key.keysym.sym == SDLK_w) {
			Player.velocity =
				vector_add(Player.velocity, Vector(0, -1));
		}
		if (Context.event.key.keysym.sym == SDLK_d) {
			Player.velocity =
				vector_add(Player.velocity, Vector(1, 0));
		}
		if (Context.event.key.keysym.sym == SDLK_LEFT) {
			Player.camera = vector_rotate(
				Player.camera,
				Context.delta * PLAYER_ROTATION_SPEED);
		}
		if (Context.event.key.keysym.sym == SDLK_RIGHT) {
			Player.camera = vector_rotate(
				Player.camera,
				Context.delta * -PLAYER_ROTATION_SPEED);
		}
		Player.velocity = vector_mul(Player.velocity,
					     Context.delta * PLAYER_WALK_SPEED);
		break;
	default:
		break;
	}
}

void engine_physics(void)
{
	Player.velocity = vector_mul(Player.velocity, PLAYER_WALK_SPEED * Context.delta);
	Player.position = vector_add(Player.position, Player.velocity);
	Player.velocity = Vector(0, 0);
}

void engine_quit(void)
{
	SDL_FreeSurface(Context.render);
	SDL_DestroyWindow(Context.window);
	TTF_CloseFont(Context.font);
	TTF_Quit();
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void engine_render(void)
{
	SDL_LockSurface(Context.render);
	/* draw_background(Context.render, Color(100, 100, 100)); */
	/* draw_line(Context.render->pixels, Line(-10, -10, 1000, 800), Color(255, 255, 255)); */
	/* draw_view(Context.render->pixels, */
	/* 	  Color(100, 100, 100), */
	/* 	  Color(255, 255, 255)); */
	animate_rainbow(Context.render->pixels);
	SDL_UnlockSurface(Context.render);
	SDL_BlitSurface(Context.render, NULL,
			SDL_GetWindowSurface(Context.window), NULL);

	if (Context.show_fps) {
		static char fps[8];
		static SDL_Surface *ui;
		sprintf(fps, "%lu", Context.fps);
		ui = TTF_RenderUTF8_Solid(Context.font, fps, (color){0, 0, 0, 255});
		SDL_BlitSurface(ui, NULL,
				SDL_GetWindowSurface(Context.window), NULL);
		SDL_FreeSurface(ui);
	}

	SDL_UpdateWindowSurface(Context.window);
}
