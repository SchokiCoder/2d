/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include "label.h"
#include "menu.h"
#include "theme.h"

void Label_new(struct Label            *label,
	       struct Menu             *menu,
	       TTF_Font                *font,
	       const struct LabelStyle  style)
{
	label->menu = menu;
	label->font = font;
	label->text = String_new(LABEL_TEXT_INIT_SIZE);
	label->sprite = Sprite_new();
	label->visible = 1;
	label->style = style;

	menu->labels[menu->label_count] = label;
	menu->label_count++;
}

void Label_update_sprite(struct Label *label)
{
	Sprite_clear(&label->sprite);
	label->sprite = Sprite_from_text(label->menu->renderer,
	                                 label->text.str,
	                      	         label->font,
	                                 label->style.font_color);
}

void Label_draw(struct Label *label)
{
	// draw bg
	SDL_SetRenderDrawColor(label->menu->renderer,
			       label->style.bg_color.r,
			       label->style.bg_color.g,
			       label->style.bg_color.b,
			       label->style.bg_color.a);
	SDL_RenderFillRect(label->menu->renderer, &label->rect);

	// draw border
	SDL_SetRenderDrawColor(label->menu->renderer,
			       label->style.border_color.r,
			       label->style.border_color.g,
			       label->style.border_color.b,
			       label->style.border_color.a);
	SDL_RenderDrawRect(label->menu->renderer, &label->rect);

	// draw text
	SDL_RenderCopy(label->menu->renderer,
		       label->sprite.texture, NULL, &label->rect);
}

void Label_resize(struct Label *label)
{
	label->rect.w = label->sprite.surface->w;
	label->rect.h = label->sprite.surface->h;
}
