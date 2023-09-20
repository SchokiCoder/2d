/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
