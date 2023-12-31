/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _MENU_H
#define _MENU_H

#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#include "engine/types.h"

struct Theme;
struct Label;
struct Button;
struct Entry;

#define MENU_MAX_WIDGETS 16	/* per type */

struct MenuStyle {
	SDL_Color bg_color;
};

struct Menu {
	SDL_Renderer *renderer;

	int visible;
	int active;

	u8 label_count;
	struct Label *labels[MENU_MAX_WIDGETS];

	u8 button_count;
	struct Button *buttons[MENU_MAX_WIDGETS];

	u8 entry_count;
	struct Entry *entries[MENU_MAX_WIDGETS];
	struct Entry *focused_entry;

	SDL_Rect rect;
	struct MenuStyle style;
};

struct Menu Menu_new(SDL_Renderer *renderer, const struct MenuStyle style);

void Menu_draw(struct Menu *menu);

void Menu_handle_event(struct Menu *menu, SDL_Event *event);

void Menu_grid(struct Menu *menu);

void Menu_clear(struct Menu *menu);

#endif				/* _MENU_H */
