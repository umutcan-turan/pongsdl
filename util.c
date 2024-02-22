#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "util.h"

void die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
	exit(1);
}

void *ecalloc(size_t nmemb, size_t size)
{
	void *p;

	p = calloc(nmemb, size);
	if (!p) {
		die("calloc:");
	}
	return p;
}

double rand_norm(void)
{
	static int init = 0;

	if (!init) {
		init = 1;
		srand(time(NULL));
	}
	return (double)rand() * (1.0 / ((double)RAND_MAX + 1.0));
}

int rand_range(int s, int e)
{
	double r;

	if (s > e) {
		return 0;
	}
	r = rand_norm();
	r *= (double)(e- s) + 1.0;
	return (int)r + s;
}

int aabb_d(
		double x1, double y1, double w1, double h1,
		double x2, double y2, double w2, double h2)
{
	int xcollision = (x1 < x2 + w2) && (x2 < x1 + w1);
	int ycollision = (y1 < y2 + h2) && (y2 < y1 + h1);

	return xcollision && ycollision;
}
