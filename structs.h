#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

enum {
	CONTROL_MOUSE = 0,
	CONTROL_KB,
	CONTROL_GAMEPAD
};

enum {
	KEY_UP = 0,
	KEY_DOWN,
	KEY_SELECT
};

enum {
	BUTTON_UP = 0,
	BUTTON_DOWN,
	BUTTON_SELECT
};

typedef struct {
	int type;
	int joystick_id;
	int keys[3];
} controls;

typedef struct {
	SDL_FRect body;
	double dx;
	double dy;
} obj;

typedef struct {
	obj paddle;
	int up;
	int down;
	int is_cpu;
	int score;
} player;

typedef struct {
	void (*input)(SDL_Event *e);
	void (*logic)(double dt);
	void (*render)(void);
} game_funcs;

typedef struct {
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *player_tex;
	SDL_Texture *ball_tex;
	SDL_Texture *bg_tex;
	controls c[2];
	game_funcs funcs;
	int running;
	int resolution_changed;
} game;

 #endif
