/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL_ttf.h>
#include "SGUI_sprite.h"

SGUI_Sprite SGUI_Sprite_new(void)
{
	SGUI_Sprite result = {
		.invalid = false,
		.surface = NULL,
		.texture = NULL
	};

	return result;
}

void SGUI_Sprite_create_texture(SGUI_Sprite * sprite, SDL_Renderer * renderer)
{
	// create texture
	sprite->texture =
	    SDL_CreateTextureFromSurface(renderer, sprite->surface);

	// check
	if (sprite->texture == NULL) {
		sprite->invalid = true;
	}
}

SGUI_Sprite SGUI_Sprite_from_file(SDL_Renderer * renderer, const char *filepath)
{
	SGUI_Sprite result = {.invalid = false };

	// load image, create texture
	result.surface = SDL_LoadBMP(filepath);
	SGUI_Sprite_create_texture(&result, renderer);

	return result;
}

SGUI_Sprite SGUI_Sprite_from_text(SDL_Renderer * renderer, const char *text,
				  TTF_Font * font, SDL_Color color)
{
	SGUI_Sprite result = {.invalid = false };

	// create text, create texture
	result.surface = TTF_RenderText_Solid(font, text, color);
	SGUI_Sprite_create_texture(&result, renderer);

	return result;
}

void SGUI_Sprite_clear(SGUI_Sprite * sprite)
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
