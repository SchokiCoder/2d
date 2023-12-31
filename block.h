/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _CH_BLOCK_H
#define _CH_BLOCK_H

#include "engine/types.h"

static const i32 BLOCK_SIZE = 32;

enum Blocks {
	B_NONE,
	B_DIRT,
	B_STONE,

	B_FIRST = B_NONE,
	B_LAST = B_STONE
};

#endif				/* _CH_BLOCK_H */
