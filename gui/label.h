/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SGUI_LABEL_H
#define SGUI_LABEL_H

#include <SM_types.h>
#include <SM_string.h>
#include "SGUI_sprite.h"

typedef struct SGUI_Theme SGUI_Theme;
typedef struct SGUI_Menu SGUI_Menu;

#define SGUI_LABEL_TEXT_INIT_SIZE 32

typedef struct SGUI_LabelStyle {
	SDL_Color font_color;
	SDL_Color bg_color;
	SDL_Color border_color;
} SGUI_LabelStyle;

typedef struct SGUI_Label {
	SGUI_Menu *menu;
	TTF_Font *font;
	bool visible;
	SM_String text;
	SGUI_Sprite sprite;

	SDL_Rect rect;
	SGUI_LabelStyle style;
} SGUI_Label;

void SGUI_Label_new(SGUI_Label * label,
		    SGUI_Menu * menu,
		    TTF_Font * font, const SGUI_LabelStyle style);

void SGUI_Label_update_sprite(SGUI_Label * label);

void SGUI_Label_draw(SGUI_Label * label);

void SGUI_Label_resize(SGUI_Label * label);

#endif				/* SGUI_LABEL_H */
