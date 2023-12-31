/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _CH_WORLD_H
#define _CH_WORLD_H

#include "engine/world.h"

struct World Ch_World_new(const size_t width, const size_t height);

struct World Ch_World_from_file(const char *world_name);

void Ch_World_to_file(struct World *world, const char *world_name);

#endif				/* _CH_WORLD_H */
