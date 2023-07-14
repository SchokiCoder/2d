/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SGUI_MENU_H
#define SGUI_MENU_H

#include <stdbool.h>
#include <SM_types.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_pixels.h>

typedef struct SGUI_Theme SGUI_Theme;
typedef struct SGUI_Label SGUI_Label;
typedef struct SGUI_Button SGUI_Button;
typedef struct SGUI_Entry SGUI_Entry;

#define SGUI_MENU_MAX_WIDGETS 16	/* per type */

typedef struct SGUI_MenuStyle {
	SDL_Color bg_color;
} SGUI_MenuStyle;

typedef struct SGUI_Menu {
	SDL_Renderer *renderer;

	bool visible;
	bool active;

	u8 label_count;
	SGUI_Label *labels[SGUI_MENU_MAX_WIDGETS];

	u8 button_count;
	SGUI_Button *buttons[SGUI_MENU_MAX_WIDGETS];

	u8 entry_count;
	SGUI_Entry *entries[SGUI_MENU_MAX_WIDGETS];
	SGUI_Entry *focused_entry;

	SDL_Rect rect;
	SGUI_MenuStyle style;
} SGUI_Menu;

SGUI_Menu SGUI_Menu_new(SDL_Renderer * renderer, const SGUI_MenuStyle style);

void SGUI_Menu_draw(SGUI_Menu * menu);

void SGUI_Menu_handle_event(SGUI_Menu * menu, SDL_Event * event);

void SGUI_Menu_grid(SGUI_Menu * menu);

void SGUI_Menu_clear(SGUI_Menu * menu);

#endif				/* SGUI_MENU_H */
