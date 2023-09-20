/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _CH_CONFIG_H
#define _CH_CONFIG_H

#include "engine/types.h"
#include <SDL.h>

static const char CFG_SETTING_GFX_WINDOW_X[] = "gfx_window_x";
static const char CFG_SETTING_GFX_WINDOW_Y[] = "gfx_window_y";
static const char CFG_SETTING_GFX_WINDOW_W[] = "gfx_window_w";
static const char CFG_SETTING_GFX_WINDOW_H[] = "gfx_window_h";
static const char CFG_SETTING_GFX_WINDOW_FULLSCREEN[] = "gfx_window_fullscreen";

static const u32 CFG_STD_GFX_WINDOW_X = SDL_WINDOWPOS_CENTERED;
static const u32 CFG_STD_GFX_WINDOW_Y = SDL_WINDOWPOS_CENTERED;
static const f32 CFG_STD_GFX_WINDOW_W = 640.0f;
static const f32 CFG_STD_GFX_WINDOW_H = 480.0f;
static const int CFG_STD_GFX_WINDOW_FULLSCREEN = 0;

struct Config {
	int invalid;
	i32 gfx_window_x;
	i32 gfx_window_y;
	i32 gfx_window_w;
	i32 gfx_window_h;
	int gfx_window_fullscreen;
};

struct Config Config_new(void);

void Config_from_file(struct Config *cfg);

void Config_to_file(struct Config *cfg);

#endif				/* _CH_CONFIG_H */
