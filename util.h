#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);
double rand_norm(void);
int rand_range(int s, int e);
int aabb_d(
		double x1, double y1, double w1, double h1,
		double x2, double y2, double w2, double h2);

#endif
