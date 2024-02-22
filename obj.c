#include "defs.h"
#include "defs.h"
#include "util.h"
#include "sdl_util.h"
#include "obj.h"

obj obj_new(double x, double y, double w, double h, double dx, double dy)
{
	obj o;

	o.body.x = x;
	o.body.y = y;
	o.body.w = w;
	o.body.h = h;
	o.dx = dx;
	o.dy = dy;

	return o;
}

void obj_update(obj *o, double dt)
{
	o->body.x += o->dx * dt;
	o->body.y += o->dy * dt;
}

void obj_setpos(obj *o, double x, double y)
{
	o->body.x = x;
	o->body.y = y;
}

void obj_setdelta(obj *o, double dx, double dy)
{
	o->dx = dx;
	o->dy = dy;
}

void obj_setsize(obj *o, double w, double h)
{
	o->body.w = w;
	o->body.h = h;
}

int obj_x_min(obj *o, double x)
{
	if ((o->body.x - o->body.w / 2) < x) {
		o->body.x = x + o->body.w / 2;
		return 1;
	}
	return 0;
}

int obj_x_max(obj *o, double x)
{
	if ((o->body.x + o->body.w / 2) > x) {
		o->body.x = x - o->body.w / 2;
		return 1;
	}
	return 0;
}

int obj_y_min(obj *o, double y)
{
	if ((o->body.y - o->body.h / 2) < y) {
		o->body.y = y + o->body.h / 2;
		return 1;
	}
	return 0;
}

int obj_y_max(obj *o, double y)
{
	if ((o->body.y + o->body.h / 2) > y) {
		o->body.y = y - o->body.h / 2;
		return 1;
	}
	return 0;
}


int obj_aabb(obj *a, obj *b)
{
	return aabb_d(
			a->body.x - a->body.w / 2,
			a->body.y - a->body.h / 2,
			a->body.w, a->body.h,
			b->body.x - b->body.w / 2,
			b->body.y - b->body.h / 2,
			b->body.w, b->body.h);
}

void obj_rendercopy(obj *o, SDL_Texture *tex, SDL_Rect *src)
{
	SDL_FRect dst;

	dst.x = o->body.x - o->body.w / 2;
	dst.y = o->body.y - o->body.h / 2;
	dst.w = o->body.w;
	dst.h = o->body.h;

	ren_copy(tex, src, &dst);
}
