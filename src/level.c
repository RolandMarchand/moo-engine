#include "level.h"

struct wall *Walls;
struct sector *Sectors;
size_t WallCount;
size_t SectorCount;

sector_idx_t LastKnownSector = INVALID_SECTOR;

void initialize_level(
	struct wall *p_walls, struct sector *p_sectors,
	size_t wall_cnt, size_t sector_cnt)
{
	terminate_level();
	WallCount = wall_cnt;
	SectorCount = sector_cnt;
	Walls = malloc(sizeof(struct wall) * wall_cnt);
	Sectors = malloc(sizeof(struct sector) * sector_cnt);
	memcpy(Walls, p_walls, sizeof(struct wall) * wall_cnt);
	memcpy(Sectors, p_sectors, sizeof(struct sector) * sector_cnt);
}

void terminate_level()
{
	if (Walls) {
		free(Walls);
		Walls = NULL;
	}
	if (Sectors){
		free(Sectors);
		Sectors = NULL;
	}
	WallCount = 0;
	SectorCount = 0;
}

void update_sector(vector updated_pos)
{
	if (LastKnownSector == INVALID_SECTOR) {
		goto scan_all;
	}

	if (inside(LastKnownSector, updated_pos)) {
		return;
	}

	/* TODO: Optimize so that the same sector isn't checked more
	 * than once */
	/* Scan neighboring sectors */
	struct sector last_sector = Sectors[LastKnownSector];
	for (wall_idx_t idx = 0; idx < last_sector.wall_count; idx++) {
		struct wall wall = Walls[last_sector.wall_start + idx];
		sector_idx_t next_sector = wall.next_sector;
		if (next_sector == INVALID_SECTOR) {
			continue;
		}
		if (inside(next_sector, updated_pos)) {
			LastKnownSector = next_sector;
			return;
		}
	}

  scan_all:
	for (size_t sector_idx = 0; sector_idx < SectorCount; sector_idx++) {
		if (inside(sector_idx, updated_pos)) {
			LastKnownSector = sector_idx;
			return;
		}
	}
	LastKnownSector = INVALID_SECTOR;
}

bool inside(sector_idx_t idx, vector point)
{
	if (idx == INVALID_SECTOR) {
		return false;
	}

	/* TODO: check if sector is valid */
	int intersection_count = 0;
	struct sector s = Sectors[idx];
	for (size_t i = 0; i < s.wall_count; i++) {
		line wall = Walls[s.wall_start + i].line;

		double min_x = wall.a.x < wall.b.x ? wall.a.x : wall.b.x;
		vector crossed = lines_intersect_at(
			wall, Linev(Vector(min_x - 1, point.y), point));

		if (VectorIsValid(crossed)) {
			if (wall.a.y < point.y || wall.b.y < point.y) {
				intersection_count++;
			}
		}
	}
	return intersection_count % 2 == 1;
}
