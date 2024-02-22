#include "structs.h"
#include "defs.h"
#include "util.h"
#include "sdl_util.h"
#include "text.h"
#include "obj.h"
#include "menu.h"

extern game g;

enum {
	PLAY_SERVE,
	PLAY_PLAY,
	PLAY_OVER
};

static int playstate;
static player players[2];
static obj ball;
static int winner;
static int serving;

static void play_input(SDL_Event *e);
static void play_logic(double dt);
static void play_render(void);
static void reset_keys(void);
static void reset_pos(void);
static void player_win_round(int round_winner);
static void draw_player_scores(void);

static void play_input(SDL_Event *e)
{
	int scancode;

	if (e->type != SDL_KEYDOWN && e->type != SDL_KEYUP) {
		return;
	}
	scancode = e->key.keysym.scancode;

	if (scancode == SDL_SCANCODE_ESCAPE) {
		menu_enter();
	}

	if (playstate == PLAY_OVER) {
		if (scancode == SDL_SCANCODE_RETURN) {
			menu_enter();
		}
		return;
	}

	switch (scancode) {
	case SDL_SCANCODE_W:
		players[0].up = e->type == SDL_KEYDOWN;
		break;
	case SDL_SCANCODE_S:
		players[0].down = e->type == SDL_KEYDOWN;
		break;
	case SDL_SCANCODE_UP:
		players[1].up = e->type == SDL_KEYDOWN;
		break;
	case SDL_SCANCODE_DOWN:
		players[1].down = e->type == SDL_KEYDOWN;
		break;
	case SDL_SCANCODE_RETURN:
		if (e->type == SDL_KEYDOWN) {
			g.funcs.logic = g.funcs.logic ? NULL : play_logic;
		}
		break;
	default:
		break;
	}

}

static void play_logic(double dt)
{
	if (playstate == PLAY_SERVE) {
		for (int i = 0; i < 2; i++) {
			if (serving == i && (players[i].up || players[i].down)) {
				playstate = PLAY_PLAY;
				reset_pos();
			}
		}
	}
	
	if (playstate == PLAY_SERVE || playstate == PLAY_OVER) {
		return;
	}

	obj_update(&ball, dt);

	for (int i = 0; i < 2; i++) {
		obj *paddle = &players[i].paddle;

		obj_setdelta(paddle, 0, 0);
		if (players[i].up) {
			obj_setdelta(paddle, 0, -50);
		}
		if (players[i].down) {
			obj_setdelta(paddle, 0, 50);
		}

		obj_update(paddle, dt);

		// Keep players on screen
		obj_y_min(paddle, 0);
		obj_y_max(paddle, WORLD_HEIGHT);

		// If ball hits paddle, make it bounce
		if (obj_aabb(&ball, paddle)) {
			if (players[i].up ) {
				obj_setdelta(&ball, -ball.dx * 1.04, rand_range(ball.dy - 40, ball.dy - 10));
			} else if (players[i].down) {
				obj_setdelta(&ball, -ball.dx * 1.04, rand_range(ball.dy + 10, ball.dy + 40));
			} else {
				obj_setdelta(&ball, -ball.dx * 1.04, rand_range(ball.dy - 15, ball.dy + 15));
			}
			if (i == 0) {
				obj_setpos(&ball, paddle->body.x + (PADDLE_WIDTH + BALL_WIDTH) / 2, ball.body.y);
			} else {
				obj_setpos(&ball, paddle->body.x - (PADDLE_WIDTH + BALL_WIDTH) / 2, ball.body.y);
			}
		}
	}

	// Keep ball on screen
	if (obj_y_min(&ball, 0) || obj_y_max(&ball, WORLD_HEIGHT)) {
		obj_setdelta(&ball, ball.dx, -ball.dy);
	}

	// Score conditions
	if (obj_x_max(&ball, TOTAL_WIDTH)) {
		player_win_round(0);
	}
	if (obj_x_min(&ball, 0)) {
		player_win_round(1);
	}
}

static void play_render(void)
{
	ren_copy(g.bg_tex, NULL, &(SDL_FRect){0,0,TOTAL_WIDTH,WORLD_HEIGHT+HUD_HEIGHT});
	ren_rect_centered((SDL_FRect){TOTAL_WIDTH / 2, WORLD_HEIGHT + HUD_HEIGHT / 2, TOTAL_WIDTH, HUD_HEIGHT}, 0, 0, 0, 1);

	if (playstate == PLAY_OVER) {
		text_draw(TOTAL_WIDTH / 2, (WORLD_HEIGHT + HUD_HEIGHT) / 2, winner == 0 ? "Player 1 wins!" : "Player 2 wins!", 5);
		text_draw(TOTAL_WIDTH / 2, (WORLD_HEIGHT + HUD_HEIGHT) / 2 + 10, "Press ENTER to return to menu", 3);
	} else {
		obj_rendercopy(&players[0].paddle, g.player_tex, NULL);
		obj_rendercopy(&players[1].paddle, g.player_tex, NULL);
		obj_rendercopy(&ball, g.ball_tex, NULL);
	}

	if (g.funcs.logic == NULL) {
		text_draw(TOTAL_WIDTH / 2, WORLD_HEIGHT / 2 - 10, "PAUSE", 4);
	}
	text_draw(TOTAL_WIDTH / 2, WORLD_HEIGHT + HUD_HEIGHT / 2, "SCORE", 3);
	draw_player_scores();
}

static void reset_keys(void)
{
	for (int i = 0; i < 2; i++) {
		players[i].up = players[i].down = 0;
	}
}

static void reset_pos(void)
{
	for (int i = 0; i < 2; i++) {
		players[i].paddle = obj_new(
				i == 0 ? 4 : TOTAL_WIDTH - 4,
				WORLD_HEIGHT / 2,
				PADDLE_WIDTH, PADDLE_HEIGHT,
				0, 0);
	}

	ball = obj_new(
			TOTAL_WIDTH / 2, WORLD_HEIGHT / 2,
			BALL_WIDTH, BALL_HEIGHT,
			(rand_range(0, 1) ? 1 : -1) * rand_range(30, 70),
			(rand_range(0, 1) ? 1 : -1) * rand_range(-30, 30));
			/*
			(rand_range(0, 1) ? 1 : -1) * 20 + rand_range(0, 10),
			(rand_range(0, 1) ? 1 : -1) * 5 + rand_range(0, 10));
			*/
}

static void player_win_round(int round_winner)
{
	players[round_winner].score++;
	serving = round_winner;
	if (players[round_winner].score == 10) {
		playstate = PLAY_OVER;
		winner = round_winner;
	} else {
		playstate = PLAY_SERVE;
		reset_pos();
		reset_keys();
	}
}

static void draw_player_scores(void)
{
	SDL_FRect rect;
	
	rect.w = 2;
	rect.h = 2;
	rect.y = WORLD_HEIGHT + HUD_HEIGHT / 2;
	for (int i = 0; i < 10; i++) {
		rect.x = 4 + (i * 3);
		ren_rect_centered(rect, 255, 255, 255, i >= (10 - players[0].score));
		rect.x = TOTAL_WIDTH - 4 - (i * 3);
		ren_rect_centered(rect, 255, 255, 255, i >= (10 - players[1].score));
	}
}

void play_enter(void)
{
	reset_pos();
	reset_keys();
	players[0].score = players[1].score = 0;
	players[0].is_cpu = players[1].is_cpu = 0;
	serving = rand_range(0, 1);
	playstate = PLAY_SERVE;

	g.funcs.input = play_input;
	g.funcs.logic = play_logic;
	g.funcs.render = play_render;
}
