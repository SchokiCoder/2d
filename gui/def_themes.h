/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef SGUI_DEF_THEMES_H
#define SGUI_DEF_THEMES_H

#include "SGUI_theme.h"

static const SGUI_Theme SGUI_THEME_LIGHT = {
	.menu = {
		 .bg_color = {.r = 200,.g = 200,.b = 200,.a = 255},
		 },

	.label = {
		  .font_color = {.r = 50,.g = 50,.b = 50,.a = 255},
		  .bg_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		  },

	.button = {
		   .font_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		   .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
		   .border_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		   .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
		   },

	.entry = {
		  .font_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .bg_color = {.r = 240,.g = 240,.b = 240,.a = 255},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
		  },
};

static const SGUI_Theme SGUI_THEME_DARK = {
	.menu = {
		 .bg_color = {.r = 30,.g = 30,.b = 30,.a = 255},
		 },

	.label = {
		  .font_color = {.r = 200,.g = 200,.b = 200,.a = 255},
		  .bg_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		  },

	.button = {
		   .font_color = {.r = 200,.g = 200,.b = 200,.a = 255},
		   .bg_color = {.r = 50,.g = 50,.b = 50,.a = 255},
		   .border_color = {.r = 70,.g = 70,.b = 70,.a = 255},
		   .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 75},
		   },

	.entry = {
		  .font_color = {.r = 200,.g = 200,.b = 200,.a = 255},
		  .bg_color = {.r = 65,.g = 65,.b = 65,.a = 255},
		  .border_color = {.r = 70,.g = 70,.b = 70,.a = 255},
		  .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 75},
		  },
};

#endif				/* SGUI_DEF_THEMES_H */
