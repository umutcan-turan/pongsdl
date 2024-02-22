#include "defs.h"
#include "structs.h"
#include "sdl_util.h"
#include "text.h"

#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern game g;

static SDL_Texture *bitmap;
static int charwidth;
static int charheight;

void text_init(void)
{
	bitmap = IMG_LoadTexture(g.ren, "./asset/digifont_16x16.png");
	charwidth = 16;
	charheight = 16;
}

void text_draw(int x, int y, const char *txt, double mult)
{
	int c;
	SDL_FRect dst;
	SDL_Rect src;

	dst.w = mult;
	dst.h = mult;
	dst.x = x - (strlen(txt) / 2.0) * mult;
	dst.y = y - dst.h / 2;

	src.w = charwidth;
	src.h = charheight;

	while ((c = *txt++)) {
		if (c >= 'a' && c <= 'z') {
			c -= 32;
		}
		if (c < ' ' || c > 'Z') {
			c = '?';
		}
		src.x = (c - ' ') * charwidth;
		src.y = 0;
		ren_copy(bitmap, &src, &dst);
		dst.x += mult;
	}
}

void text_cleanup(void)
{
	SDL_DestroyTexture(bitmap);
	bitmap = NULL;
}
