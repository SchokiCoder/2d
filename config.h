/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

static const char CFG_SETTING_GFX_WINDOW_X[] = "gfx_window_x";
static const char CFG_SETTING_GFX_WINDOW_Y[] = "gfx_window_y";
static const char CFG_SETTING_GFX_WINDOW_W[] = "gfx_window_w";
static const char CFG_SETTING_GFX_WINDOW_H[] = "gfx_window_h";
static const char CFG_SETTING_GFX_WINDOW_FULLSCREEN[] = "gfx_window_fullscreen";

static const uint32_t CFG_STD_GFX_WINDOW_X = SDL_WINDOWPOS_CENTERED;
static const uint32_t CFG_STD_GFX_WINDOW_Y = SDL_WINDOWPOS_CENTERED;
static const float CFG_STD_GFX_WINDOW_W = 640.0f;
static const float CFG_STD_GFX_WINDOW_H = 480.0f;
static const bool CFG_STD_GFX_WINDOW_FULLSCREEN = false;

typedef struct Config {
	bool invalid;
	int32_t gfx_window_x;
	int32_t gfx_window_y;
	int32_t gfx_window_w;
	int32_t gfx_window_h;
	bool gfx_window_fullscreen;
} Config;

Config Config_new(void);

void Config_load(Config * cfg);

void Config_save(Config * cfg);

#endif				// CONFIG_H
