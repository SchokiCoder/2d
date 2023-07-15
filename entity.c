/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "engine/world.h"
#include "engine/physics.h"
#include "entity.h"

/*
	player_pos: 		player.rect.x or y
	player_velocity:	player.velocity_x or y
*/
int
Entity_move(struct Entity *ent,
	    f32 *pos,
	    f32 *velocity,
	    f32 distance,
	    struct World *world)
{
	int collision = 0;
	i32 x1, y1, x2, y2;
	struct FRect block_hitbox;

	// set position
	*pos += distance;

	// calculate which blocks to check
	x1 = ent->rect.x / BLOCK_SIZE;
	y1 = ent->rect.y / BLOCK_SIZE;
	x2 = (ent->rect.x + ent->rect.w) / BLOCK_SIZE + 1;
	y2 = (ent->rect.y + ent->rect.h) / BLOCK_SIZE + 1;

	// check if within bounds
	if (x1 < 0)
		x1 = 0;

	if (x1 > (i32) world->width - 1)
		x1 = (i32) world->width - 1;

	if (y1 < 0)
		y1 = 0;

	if (y1 > (i32) world->height - 1)
		y1 = (i32) world->height - 1;

	if (x2 < 0)
		x2 = 0;

	if (x2 > (i32) world->width - 1)
		x2 = (i32) world->width - 1;

	if (y2 < 0)
		y2 = 0;

	if (y2 > (i32) world->height - 1)
		y2 = (i32) world->height - 1;

	// collision check for nearby blocks
	block_hitbox.w = BLOCK_SIZE;
	block_hitbox.h = BLOCK_SIZE;

	for (i32 x = x1; x <= x2; x++) {
		for (i32 y = y1; y <= y2; y++) {
			// if non-solid block here, skip
			if (world->blocks[x][y][0] == B_NONE)
				continue;

			block_hitbox.x = x * BLOCK_SIZE;
			block_hitbox.y = y * BLOCK_SIZE;

			// if collision
			if (frect_within_frect(&ent->rect, &block_hitbox)) {
				// flag, reset pos, kill velocity
				collision = 1;
				*pos -= distance;
				*velocity = 0.0f;
			}
		}
	}

	return collision;
}

void Entity_move_x(struct Entity *ent, f32 x_distance, struct World *world)
{
	Entity_move(ent, &ent->rect.x, &ent->velocity_x, x_distance, world);
}

void Entity_move_y(struct Entity *ent, f32 y_distance, struct World *world)
{
	int collision;

	// move
	collision =
	    Entity_move(ent, &ent->rect.y, &ent->velocity_y, y_distance, world);

	// if falling and collision happened, set grounded, else set non-grounded
	if (y_distance > 0.0f && collision)
		ent->grounded = 1;
	else
		ent->grounded = 0;
}
