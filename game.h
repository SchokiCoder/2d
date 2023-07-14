/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GAME_H
#define GAME_H

#include <SG_types.h>
#include <SDL_render.h>
#include "entity.h"
#include "block.h"

typedef struct Config Config;

float now(void);

typedef struct Game {
	char *world_name;
	SDL_Renderer *renderer;
	Config *cfg;

	SM_String msg;
	bool active;
	SGUI_Sprite spr_blocks[B_LAST + 1];
	SGUI_Sprite spr_walls[B_LAST + 1];
	SGUI_Sprite spr_ents[E_LAST + 1];
	SG_World world;
	SDL_Event event;
	const uint8_t *kbd;
	SG_IPoint wld_draw_pts[2];
	SDL_Rect camera;
} Game;

void Game_setup(Game * game);

void Game_run(Game * game);

void Game_edit(Game * game, const size_t width, const size_t height);

void Game_clear(Game * game);

#endif
