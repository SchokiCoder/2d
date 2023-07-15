/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _CH_WORLD_H
#define _CH_WORLD_H

#include "engine/world.h"

struct World Ch_World_new(const size_t width, const size_t height);

struct World Ch_World_from_file(const char *world_name);

void Ch_World_to_file(struct World *world, const char *world_name);

#endif				/* _CH_WORLD_H */
