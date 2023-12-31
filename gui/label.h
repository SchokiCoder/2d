/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _LABEL_H
#define _LABEL_H

#include "engine/sstring.h"
#include "engine/types.h"
#include "sprite.h"

struct Theme;
struct Menu;

#define LABEL_TEXT_INIT_SIZE 32

struct LabelStyle {
	SDL_Color font_color;
	SDL_Color bg_color;
	SDL_Color border_color;
};

struct Label {
	struct Menu *menu;
	TTF_Font *font;
	int visible;
	struct String text;
	struct Sprite sprite;

	SDL_Rect rect;
	struct LabelStyle style;
};

void Label_new(struct Label            *label,
	       struct Menu             *menu,
	       TTF_Font                *font,
	       const struct LabelStyle  style);

void Label_update_sprite(struct Label *label);

void Label_draw(struct Label *label);

void Label_resize(struct Label *label);

#endif				/* _LABEL_H */
