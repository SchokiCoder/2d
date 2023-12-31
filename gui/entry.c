/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include "entry.h"
#include "menu.h"
#include "theme.h"

void Entry_new(struct Entry             *entry,
               struct Menu              *menu,
               TTF_Font                 *font,
	       const struct EntryStyle  style)
{
	entry->menu = menu;
	entry->font = font;

	entry->sprites = malloc(ENTRY_TEXT_INIT_SIZE *sizeof(struct Sprite));

	for (size_t i = 0; i < ENTRY_TEXT_INIT_SIZE; i++)
		entry->sprites[i] = Sprite_new();

	entry->text = String_new(ENTRY_TEXT_INIT_SIZE);
	entry->visible = true;
	entry->active = true;
	entry->style = style;

	menu->entries[menu->entry_count] = entry;
	menu->entry_count++;
}

void Entry_update_sprite(struct Entry *entry, size_t pos)
{
	// stop if sprite doesn't exist
	if (pos > (entry->text.len - 1))
		return;

	char letter[2] = {
		[0] = entry->text.str[pos],
		[1] = '\0'
	};

	// clear
	Sprite_clear(&entry->sprites[pos]);

	// generate sprite of that letter
	entry->sprites[pos] = Sprite_from_text(entry->menu->renderer,
						    letter,
						    entry->font,
						    entry->style.font_color);
}

void Entry_update_sprites(struct Entry *entry)
{
	size_t i;
	
	for (i = 0; i < entry->text.len; i++)
		Entry_update_sprite(entry, i);
}

void Entry_append(struct Entry *entry, struct String *appendage)
{
	size_t old_size = entry->text.size;
	size_t new_size;
	size_t i;

	String_append(&entry->text, appendage);

	// increase sprite array size
	if (entry->text.size > old_size) {
		new_size = old_size * 2 * sizeof(struct Sprite);
		entry->sprites = realloc(entry->sprites, new_size);

		for (size_t i = old_size; i < entry->text.size; i++)
			entry->sprites[i] = Sprite_new();
	}

	// update new sprites
	for (i = entry->text.len - appendage->len; i < entry->text.len; i++)
		Entry_update_sprite(entry, i);
}

void Entry_draw(struct Entry *entry)
{
	SDL_Rect draw_target;
	u32 text_width = 0;

	// draw bg
	SDL_SetRenderDrawColor(entry->menu->renderer,
			       entry->style.bg_color.r,
			       entry->style.bg_color.g,
			       entry->style.bg_color.b,
			       entry->style.bg_color.a);
	SDL_RenderFillRect(entry->menu->renderer, &entry->rect);

	// draw border
	SDL_SetRenderDrawColor(entry->menu->renderer,
			       entry->style.border_color.r,
			       entry->style.border_color.g,
			       entry->style.border_color.b,
			       entry->style.border_color.a);
	SDL_RenderDrawRect(entry->menu->renderer, &entry->rect);

	// draw text
	for (size_t i = 0; i < entry->text.len; i++) {
		// if text exceeds entry width, stop
		if (text_width > (u32) entry->rect.w)
			break;

		// draw
		draw_target.x = entry->rect.x + text_width;
		draw_target.y = entry->rect.y;
		draw_target.w = entry->sprites[i].surface->w;
		draw_target.h = entry->sprites[i].surface->h;

		SDL_RenderCopy(entry->menu->renderer,
			       entry->sprites[i].texture, NULL, &draw_target);

		text_width += draw_target.w;
	}

	// if disabled, draw disabled shade
	if (entry->active == false) {
		SDL_SetRenderDrawColor(entry->menu->renderer,
				       entry->style.disabled_color.r,
				       entry->style.disabled_color.g,
				       entry->style.disabled_color.b,
				       entry->style.disabled_color.a);
		SDL_RenderFillRect(entry->menu->renderer, &entry->rect);
	}
}

void Entry_clear_sprites(struct Entry *entry)
{
	size_t i;
	
	for (i = 0; i < entry->text.len; i++)
		Sprite_clear(&entry->sprites[i]);

	free(entry->sprites);
}

void Entry_resize(struct Entry *entry)
{
	size_t i;
	
	entry->rect.h = entry->sprites[0].surface->h;
	entry->rect.w = 0;

	for (i = 0; i < entry->text.len; i++)
		entry->rect.w += entry->sprites[i].surface->w;
}
