/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
