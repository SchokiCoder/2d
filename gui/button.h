/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
