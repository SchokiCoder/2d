/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SGUI_SPRITE_H
#define SGUI_SPRITE_H

#include <SM_types.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

typedef struct SGUI_Sprite {
	bool invalid;
	SDL_Surface *surface;
	SDL_Texture *texture;
} SGUI_Sprite;

SGUI_Sprite SGUI_Sprite_new(void);

SGUI_Sprite SGUI_Sprite_from_file(SDL_Renderer * renderer,
				  const char *filepath);

SGUI_Sprite SGUI_Sprite_from_text(SDL_Renderer *, const char *text,
				  TTF_Font * font, SDL_Color color);

void SGUI_Sprite_create_texture(SGUI_Sprite * sprite, SDL_Renderer * renderer);

void SGUI_Sprite_clear(SGUI_Sprite * sprite);

#endif				/* SGUI_SPRITE_H */
