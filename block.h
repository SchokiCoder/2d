/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

static const int_fast32_t BLOCK_SIZE = 32;

typedef enum Block {
	B_NONE,
	B_DIRT,
	B_STONE,

	B_FIRST = B_NONE,
	B_LAST = B_STONE,
} Block;

#endif				/* BLOCK_H */
