#include "structs.h"
#include "defs.h"
#include "util.h"
#include "sdl_util.h"
#include "text.h"
#include "obj.h"
#include "play.h"
#include "menu.h"

extern game g;

static obj balls[15];

static void menu_input(SDL_Event *e)
{
	if (e->type != SDL_KEYDOWN) {
		return;
	}
	if (e->key.keysym.scancode == SDL_SCANCODE_RETURN) {
		play_enter();
	}
	if (e->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		g.running = 0;
	}
}

static void menu_logic(double dt)
{
	for (int i = 0; i < 15; i++) {
		obj *b = balls + i;
		obj_update(b, dt);

		if (obj_x_min(b, 0) || obj_x_max(b, TOTAL_WIDTH)) {
			obj_setdelta(b, -b->dx, b->dy);
		}
		if (obj_y_min(b, 0) || obj_y_max(b, WORLD_HEIGHT + HUD_HEIGHT)) {
			obj_setdelta(b, b->dx, -b->dy);
		}
	}
}

static void menu_render(void)
{
	SDL_FRect bg = {
		0, 0, TOTAL_WIDTH, WORLD_HEIGHT + HUD_HEIGHT
	};

	ren_copy(g.bg_tex, NULL, &bg);
	for (int i = 0; i < 15; i++) {
		obj_rendercopy(balls + i, g.ball_tex, NULL);
	}
	text_draw(
			TOTAL_WIDTH / 2, (WORLD_HEIGHT + HUD_HEIGHT) / 2 - 20,
			"PONG", 4);
	text_draw(
			TOTAL_WIDTH / 2, (WORLD_HEIGHT + HUD_HEIGHT) / 2 + 10,
			"Press ENTER to play", 3);
}

void menu_enter(void)
{
	for (int i = 0; i < 15; i++) {
		obj *b = balls + i;

		*b = obj_new(
				rand_range(BALL_WIDTH / 2, TOTAL_WIDTH - BALL_WIDTH / 2),
				rand_range(BALL_HEIGHT / 2, WORLD_HEIGHT + HUD_HEIGHT - BALL_HEIGHT / 2),
				BALL_WIDTH,
				BALL_HEIGHT,
				(rand_norm() > 0.5 ? 1 : -1) * (30 + rand_range(-10, 10)),
				(rand_norm() > 0.5 ? 1 : -1) * (30 + rand_range(-10, 10)));
	}
	menu_continue();
}

void menu_continue(void)
{
	g.funcs.input = menu_input;
	g.funcs.logic = menu_logic;
	g.funcs.render = menu_render;
}
