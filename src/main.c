#include "engine.h"

int main(void)
{
	engine_init();
	while (Context.event.type != SDL_QUIT) {
		engine_update();
		engine_input();
		engine_physics();
		engine_render();
	}
	engine_quit();
}
