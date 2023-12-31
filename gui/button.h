/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _BUTTON_H
#define _BUTTON_H

#include "engine/sstring.h"
#include "engine/types.h"
#include "sprite.h"

struct Theme;
struct Menu;

#define BUTTON_TEXT_INIT_SIZE 32

struct ButtonStyle {
	SDL_Color font_color;
	SDL_Color bg_color;
	SDL_Color border_color;
	SDL_Color disabled_color;
};

struct Button {
	struct Menu *menu;
	TTF_Font *font;
	int visible;
	int active;
	struct String text;
	struct Sprite sprite;

	SDL_Rect rect;
	struct ButtonStyle style;

	void (*func_click)(void *);
	void *data_click;
};

void Button_new(struct Button            *button,
		struct Menu              *menu,
		TTF_Font                 *font,
		const struct ButtonStyle  style);

void Button_update_sprite(struct Button *button);

void Button_draw(struct Button *button);

void Button_resize(struct Button *button);

#endif				/* _BUTTON_H */
