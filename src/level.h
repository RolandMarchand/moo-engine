#pragma once

#include "common.h"

typedef struct {
	line line;
	int portal;
} wall;

typedef struct {
	int wall_start;
	int wall_count;
} sector;

bool inside(wall *walls, sector s, vector point);
