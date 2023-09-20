/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _CH_GAME_H
#define _CH_GAME_H

#include <SDL_render.h>

#include "block.h"
#include "gui/sprite.h"
#include "engine/types.h"
#include "engine/world.h"
#include "entity.h"

struct Config;

f32 now(void);

struct Game {
	char *world_name;
	SDL_Renderer *renderer;
	struct Config *cfg;

	struct String msg;
	int active;
	struct Sprite spr_blocks[B_LAST + 1];
	struct Sprite spr_walls[B_LAST + 1];
	struct Sprite spr_ents[E_LAST + 1];
	struct World world;
	SDL_Event event;
	const u8 *kbd;
	struct IPoint wld_draw_pts[2];
	SDL_Rect camera;
};

void Game_setup(struct Game *game);

void Game_run(struct Game *game);

void Game_edit(struct Game *game, const size_t width, const size_t height);

void Game_clear(struct Game *game);

#endif /* _CH_GAME_H */
