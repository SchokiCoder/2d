/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <SDL_events.h>

#include "button.h"
#include "engine/sstring.h"
#include "entry.h"
#include "label.h"
#include "menu.h"
#include "theme.h"

struct Menu Menu_new(SDL_Renderer *renderer, const struct MenuStyle style)
{
	struct Menu result = {
		.renderer = renderer,
		.label_count = 0,
		.button_count = 0,
		.entry_count = 0,
		.focused_entry = NULL,
		.visible = 1,
		.active = 1,
		.style = style
	};

	return result;
}

void Menu_draw(struct Menu *menu)
{
	if (menu->visible == 0)
		return;

	SDL_SetRenderDrawColor(menu->renderer,
			       menu->style.bg_color.r,
			       menu->style.bg_color.g,
			       menu->style.bg_color.b, menu->style.bg_color.a);
	SDL_RenderFillRect(menu->renderer, &menu->rect);

	for (u8 i = 0; i < menu->label_count; i++) {
		if (menu->labels[i]->visible) {
			Label_draw(menu->labels[i]);
		}
	}

	for (u8 i = 0; i < menu->button_count; i++) {
		if (menu->buttons[i]->visible) {
			Button_draw(menu->buttons[i]);
		}
	}

	for (u8 i = 0; i < menu->entry_count; i++) {
		if (menu->entries[i]->visible) {
			Entry_draw(menu->entries[i]);
		}
	}
}

void Menu_handle_event(struct Menu *menu, SDL_Event *event)
{
	SDL_Point mouse;

	if (menu->visible == 0 || menu->active == 0)
		return;

	switch (event->type) {
	case SDL_MOUSEBUTTONUP:

		SDL_GetMouseState(&mouse.x, &mouse.y);

		// check buttons
		for (u8 i = 0; i < menu->button_count; i++) {
			// if not visible or not active or no event-function stored, skip
			if (menu->buttons[i]->visible == 0 ||
			    menu->buttons[i]->active == 0 ||
			    menu->buttons[i]->func_click == NULL)
				continue;

			// if mouse hit button
			if (SDL_PointInRect(&mouse, &menu->buttons[i]->rect)) {
				// execute event-function, stop
				menu->buttons[i]->func_click(menu->buttons[i]->
							     data_click);
				return;
			}
		}

		// check entries
		for (u8 i = 0; i < menu->entry_count; i++) {
			// if not visible or not active, skip
			if (menu->entries[i]->visible == 0 ||
			    menu->entries[i]->active == 0)
				continue;

			// if mouse hit entry
			if (SDL_PointInRect(&mouse, &menu->entries[i]->rect)) {
				// mark entry as focused, stop
				menu->focused_entry = menu->entries[i];
				return;
			}
		}
		break;

	case SDL_TEXTINPUT:

		// if menu has a focused entry
		if (menu->focused_entry != NULL) {
			// add new characters
			struct String new = String_contain(event->text.text);
			Entry_append(menu->focused_entry, &new);
		}
		break;

	case SDL_KEYDOWN:

		// if menu has a focused entry
		if (menu->focused_entry != NULL) {
			// backspace
			if (event->key.keysym.sym == SDLK_BACKSPACE) {
				if (menu->focused_entry->text.len > 0) {
					menu->focused_entry->text.str[menu->
								      focused_entry->
								      text.len -
								      1] = '\0';
					menu->focused_entry->text.len--;
				}
			}
		}

		break;
	}
}

