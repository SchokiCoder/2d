/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <stdint.h>
#include <stdio.h>

#include "engine/log.h"
#include "entity.h"
#include "path.h"
#include "world.h"

struct World Ch_World_new(const size_t width, const size_t height)
{
	struct World world = World_new(BLOCK_SIZE, width, height, 2);

	// set values
	world.entities[0].id = E_PLAYER;
	world.entities[0].rect.x = 0.0f;
	world.entities[0].rect.y = 0.0f;
	world.entities[0].rect.w = DATA_ENTITIES[E_PLAYER].width;
	world.entities[0].rect.h = DATA_ENTITIES[E_PLAYER].height;
	world.entities[0].grounded = 0;
	world.entities[0].velocity_x = 0.0f;
	world.entities[0].velocity_y = 0.0f;

	return world;
}

struct World Ch_World_from_file(const char *world_name)
{
	const char *temp;
	struct World world;
	struct String filepath = String_new(8);

	// get path
	if (get_world_path(&filepath) != 0) {
		world.invalid = 1;
		return world;
	}

	String_append(&filepath, world_name, strlen(world_name));
	temp = ".";
	String_append(&filepath, temp, strlen(temp));
	String_append(&filepath, FILETYPE_WORLD, strlen(FILETYPE_WORLD));

	// read
	world = World_from_file(filepath.str);

	if (world.invalid) {
		struct String msg = String_new(16);
		temp = "World \"";
		String_copy(&msg, temp, strlen(temp));
		String_append(&msg, world_name, strlen(world_name));
		temp = "\" could not be read.";
		String_append(&msg, temp, strlen(temp));
		log_err(msg.str);
		String_clear(&msg);
	}

	String_clear(&filepath);
	return world;
}

void Ch_World_to_file(struct World *world, const char *world_name)
{
	const char *temp;
	struct String filepath = String_new(8);

	// get path
	if (get_world_path(&filepath) != 0) {
		world->invalid = 1;
		return;
	}

	String_append(&filepath, world_name, strlen(world_name));
	temp = ".";
	String_append(&filepath, temp, strlen(temp));
	String_append(&filepath, FILETYPE_WORLD, strlen(FILETYPE_WORLD));

	// write
	Ch_World_to_file(world, filepath.str);

	if (world->invalid) {
		struct String msg = String_new(16);
		temp = "World \"";
		String_copy(&msg, temp, strlen(temp));
		String_append(&msg, world_name, strlen(world_name));
		temp = "\" could not be read.";
		String_append(&msg, temp, strlen(temp));
		log_err(msg.str);
		String_clear(&msg);
	}

	String_clear(&filepath);
}
