/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _WORLD_H
#define _WORLD_H

#include <SDL_render.h>

#include "sstring.h"
#include "types.h"

struct Entity Entity;

struct World {
	int invalid;

	u32 block_size;
	u32 width;
	u32 height;
	u32 depth;
	u32 ***blocks;
	SDL_Texture ****block_textures;

	u32 ent_count;
	struct Entity *entities;
};

struct World World_new(const u32 block_size,
		       const u32 width,
		       const u32 height, const u32 depth);

struct World World_from_file(const char *filepath);

void World_write(struct World *world, const char *filepath);

void World_clear(struct World *world);

#endif				/* _WORLD_H */
