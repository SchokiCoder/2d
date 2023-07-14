/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SGUI_BUTTON_H
#define SGUI_BUTTON_H

#include <SM_types.h>
#include <SM_string.h>
#include "SGUI_sprite.h"

typedef struct SGUI_Theme SGUI_Theme;
typedef struct SGUI_Menu SGUI_Menu;

#define SGUI_BUTTON_TEXT_INIT_SIZE 32

typedef struct SGUI_ButtonStyle {
	SDL_Color font_color;
	SDL_Color bg_color;
	SDL_Color border_color;
	SDL_Color disabled_color;
} SGUI_ButtonStyle;

typedef struct SGUI_Button {
	SGUI_Menu *menu;
	TTF_Font *font;
	bool visible;
	bool active;
	SM_String text;
	SGUI_Sprite sprite;

	SDL_Rect rect;
	SGUI_ButtonStyle style;

	void (*func_click)(void *);
	void *data_click;
} SGUI_Button;

void SGUI_Button_new(SGUI_Button * button, SGUI_Menu * menu, TTF_Font * font,
		     const SGUI_ButtonStyle style);

void SGUI_Button_update_sprite(SGUI_Button * button);

void SGUI_Button_draw(SGUI_Button * button);

void SGUI_Button_resize(SGUI_Button * button);

#endif				/* SGUI_BUTTON_H */