void Menu_grid(struct Menu *menu)
{
	enum WidgetType {
		WT_None,
		WT_Label,
		WT_Button,
		WT_Entry,
	};

	union WidgetPointer {
		struct Label *label;
		struct Button *button;
		struct Entry *entry;
	};

	struct WidgetReference {
		enum WidgetType type;
		union WidgetPointer ptr;
	};

	struct CellData {
		struct WidgetReference wref;
		u32 width;
		u32 height;
	};

	const usize TABLE_W = 256;
	const usize TABLE_H = 256;

	struct CellData table[TABLE_W][TABLE_H];
	u32 colwidth[TABLE_W];
	u32 rowheight[TABLE_H];

	// init table
	for (usize x = 0; x < TABLE_W; x++) {
		for (usize y = 0; y < TABLE_H; y++) {
			table[x][y].wref.type = WT_None;
			table[x][y].wref.ptr.label = NULL;
			table[x][y].width = 0;
			table[x][y].height = 0;
		}
	}

	// assign widgets to cells
	SDL_Point tblpt;

	for (u8 i = 0; i < menu->label_count; i++) {
		tblpt.x = menu->labels[i]->rect.x;
		tblpt.y = menu->labels[i]->rect.y;

		table[tblpt.x][tblpt.y].wref.type = WT_Label;
		table[tblpt.x][tblpt.y].wref.ptr.label = menu->labels[i];
		table[tblpt.x][tblpt.y].width = menu->labels[i]->rect.w;
		table[tblpt.x][tblpt.y].height = menu->labels[i]->rect.h;
	}

	for (u8 i = 0; i < menu->button_count; i++) {
		tblpt.x = menu->buttons[i]->rect.x;
		tblpt.y = menu->buttons[i]->rect.y;

		table[tblpt.x][tblpt.y].wref.type = WT_Button;
		table[tblpt.x][tblpt.y].wref.ptr.button = menu->buttons[i];
		table[tblpt.x][tblpt.y].width = menu->buttons[i]->rect.w;
		table[tblpt.x][tblpt.y].height = menu->buttons[i]->rect.h;
	}

	for (u8 i = 0; i < menu->entry_count; i++) {
		tblpt.x = menu->entries[i]->rect.x;
		tblpt.y = menu->entries[i]->rect.y;

		table[tblpt.x][tblpt.y].wref.type = WT_Entry;
		table[tblpt.x][tblpt.y].wref.ptr.entry = menu->entries[i];
		table[tblpt.x][tblpt.y].width = menu->entries[i]->rect.w;
		table[tblpt.x][tblpt.y].height = menu->entries[i]->rect.h;
	}

	// get col width, row height
	for (usize x = 0; x < TABLE_W; x++) {
		for (usize y = 0; y < TABLE_H; y++) {
			// if cell has no content, skip
			if (table[x][y].wref.type == WT_None)
				continue;

			// if needed, increase size
			if (table[x][y].width > colwidth[x])
				colwidth[x] = table[x][y].width;

			if (table[x][y].height > rowheight[y])
				rowheight[y] = table[x][y].height;
		}
	}

	// change pos of widgets
	u32 pos_x = 0;
	u32 pos_y = 0;

	for (usize x = 0; x < TABLE_W; x++) {
		for (usize y = 0; y < TABLE_H; y++) {
			switch (table[x][y].wref.type) {
			case WT_Label:
				table[x][y].wref.ptr.label->rect.x = pos_x;
				table[x][y].wref.ptr.label->rect.y = pos_y;
				break;

			case WT_Button:
				table[x][y].wref.ptr.button->rect.x = pos_x;
				table[x][y].wref.ptr.button->rect.y = pos_y;
				break;

			case WT_Entry:
				table[x][y].wref.ptr.entry->rect.x = pos_x;
				table[x][y].wref.ptr.entry->rect.y = pos_y;
				break;

			case WT_None:
			default:
				break;
			}

			pos_y += rowheight[y];
		}

		pos_y = 0;
		pos_x += colwidth[x];
	}
}

void Menu_clear(struct Menu *menu)
{
	for (u8 i = 0; i < menu->label_count; i++) {
		Sprite_clear(&menu->labels[i]->sprite);
		String_clear(&menu->labels[i]->text);
	}

	for (u8 i = 0; i < menu->button_count; i++) {
		Sprite_clear(&menu->buttons[i]->sprite);
		String_clear(&menu->buttons[i]->text);
	}

	for (u8 i = 0; i < menu->entry_count; i++) {
		Entry_clear_sprites(menu->entries[i]);
		String_clear(&menu->entries[i]->text);
	}
}
