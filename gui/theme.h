/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SGUI_THEME_H
#define SGUI_THEME_H

#include <SDL_pixels.h>
#include "SGUI_menu.h"
#include "SGUI_label.h"
#include "SGUI_button.h"
#include "SGUI_entry.h"

typedef struct SGUI_Theme {
	SGUI_MenuStyle menu;
	SGUI_LabelStyle label;
	SGUI_ButtonStyle button;
	SGUI_EntryStyle entry;
} SGUI_Theme;

#endif				/* SGUI_THEME_H */
