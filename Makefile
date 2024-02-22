CFLAGS   = -std=c99 -pedantic -Wall -O2
LDFLAGS  = $(LIBS) -lm -lSDL2 -lSDL2_image #-lSDL2_ttf #-lSDL2_image -lSDL2_mixer

CC = cc

SRC = main.c util.c sdl_util.c text.c menu.c play.c obj.c
OBJ = $(SRC:.c=.o)

all: pong

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OBJ): defs.h structs.h util.h sdl_util.h text.h menu.h play.h obj.h

pong: main.o util.o sdl_util.o text.o menu.o play.o obj.o
	$(CC) -o $@ main.o util.o sdl_util.o text.o menu.o play.o obj.o $(LDFLAGS)

clean:
	rm -f pong $(OBJ)

.PHONY: all options clean
