/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _THEME_H
#define _THEME_H

#include <SDL_pixels.h>

#include "button.h"
#include "entry.h"
#include "label.h"
#include "menu.h"

struct Theme {
	struct MenuStyle menu;
	struct LabelStyle label;
	struct ButtonStyle button;
	struct EntryStyle entry;
};

#endif				/* _THEME_H */
