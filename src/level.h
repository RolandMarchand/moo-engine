#pragma once

#include "common.h"
#include "u_math.h"

#define INVALID_SECTOR (sector_idx_t)(-1)

typedef size_t wall_idx_t;
typedef size_t sector_idx_t;

struct wall {
	line line;
	sector_idx_t next_sector;
};

struct sector {
	wall_idx_t wall_start;
	size_t wall_count;
};

void update_sector(vector updated_pos);
bool inside(sector_idx_t idx, vector point);
void terminate_level();
void initialize_level(
	struct wall *p_walls, struct sector *p_sectors,
	size_t wall_cnt, size_t sector_cnt
	);
