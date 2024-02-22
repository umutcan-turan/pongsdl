#ifndef OBJ_H
#define OBJ_H

#include <SDL2/SDL.h>
#include "structs.h"

obj obj_new(double x, double y, double w, double h, double dx, double dy);
void obj_update(obj *o, double dt);
void obj_setpos(obj *o, double x, double y);
void obj_setdelta(obj *o, double dx, double dy);
void obj_setsize(obj *o, double w, double h);
int obj_x_min(obj *o, double x);
int obj_x_max(obj *o, double x);
int obj_y_min(obj *o, double y);
int obj_y_max(obj *o, double y);
int obj_aabb(obj *a, obj *b);
void obj_rendercopy(obj *o, SDL_Texture *tex, SDL_Rect *src);

#endif
