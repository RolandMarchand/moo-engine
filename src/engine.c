#include "engine.h"
#include "draw.h"
#include "render.h"
#include <SDL_scancode.h>

static struct config get_default_config();

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

	Player.camera = VECTOR_DOWN;
	Player.position = VECTOR_ZERO;

	Context.config = get_default_config();

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
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_A]) {
		Player.velocity =
			vector_add(Player.velocity,
				   vector_rotate(Player.camera, M_PI / -2));
	}
	if (state[SDL_SCANCODE_S]) {
		Player.velocity =
			vector_add(Player.velocity,
				   vector_mul(Player.camera, -1));
	}
	if (state[SDL_SCANCODE_W]) {
		Player.velocity =
			vector_add(Player.velocity, Player.camera);
	}
	if (state[SDL_SCANCODE_D]) {
		Player.velocity =
			vector_add(Player.velocity,
				   vector_rotate(Player.camera, M_PI / 2));
	}
	if (state[SDL_SCANCODE_LEFT]) {
		Player.camera = vector_rotate(
			Player.camera,
			Context.delta * -PLAYER_ROTATION_SPEED);
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		Player.camera = vector_rotate(
			Player.camera,
			Context.delta * PLAYER_ROTATION_SPEED);
	}

	SDL_PollEvent(&Context.event);
	switch (Context.event.type) {
	case SDL_KEYDOWN:
		switch (Context.event.key.keysym.sym) {
		case SDLK_F1:
			if ((SDL_GetModState() & KMOD_LALT)
			    && Context.event.key.repeat == 0) {
				Context.show_fps = !Context.show_fps;
			}
			break;
		case SDLK_TAB:
			if (Context.event.key.repeat == 0) {
				Context.config.map.relative = !Context.config.map.relative;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void engine_physics(void)
{
	Player.velocity = vector_normalize(Player.velocity);
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
	draw_background(Context.render, Color(100, 100, 100));
	/* draw_line(Context.render->pixels, Line(-10, -10, 1000, 800), Color(255, 255, 255)); */
	/* draw_view(Context.render->pixels, */
	/* 	  Color(100, 100, 100), */
	/* 	  Color(255, 255, 255)); */
	draw_map(Context.render->pixels, Color(255, 255, 255));
	/* animate_rainbow(Context.render->pixels, Context.delta); */
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

struct config get_default_config()
{
	return (struct config){
		.map.relative = false
	};
}
