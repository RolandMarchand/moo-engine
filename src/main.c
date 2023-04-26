#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "u_math.h"
#include "draw.h"

#define PLAYER_SPEED 100

struct {
	SDL_Window *window;
	SDL_Surface *render;
	TTF_Font *font;
	SDL_Event event;

	Uint64 frame;
	double delta;

	Uint64 fps;
	bool show_fps;
} context;

struct {
	double angle;
	vector position;
	vector velocity;
} player;

int engine_init(void);
void engine_quit(void);
void engine_input(void);
void engine_physics(void);
void engine_update(void);
void engine_render(void);

void animate_rainbow(Uint32 *pixels);
void draw_view(int x, int y, double angle);

int engine_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	memset(&context, 0, sizeof(context));
	context.window
		= SDL_CreateWindow("My SDL2 App",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   SCREEN_WIDTH, SCREEN_HEIGHT,
				   SDL_WINDOW_SHOWN);
	context.render
		= SDL_CreateRGBSurfaceWithFormat(0,
						 SCREEN_WIDTH,
						 SCREEN_HEIGHT,
						 32,
						 FORMAT);
	if (!context.window) {
		printf("Window could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}
	if (!context.render) {
		printf("Render surface could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return ERROR;
	}

	TTF_Init();
	context.font = TTF_OpenFont("../res/font.ttf", 24);
	if (!context.font) {
		fprintf(stderr, "Error: font cannot be loaded\n");
		exit(EXIT_FAILURE);
	}

	return OK;
}

void engine_update(void)
{
	context.frame++;

	/* delta */
	static Uint64 previous_time = 0;
	Uint64 current_time = SDL_GetTicks64();
	context.delta = (current_time - previous_time) / 1000.0;
	previous_time = current_time;

	static double second = 0;
	static Uint64 previous_frame_cnt = 0;
	second += context.delta;
	if (second > 1) {
		context.fps = context.frame - previous_frame_cnt;
		previous_frame_cnt = context.frame;
		second = 0;
	}
}

void engine_input()
{
	SDL_PollEvent(&context.event);
	switch (context.event.type) {
	case SDL_KEYDOWN:
		if (context.event.key.keysym.sym == SDLK_F1
		    && SDL_GetModState() & KMOD_LALT) {
			if (context.event.key.repeat == 1) {
				break;
			}
		}
		if (context.event.key.keysym.sym == SDLK_a) {
			player.velocity =
				vector_add(player.velocity, Vector(-1, 0));
		}
		if (context.event.key.keysym.sym == SDLK_s) {
			player.velocity =
				vector_add(player.velocity, Vector(0, 1));
		}
		if (context.event.key.keysym.sym == SDLK_w) {
			player.velocity =
				vector_add(player.velocity, Vector(0, -1));
		}
		if (context.event.key.keysym.sym == SDLK_d) {
			player.velocity =
				vector_add(player.velocity, Vector(1, 0));
		}
		player.velocity = vector_normalize(player.velocity);
		player.velocity = vector_mul(player.velocity,
					     context.delta * PLAYER_SPEED);
		break;
	default:
		break;
	}
}

void engine_physics(void)
{
}

void engine_quit(void)
{
	SDL_FreeSurface(context.render);
	SDL_DestroyWindow(context.window);
	TTF_CloseFont(context.font);
	TTF_Quit();
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void engine_render(void)
{
	SDL_LockSurface(context.render);
	/* draw_rect(context.render->pixels, Vector(-300, -300), Vector(300, 300), Color(255, 0, 0)); */
	/* draw_view(0, 0, FOV); */
	animate_rainbow(context.render->pixels);
	SDL_UnlockSurface(context.render);
	SDL_BlitSurface(context.render, NULL,
			SDL_GetWindowSurface(context.window), NULL);

	if (context.show_fps) {
		static char fps[8];
		static SDL_Surface *ui;
		sprintf(fps, "%lu", context.fps);
		ui = TTF_RenderUTF8_Solid(context.font, fps, (SDL_Color){0, 0, 0, 255});
		SDL_BlitSurface(ui, NULL,
				SDL_GetWindowSurface(context.window), NULL);
		SDL_FreeSurface(ui);
	}

	SDL_UpdateWindowSurface(context.window);
}

int main(void)
{
	engine_init();
	while (context.event.type != SDL_QUIT) {
		engine_input();
		engine_update();
		engine_render();
	}
	engine_quit();
}

int screen_angle_to_x(double a)
{
	double a_pi4 = (FOV / 2.0 + a) / FOV * M_PI * 2.0 + (M_PI * 4.0);
	return SCREEN_WIDTH / 2.0 * (1.0 - tan(a_pi4));
}

void draw_view(int x, int y, double angle)
{
	SDL_Rect bg = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Uint32 bg_color = SDL_MapRGB(context.render->format, 100, 100, 100);
	SDL_FillRect(context.render, &bg, bg_color);

	vector pos = Vector(100, 100);
	vector dir = VECTOR_UP;
	vector plane = vector_rotate_degrees(dir, -90);

}

void animate_rainbow(Uint32 *pixels)
{
#define ANIMATION_SPEED 0.1
	static double forward = 0;
	forward += context.delta * ANIMATION_SPEED;
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
			SDL_Color c = {r1 * 255, g1 * 255, b1 * 255, 255};
			draw_pixel(pixels, w, h, c);
		} 
	}
#undef ANIMATION_SPEED
}
