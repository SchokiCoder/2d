/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL_events.h>
#include <SM_string.h>
#include "SGUI_theme.h"
#include "SGUI_label.h"
#include "SGUI_button.h"
#include "SGUI_entry.h"
#include "SGUI_menu.h"

SGUI_Menu SGUI_Menu_new(SDL_Renderer * renderer, const SGUI_MenuStyle style)
{
	SGUI_Menu result = {
		.renderer = renderer,
		.label_count = 0,
		.button_count = 0,
		.entry_count = 0,
		.focused_entry = NULL,
		.visible = true,
		.active = true,
		.style = style
	};

	return result;
}

void SGUI_Menu_draw(SGUI_Menu * menu)
{
	// stop if not visible
	if (menu->visible == false)
		return;

	// draw bg
	SDL_SetRenderDrawColor(menu->renderer,
			       menu->style.bg_color.r,
			       menu->style.bg_color.g,
			       menu->style.bg_color.b, menu->style.bg_color.a);
	SDL_RenderFillRect(menu->renderer, &menu->rect);

	// draw labels
	for (u8 i = 0; i < menu->label_count; i++) {
		if (menu->labels[i]->visible) {
			SGUI_Label_draw(menu->labels[i]);
		}
	}

	// draw buttons
	for (u8 i = 0; i < menu->button_count; i++) {
		if (menu->buttons[i]->visible) {
			SGUI_Button_draw(menu->buttons[i]);
		}
	}

	// draw entries
	for (u8 i = 0; i < menu->entry_count; i++) {
		if (menu->entries[i]->visible) {
			SGUI_Entry_draw(menu->entries[i]);
		}
	}
}

void SGUI_Menu_handle_event(SGUI_Menu * menu, SDL_Event * event)
{
	SDL_Point mouse;

	// stop if not visible or not active
	if (menu->visible == false || menu->active == false)
		return;

	// handle current event
	switch (event->type) {
		// mouse click
	case SDL_MOUSEBUTTONUP:

		// get mouse coords
		SDL_GetMouseState(&mouse.x, &mouse.y);

		// check buttons
		for (u8 i = 0; i < menu->button_count; i++) {
			// if not visible or not active or no event-function stored, skip
			if (menu->buttons[i]->visible == false ||
			    menu->buttons[i]->active == false ||
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
			if (menu->entries[i]->visible == false ||
			    menu->entries[i]->active == false)
				continue;

			// if mouse hit entry
			if (SDL_PointInRect(&mouse, &menu->entries[i]->rect)) {
				// mark entry as focused, stop
				menu->focused_entry = menu->entries[i];
				return;
			}
		}
		break;

		// keyboard
	case SDL_TEXTINPUT:

		// if menu has a focused entry
		if (menu->focused_entry != NULL) {
			// add new characters
			SM_String new = SM_String_contain(event->text.text);
			SGUI_Entry_append(menu->focused_entry, &new);
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

void SGUI_Menu_grid(SGUI_Menu * menu)
{
	typedef enum WidgetType {
		WT_None,
		WT_Label,
		WT_Button,
		WT_Entry,
	} WidgetType;

	typedef union WidgetPointer {
		SGUI_Label *label;
		SGUI_Button *button;
		SGUI_Entry *entry;
	} WidgetPointer;

	typedef struct WidgetReference {
		WidgetType type;
		WidgetPointer ptr;
	} WidgetReference;

	typedef struct CellData {
		WidgetReference wref;
		u32 width;
		u32 height;
	} CellData;

	const usize TABLE_W = 256;
	const usize TABLE_H = 256;

	CellData table[TABLE_W][TABLE_H];
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

void SGUI_Menu_clear(SGUI_Menu * menu)
{
	for (u8 i = 0; i < menu->label_count; i++) {
		SGUI_Sprite_clear(&menu->labels[i]->sprite);
		SM_String_clear(&menu->labels[i]->text);
	}

	for (u8 i = 0; i < menu->button_count; i++) {
		SGUI_Sprite_clear(&menu->buttons[i]->sprite);
		SM_String_clear(&menu->buttons[i]->text);
	}

	for (u8 i = 0; i < menu->entry_count; i++) {
		SGUI_Entry_clear_sprites(menu->entries[i]);
		SM_String_clear(&menu->entries[i]->text);
	}
}
