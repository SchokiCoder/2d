/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _ENTRY_H
#define _ENTRY_H

#include "sprite.h"
#include "engine/sstring.h"
#include "engine/types.h"

struct Theme;
struct Menu;

#define ENTRY_TEXT_INIT_SIZE 32

struct EntryStyle {
	SDL_Color font_color;
	SDL_Color bg_color;
	SDL_Color border_color;
	SDL_Color disabled_color;
};

struct Entry {
	struct Menu *menu;
	TTF_Font *font;
	int visible;
	int active;
	struct String text;
	struct Sprite *sprites;

	SDL_Rect rect;
	struct EntryStyle style;
};

void Entry_new(struct Entry            *entry,
               struct Menu             *menu,
               TTF_Font                *font,
	       const struct EntryStyle  style);

void Entry_update_sprite(struct Entry *entry, size_t pos);

void Entry_update_sprites(struct Entry *entry);

void Entry_append(struct Entry *entry, struct String *appendage);

void Entry_draw(struct Entry *entry);

void Entry_clear_sprites(struct Entry *entry);

void Entry_resize(struct Entry *entry);

#endif				/* _ENTRY_H */
