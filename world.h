/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef WORLD_H
#define WORLD_H

#include <SG_world.h>

SG_World World_new(const size_t width, const size_t height);

SG_World World_from_file(const char *world_name);

void World_write(SG_World * world, const char *world_name);

#endif				// WORLD_H
