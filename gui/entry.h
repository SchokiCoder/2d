/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SGUI_ENTRY_H
#define SGUI_ENTRY_H

#include <stdbool.h>
#include <SM_types.h>
#include <SM_string.h>
#include "SGUI_sprite.h"

typedef struct SGUI_Theme SGUI_Theme;
typedef struct SGUI_Menu SGUI_Menu;

#define SGUI_ENTRY_TEXT_INIT_SIZE 32

typedef struct SGUI_EntryStyle {
	SDL_Color font_color;
	SDL_Color bg_color;
	SDL_Color border_color;
	SDL_Color disabled_color;
} SGUI_EntryStyle;

typedef struct SGUI_Entry {
	SGUI_Menu *menu;
	TTF_Font *font;
	bool visible;
	bool active;
	SM_String text;
	SGUI_Sprite *sprites;

	SDL_Rect rect;
	SGUI_EntryStyle style;
} SGUI_Entry;

void SGUI_Entry_new(SGUI_Entry * entry, SGUI_Menu * menu, TTF_Font * font,
		    const SGUI_EntryStyle style);

void SGUI_Entry_update_sprite(SGUI_Entry * entry, size_t pos);

void SGUI_Entry_update_sprites(SGUI_Entry * entry);

void SGUI_Entry_append(SGUI_Entry * entry, SM_String * appendage);

void SGUI_Entry_draw(SGUI_Entry * entry);

void SGUI_Entry_clear_sprites(SGUI_Entry * entry);

void SGUI_Entry_resize(SGUI_Entry * entry);

#endif				/* SGUI_ENTRY_H */
