/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SGUI_theme.h"
#include "SGUI_menu.h"
#include "SGUI_button.h"

void SGUI_Button_new(SGUI_Button * button, SGUI_Menu * menu, TTF_Font * font,
		     const SGUI_ButtonStyle style)
{
	button->menu = menu;
	button->font = font;
	button->text = SM_String_new(SGUI_BUTTON_TEXT_INIT_SIZE);
	button->sprite = SGUI_Sprite_new();
	button->visible = true;
	button->active = true;
	button->style = style;
	button->func_click = NULL;
	button->data_click = NULL;

	menu->buttons[menu->button_count] = button;
	menu->button_count++;
}

void SGUI_Button_update_sprite(SGUI_Button * button)
{
	SGUI_Sprite_clear(&button->sprite);
	button->sprite = SGUI_Sprite_from_text(button->menu->renderer,
					       button->text.str,
					       button->font,
					       button->style.font_color);
}

void SGUI_Button_draw(SGUI_Button * button)
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
	if (button->active == false) {
		SDL_SetRenderDrawColor(button->menu->renderer,
				       button->style.disabled_color.r,
				       button->style.disabled_color.g,
				       button->style.disabled_color.b,
				       button->style.disabled_color.a);
		SDL_RenderFillRect(button->menu->renderer, &button->rect);
	}
}

void SGUI_Button_resize(SGUI_Button * button)
{
	button->rect.w = button->sprite.surface->w;
	button->rect.h = button->sprite.surface->h;
}
