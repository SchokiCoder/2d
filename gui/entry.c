/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SGUI_theme.h"
#include "SGUI_menu.h"
#include "SGUI_entry.h"

void SGUI_Entry_new(SGUI_Entry * entry, SGUI_Menu * menu, TTF_Font * font,
		    const SGUI_EntryStyle style)
{
	entry->menu = menu;
	entry->font = font;

	entry->sprites =
	    malloc(SGUI_ENTRY_TEXT_INIT_SIZE * sizeof(SGUI_Sprite));

	for (size_t i = 0; i < SGUI_ENTRY_TEXT_INIT_SIZE; i++)
		entry->sprites[i] = SGUI_Sprite_new();

	entry->text = SM_String_new(SGUI_ENTRY_TEXT_INIT_SIZE);
	entry->visible = true;
	entry->active = true;
	entry->style = style;

	menu->entries[menu->entry_count] = entry;
	menu->entry_count++;
}

void SGUI_Entry_update_sprite(SGUI_Entry * entry, size_t pos)
{
	// stop if sprite doesn't exist
	if (pos > (entry->text.len - 1))
		return;

	char letter[2] = {
		[0] = entry->text.str[pos],
		[1] = '\0'
	};

	// clear
	SGUI_Sprite_clear(&entry->sprites[pos]);

	// generate sprite of that letter
	entry->sprites[pos] = SGUI_Sprite_from_text(entry->menu->renderer,
						    letter,
						    entry->font,
						    entry->style.font_color);
}

void SGUI_Entry_update_sprites(SGUI_Entry * entry)
{
	for (size_t i = 0; i < entry->text.len; i++) {
		SGUI_Entry_update_sprite(entry, i);
	}
}

void SGUI_Entry_append(SGUI_Entry * entry, SM_String * appendage)
{
	size_t old_size = entry->text.size;

	// append
	SM_String_append(&entry->text, appendage);

	// increase sprite array size
	if (entry->text.size > old_size) {
		entry->sprites =
		    realloc(entry->sprites, old_size * 2 * sizeof(SGUI_Sprite));

		for (size_t i = old_size; i < entry->text.size; i++)
			entry->sprites[i] = SGUI_Sprite_new();
	}
	// update new sprites
	for (size_t i = entry->text.len - appendage->len; i < entry->text.len;
	     i++)
		SGUI_Entry_update_sprite(entry, i);
}

void SGUI_Entry_draw(SGUI_Entry * entry)
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

void SGUI_Entry_clear_sprites(SGUI_Entry * entry)
{
	for (size_t i = 0; i < entry->text.len; i++) {
		SGUI_Sprite_clear(&entry->sprites[i]);
	}

	free(entry->sprites);
}

void SGUI_Entry_resize(SGUI_Entry * entry)
{
	entry->rect.h = entry->sprites[0].surface->h;
	entry->rect.w = 0;

	for (usize i = 0; i < entry->text.len; i++) {
		entry->rect.w += entry->sprites[i].surface->w;
	}
}
