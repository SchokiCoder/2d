/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "world.h"

void World_allocate(struct World *world)
{
	world->blocks = malloc(world->width *sizeof(u32**));
	world->block_textures = malloc(world->width *sizeof(SDL_Texture***));

	for (size_t x = 0; x < world->width; x++) {
		world->blocks[x] = malloc(world->height *sizeof(u32*));
		world->block_textures[x] =
		    malloc(world->height *sizeof(SDL_Texture**));

		for (size_t y = 0; y < world->height; y++) {
			world->blocks[x][y] =
			    malloc(world->depth *sizeof(u32));
			world->block_textures[x][y] =
			    malloc(world->depth *sizeof(SDL_Texture*));
		}
	}

	world->entities = malloc(world->ent_count *sizeof(struct Entity));
}

struct World World_new(const u32 block_size,
		       const u32 width,
		       const u32 height, const u32 depth)
{
	struct World world = {
		.invalid = 0,
		.block_size = block_size,
		.width = width,
		.height = height,
		.depth = depth,
		.ent_count = 1,
	};

	// allocate
	World_allocate(&world);

	return world;
}

struct World World_from_file(const char *filepath)
{
	struct World world = {
		.invalid = 0,
	};
	FILE *f;

	// open file
	f = fopen(filepath, "r");

	if (f == NULL) {
		fprintf(stderr, "World file \"%s\" not found\n", filepath);
		world.invalid = 1;
		return world;
	}
	// read header
	fread(&world.block_size, sizeof(world.block_size), 1, f);
	fread(&world.width, sizeof(world.width), 1, f);
	fread(&world.height, sizeof(world.height), 1, f);
	fread(&world.depth, sizeof(world.depth), 1, f);
	fread(&world.ent_count, sizeof(world.ent_count), 1, f);

	// allocate for blocks
	World_allocate(&world);

	// read blocks
	for (u32 x = 0; x < world.width; x++)
		for (u32 y = 0; y < world.height; y++)
			for (u32 z = 0; z < world.depth; z++)
				fread(&world.blocks[x][y][z],
				      sizeof(world.blocks[x][y][z]), 1, f);

	// read entitites
	for (size_t i = 0; i < world.ent_count; i++) {
		fread(&world.entities[i].id, sizeof(world.entities[i].id), 1,
		      f);
		fread(&world.entities[i].rect.x,
		      sizeof(world.entities[i].rect.x), 1, f);
		fread(&world.entities[i].rect.y,
		      sizeof(world.entities[i].rect.y), 1, f);
		fread(&world.entities[i].rect.w,
		      sizeof(world.entities[i].rect.w), 1, f);
		fread(&world.entities[i].rect.h,
		      sizeof(world.entities[i].rect.h), 1, f);
		fread(&world.entities[i].grounded,
		      sizeof(world.entities[i].grounded), 1, f);
		fread(&world.entities[i].velocity_x,
		      sizeof(world.entities[i].velocity_x), 1, f);
		fread(&world.entities[i].velocity_y,
		      sizeof(world.entities[i].velocity_y), 1, f);
	}

	fclose(f);

	return world;
}

void World_to_file(struct World *world, const char *filepath)
{
	FILE *f;

	// open file
	f = fopen(filepath, "w");

	// if file did not open, set flag and stop
	if (f == NULL) {
		world->invalid = 1;
		return;
	}
	// write header
	fwrite(&world->block_size, sizeof(world->block_size), 1, f);
	fwrite(&world->width, sizeof(world->width), 1, f);
	fwrite(&world->height, sizeof(world->height), 1, f);
	fwrite(&world->depth, sizeof(world->depth), 1, f);
	fwrite(&world->ent_count, sizeof(world->ent_count), 1, f);

	// write blocks
	for (u32 x = 0; x < world->width; x++)
		for (u32 y = 0; y < world->height; y++)
			for (u32 z = 0; z < world->depth; z++)
				fwrite(&world->blocks[x][y][z],
				       sizeof(world->blocks[x][y][z]), 1, f);

	// write entities
	for (size_t i = 0; i < world->ent_count; i++) {
		fwrite(&world->entities[i].id, sizeof(world->entities[i].id), 1,
		       f);
		fwrite(&world->entities[i].rect.x,
		       sizeof(world->entities[i].rect.x), 1, f);
		fwrite(&world->entities[i].rect.y,
		       sizeof(world->entities[i].rect.y), 1, f);
		fwrite(&world->entities[i].rect.w,
		       sizeof(world->entities[i].rect.w), 1, f);
		fwrite(&world->entities[i].rect.h,
		       sizeof(world->entities[i].rect.h), 1, f);
		fwrite(&world->entities[i].grounded,
		       sizeof(world->entities[i].grounded), 1, f);
		fwrite(&world->entities[i].velocity_x,
		       sizeof(world->entities[i].velocity_x), 1, f);
		fwrite(&world->entities[i].velocity_y,
		       sizeof(world->entities[i].velocity_y), 1, f);
	}

	fclose(f);
}

void World_clear(struct World *world)
{
	for (u32 i = 0; i < world->width; i++) {
		free(world->blocks[i]);
		free(world->block_textures[i]);
	}

	free(world->blocks);
	free(world->block_textures);

	free(world->entities);
}
