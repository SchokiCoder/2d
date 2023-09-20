/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _SPRITE_H
#define _SPRITE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#include "engine/types.h"

struct Sprite {
	bool invalid;
	SDL_Surface *surface;
	SDL_Texture *texture;
};

struct Sprite Sprite_new(void);

struct Sprite Sprite_from_file(SDL_Renderer *renderer,
				  const char *filepath);

struct Sprite Sprite_from_text(SDL_Renderer *, const char *text,
				  TTF_Font *font, SDL_Color color);

void Sprite_create_texture(struct Sprite *sprite, SDL_Renderer *renderer);

void Sprite_clear(struct Sprite *sprite);

#endif				/* _SPRITE_H */
