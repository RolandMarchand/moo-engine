#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "SDL2/SDL_stdinc.h"

#include "config.h"

enum Error {
	OK = 0,
	ERROR
};

#define FOV 90
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define FORMAT SDL_PIXELFORMAT_RGB888
