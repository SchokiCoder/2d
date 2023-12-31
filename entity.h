/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _CH_ENTITY_H
#define _CH_ENTITY_H

#include "block.h"
#include "engine/entity.h"

static const f32 ENTITY_VELOCITY_THRESHOLD = 0.01f;
static const f32 ENTITY_GRAVITY = BLOCK_SIZE * 18;

enum Entities {
	E_NONE,
	E_PLAYER,

	E_LAST = E_PLAYER,
};

static const struct EntityData DATA_ENTITIES[] = {
	[0] = {
		.name = "none",
		.width = 0,
		.height = 0,
		.acceleration = 0,
		.max_velocity = 0,
		.decceleration = 0,
		.jump_velocity = 0,
	},

	[1] = {
		.name = "player",
		.width = (BLOCK_SIZE * 2) - 6,
		.height = (BLOCK_SIZE * 3) - 8,
		.acceleration = BLOCK_SIZE * 12,
		.max_velocity = BLOCK_SIZE * 6,
		.decceleration = BLOCK_SIZE * 6,
		.jump_velocity = BLOCK_SIZE * 12,
	}
};

void Entity_move_x(struct Entity *ent, f32 x_distance, struct World *world);

void Entity_move_y(struct Entity *ent, f32 y_distance, struct World *world);

#endif				/* _ENTITY_H */
