#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include <SDL2/SDL.h>

int sdl_init(void);
void sdl_cleanup(void);
void update_resolution(void);
SDL_FRect game_to_window(SDL_FRect r);
void ren_rect_centered(SDL_FRect r, int red, int green, int blue, int fill);
void ren_copy(SDL_Texture *t, SDL_Rect *src, SDL_FRect *dst);

#endif
