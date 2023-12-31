/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <SDL_ttf.h>

#include "sprite.h"

struct Sprite Sprite_new(void)
{
	struct Sprite result = {
		.invalid = false,
		.surface = NULL,
		.texture = NULL
	};

	return result;
}

void Sprite_create_texture(struct Sprite *sprite, SDL_Renderer *renderer)
{
	// create texture
	sprite->texture =
	    SDL_CreateTextureFromSurface(renderer, sprite->surface);

	// check
	if (sprite->texture == NULL) {
		sprite->invalid = true;
	}
}

struct Sprite Sprite_from_file(SDL_Renderer *renderer, const char *filepath)
{
	struct Sprite result = {.invalid = false };

	// load image, create texture
	result.surface = SDL_LoadBMP(filepath);
	Sprite_create_texture(&result, renderer);

	return result;
}

struct Sprite Sprite_from_text(SDL_Renderer *renderer, const char *text,
				  TTF_Font *font, SDL_Color color)
{
	struct Sprite result = {.invalid = false };

	// create text, create texture
	result.surface = TTF_RenderText_Solid(font, text, color);
	Sprite_create_texture(&result, renderer);

	return result;
}

void Sprite_clear(struct Sprite *sprite)
{
	//reset values
	sprite->invalid = false;

	// if surface and texture exist, clear them
	if (sprite->surface != NULL) {
		SDL_FreeSurface(sprite->surface);
		sprite->surface = NULL;
	}

	if (sprite->texture != NULL) {
		SDL_DestroyTexture(sprite->texture);
		sprite->texture = NULL;
	}
}
