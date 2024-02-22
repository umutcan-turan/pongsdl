#include <stdlib.h>

#include "defs.h"
#include "structs.h"
#include "sdl_util.h"
#include "util.h"
#include "text.h"
#include "menu.h"

game g;

void input(void)
{
	SDL_Event e;
	while (g.running && SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			g.running = 0;
		} else if (e.type == SDL_WINDOWEVENT) {
			update_resolution();
		} else if (e.type == SDL_CONTROLLERDEVICEADDED) {
		} else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
		} else if (g.funcs.input) {
				g.funcs.input(&e);
		}
	}
}

void logic(double dt)
{
	if (g.funcs.logic) {
		g.funcs.logic(dt);
	}
}

void draw(void)
{
	SDL_SetRenderDrawColor(g.ren, 0, 0, 0, 255);
	SDL_RenderClear(g.ren);
	if (g.funcs.render) {
		g.funcs.render();
	}
	SDL_RenderPresent(g.ren);
}

int main(void)
{
	Uint64 prev;
	Uint64 cur;
	Uint64 acc;
	Uint64 frame_time;

	if (sdl_init()) {
		die("sdl_init failed");
	}

	text_init();
	update_resolution();

	menu_enter();

	g.running = 1;
	prev = SDL_GetTicks64();
	acc = 0;
	while (g.running) {
		cur = SDL_GetTicks64();
		frame_time = cur - prev;
		prev = cur;
		acc += frame_time;

		while (acc >= 1000 / 100) { // 100 updates/sec, 10ms frametime
			input();
			logic(1.0 / 100.0);
			acc -= 1000 / 100;
		}
		draw();
	}
	text_cleanup();
	sdl_cleanup();
	return 0;
}
