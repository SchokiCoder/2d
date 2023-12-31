/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include "button.h"
#include "menu.h"
#include "theme.h"

void Button_new(struct Button *button,
		struct Menu *menu,
		TTF_Font *font,
		const struct ButtonStyle style)
{
	button->menu = menu;
	button->font = font;
	button->text = String_new(BUTTON_TEXT_INIT_SIZE);
	button->sprite = Sprite_new();
	button->visible = 1;
	button->active = 1;
	button->style = style;
	button->func_click = NULL;
	button->data_click = NULL;

	menu->buttons[menu->button_count] = button;
	menu->button_count++;
}

void Button_update_sprite(struct Button *button)
{
	Sprite_clear(&button->sprite);
	button->sprite = Sprite_from_text(button->menu->renderer,
					  button->text.str,
					  button->font,
					  button->style.font_color);
}

void Button_draw(struct Button *button)
{
	// draw bg
	SDL_SetRenderDrawColor(button->menu->renderer,
			       button->style.bg_color.r,
			       button->style.bg_color.g,
			       button->style.bg_color.b,
			       button->style.bg_color.a);
	SDL_RenderFillRect(button->menu->renderer, &button->rect);

	// draw border
	SDL_SetRenderDrawColor(button->menu->renderer,
			       button->style.border_color.r,
			       button->style.border_color.g,
			       button->style.border_color.b,
			       button->style.border_color.a);
	SDL_RenderDrawRect(button->menu->renderer, &button->rect);

	// draw text
	SDL_RenderCopy(button->menu->renderer,
		       button->sprite.texture, NULL, &button->rect);

	// if disabled, draw disabled shade
	if (button->active == 0) {
		SDL_SetRenderDrawColor(button->menu->renderer,
				       button->style.disabled_color.r,
				       button->style.disabled_color.g,
				       button->style.disabled_color.b,
				       button->style.disabled_color.a);
		SDL_RenderFillRect(button->menu->renderer, &button->rect);
	}
}

void Button_resize(struct Button *button)
{
	button->rect.w = button->sprite.surface->w;
	button->rect.h = button->sprite.surface->h;
}
