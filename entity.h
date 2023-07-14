/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <SG_entity.h>
#include "block.h"

static const float ENTITY_VELOCITY_THRESHOLD = 0.01f;
static const float ENTITY_GRAVITY = BLOCK_SIZE * 18;

typedef enum Entity {
	E_NONE,
	E_PLAYER,

	E_LAST = E_PLAYER,
} Entity;

static const SG_EntityData DATA_ENTITIES[] = {
	{
	 .name = "none",
	 .width = 0,
	 .height = 0,
	 .acceleration = 0,
	 .max_velocity = 0,
	 .decceleration = 0,
	 .jump_velocity = 0,
	 },

	{
	 .name = "player",
	 .width = (BLOCK_SIZE * 2) - 6,
	 .height = (BLOCK_SIZE * 3) - 8,
	 .acceleration = BLOCK_SIZE * 12,
	 .max_velocity = BLOCK_SIZE * 6,
	 .decceleration = BLOCK_SIZE * 6,
	 .jump_velocity = BLOCK_SIZE * 12,
	 },
};

void Entity_move_x(SG_Entity * ent, float x_distance, SG_World * world);

void Entity_move_y(SG_Entity * ent, float y_distance, SG_World * world);

#endif				// ENTITY_H
