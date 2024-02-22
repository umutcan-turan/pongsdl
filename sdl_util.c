#include "defs.h"
#include "structs.h"
#include "sdl_util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern game g;

static double scale;
static double xoffset;
static double yoffset;

int sdl_init(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	g.win = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!g.win) {
		return 1;
	}
	g.ren = SDL_CreateRenderer(g.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!g.ren) {
		SDL_DestroyWindow(g.win);
		return 1;
	}

	IMG_Init(IMG_INIT_PNG);

	g.player_tex = IMG_LoadTexture(g.ren, "./asset/player2.png");
	g.ball_tex = IMG_LoadTexture(g.ren, "./asset/ball2.png");
	g.bg_tex = IMG_LoadTexture(g.ren, "./asset/background2.png");

	return 0;
}

void sdl_cleanup(void)
{
	SDL_DestroyTexture(g.player_tex);
	SDL_DestroyTexture(g.ball_tex);
	SDL_DestroyTexture(g.bg_tex);
	IMG_Quit();
	SDL_DestroyRenderer(g.ren);
	SDL_DestroyWindow(g.win);
	SDL_Quit();
}

void update_resolution(void)
{
	int width, height;
	double xscale;
	double yscale;

	SDL_GetWindowSize(g.win, &width, &height);
	xscale = (double)width / TOTAL_WIDTH;
	yscale = (double)height / (WORLD_HEIGHT + HUD_HEIGHT);
	scale = xscale < yscale ? xscale : yscale;
	xoffset = (width - (TOTAL_WIDTH * scale)) / 2.0;
	yoffset = (height - ((WORLD_HEIGHT + HUD_HEIGHT) * scale)) / 2.0;
}

SDL_FRect game_to_window(SDL_FRect r)
{
	SDL_FRect ret;

	ret.w = r.w * scale;
	ret.h = r.h * scale;
	ret.x = xoffset + (r.x * scale);
	ret.y = yoffset + (r.y * scale);

	return ret;
}

void ren_rect_centered(SDL_FRect r, int red, int green, int blue, int fill)
{
	r = game_to_window(r);

	r.x -= r.w / 2;
	r.y -= r.h / 2;

	SDL_SetRenderDrawColor(g.ren, red, green, blue, 255);
	if (fill) {
		SDL_RenderFillRectF(g.ren, &r);
	} else {
		SDL_RenderDrawRectF(g.ren, &r);
	}
}

void ren_copy(SDL_Texture *t, SDL_Rect *src, SDL_FRect *dst)
{
	SDL_FRect dst_scaled;

	if (dst) {
		dst_scaled = game_to_window(*dst);
		SDL_RenderCopyF(g.ren, t, src, &dst_scaled); 
	} else {
		SDL_RenderCopyF(g.ren, t, src, dst); 
	}
}
