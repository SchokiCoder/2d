/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <time.h>

#include "config.h"
#include "engine/log.h"
#include "engine/sstring.h"
#include "game.h"
#include "gui/sprite.h"
#include "gui/theme.h"
#include "path.h"
#include "world.h"

#define PATH_TEXTURES_BLOCKS_DIR PATH_TEXTURES "blocks"
#define PATH_TEXTURES_ENTITIES_DIR PATH_TEXTURES "entities"

static const char *PATH_TEXTURES_BLOCKS[] = {
	PATH_TEXTURES_BLOCKS_DIR SLASH "dirt.png",
	PATH_TEXTURES_BLOCKS_DIR SLASH "stone.png"
};

static const char *PATH_TEXTURES_ENTITIES[] = {
	PATH_TEXTURES_ENTITIES_DIR SLASH "player.png"
};

static const f32 TIMESCALE = 1.0f;

#ifdef _DEBUG
static const struct Theme THEME_DEBUG = {
	.menu = {
		 .bg_color = {.r = 155,.g = 219,.b = 245,.a = 0},
	},

	.label = {
		  .font_color = {.r = 255,.g = 0,.b = 255,.a = 255},
		  .bg_color = {.r = 0,.g = 0,.b = 0,.a = 20},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
	},

	.button = {
		   .font_color = {.r = 50,.g = 50,.b = 50,.a = 255},
		   .bg_color = {.r = 0,.g = 0,.b = 0,.a = 25},
		   .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		   .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
	},

	.entry = {
		  .font_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .bg_color = {.r = 240,.g = 240,.b = 240,.a = 255},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
	},
};
#endif

static const f32 EDIT_MOVE_SPEED = BLOCK_SIZE * 12;
static const size_t EDIT_CROSSHAIR_SIZE = 10;
static const f32 EDIT_SELECT_DELAY = 0.25f;
static const f32 EDIT_SAVE_DELAY = 1.0f;

f32 now(void)
{
	return (f32)clock() / (f32)CLOCKS_PER_SEC;
}

void Game_setup(struct Game *game)
{
	game->active = 1;
	game->msg = String_new(8);

	// set viewport
	game->camera.x = 0;
	game->camera.y = 0;
	game->camera.w = game->cfg->gfx_window_w;
	game->camera.h = game->cfg->gfx_window_h;

	// init sprites
	for (u32 i = 0; i <= B_LAST; i++) {
		game->spr_blocks[i] = Sprite_new();
		game->spr_walls[i] = Sprite_new();
	}

	for (u32 i = 0; i <= E_LAST; i++)
		game->spr_ents[i] = Sprite_new();

	// open world and check
	game->world = World_from_file(game->world_name);

	if (game->world.invalid) {
		String_copy_cstr(&game->msg, "World ");
		String_append_cstr(&game->msg, game->world_name);
		String_append_cstr(&game->msg, "is not valid.");
		Game_clear(game);
		return;
	}
	
	// load block sprites
	for (u32 i = 1; i <= B_LAST; i++) {
		game->spr_blocks[i] =
		    Sprite_from_file(game->renderer,
					  PATH_TEXTURES_BLOCKS[i - 1]);

		if (game->spr_blocks[i].invalid) {
			String_copy_cstr(&game->msg, "Sprite ");
			String_append_cstr(&game->msg,
					   PATH_TEXTURES_BLOCKS[i - 1]);
			String_append_cstr(&game->msg,
					   " could not be loaded.");

			log_err(game->msg.str);
			Game_clear(game);
			return;
		}
	}

	// create wall sprites
	for (u32 i = 1; i <= B_LAST; i++) {
		// copy block surface, modify, create texture
		game->spr_walls[i].invalid = 0;

		game->spr_walls[i].surface =
		    SDL_ConvertSurface(game->spr_blocks[i].surface,
				       game->spr_blocks[i].surface->format, 0);

		SDL_SetSurfaceColorMod(game->spr_walls[i].surface, 175, 175,
				       175);
		Sprite_create_texture(&game->spr_walls[i], game->renderer);

		// check sprite
		if (game->spr_walls[i].invalid) {
			log_err("Wall-sprite could not be generated from "
				"block-sprite.");
			Game_clear(game);
			return;
		}
	}

	// load ent sprites
	for (u32 i = 1; i <= E_LAST; i++) {
		game->spr_ents[i] =
		    Sprite_from_file(game->renderer,
					  PATH_TEXTURES_ENTITIES[i - 1]);

		if (game->spr_ents[i].invalid) {
			String_copy_cstr(&game->msg, "Sprite ");
			String_append_cstr(&game->msg,
					   PATH_TEXTURES_ENTITIES[i - 1]);
			String_append_cstr(&game->msg,
					   " could not be loaded.");

			log_err(game->msg.str);
			Game_clear(game);
			return;
		}
	}

	// map textures
	for (u32 x = 0; x < game->world.width; x++) {
		for (u32 y = 0; y < game->world.height; y++) {
			game->world.block_textures[x][y][0] =
			    game->spr_blocks[game->world.blocks[x][y][0]].
			    texture;
			game->world.block_textures[x][y][1] =
			    game->spr_walls[game->world.blocks[x][y][0]].
			    texture;
		}
	}

	// set keyboard state pointer
	game->kbd = SDL_GetKeyboardState(NULL);
}

void Game_run(struct Game *game)
{
#ifdef _DEBUG
	TTF_Font *font;
	struct Menu mnu_debugvals;
	struct Label lbl_velocity_x;
	struct Label lbl_velocity_x_val;
	struct Label lbl_velocity_y;
	struct Label lbl_velocity_y_val;
	struct Label lbl_pos_x;
	struct Label lbl_pos_x_val;
	struct Label lbl_pos_y;
	struct Label lbl_pos_y_val;
	struct Label lbl_grounded;
	struct Label lbl_grounded_val;
#endif

	struct Entity *player = NULL;
	SDL_Rect temp;
	f32 ts1, ts2, delta = 0.0f;
	f32 x_step = 0.0f;
	f32 y_step = 0.0f;

	// setup
	Game_setup(game);
	
	if (game->world.invalid) {
		return;
	}

	// set 1st player of world as player
	for (size_t i = 0; i < game->world.ent_count; i++)
		if (game->world.entities[i].id == E_PLAYER)
			player = &game->world.entities[i];

	if (player == NULL) {
		String_copy_cstr(&game->msg, "World ");
		String_append_cstr(&game->msg, game->world_name);
		String_append_cstr(&game->msg,
				      " does not contain a player entity.");
		log_err(game->msg.str);

		Game_clear(game);
		return;
	}
#ifdef _DEBUG
	// load font
	font =
	    TTF_OpenFont("/usr/share/fonts/liberation-mono/LiberationMono-Regular.ttf",
			 16);

	// make debug values menu
	mnu_debugvals = Menu_new(game->renderer, THEME_DEBUG.menu);
	Label_new(&lbl_velocity_x, &mnu_debugvals, font,
		       THEME_DEBUG.label);
	Label_new(&lbl_velocity_x_val, &mnu_debugvals, font,
		       THEME_DEBUG.label);
	Label_new(&lbl_velocity_y, &mnu_debugvals, font,
		       THEME_DEBUG.label);
	Label_new(&lbl_velocity_y_val, &mnu_debugvals, font,
		       THEME_DEBUG.label);
	Label_new(&lbl_pos_x, &mnu_debugvals, font, THEME_DEBUG.label);
	Label_new(&lbl_pos_x_val, &mnu_debugvals, font, THEME_DEBUG.label);
	Label_new(&lbl_pos_y, &mnu_debugvals, font, THEME_DEBUG.label);
	Label_new(&lbl_pos_y_val, &mnu_debugvals, font, THEME_DEBUG.label);
	Label_new(&lbl_grounded, &mnu_debugvals, font, THEME_DEBUG.label);
	Label_new(&lbl_grounded_val, &mnu_debugvals, font,
		       THEME_DEBUG.label);

	// define menu
	mnu_debugvals.rect.x = 0;
	mnu_debugvals.rect.y = 0;
	mnu_debugvals.rect.w = game->cfg->gfx_window_w;
	mnu_debugvals.rect.h = game->cfg->gfx_window_h;

	String_copy_cstr(&lbl_velocity_x.text, "vel_x:");
	Label_update_sprite(&lbl_velocity_x);
	lbl_velocity_x.rect.w = lbl_velocity_x.sprite.surface->w;
	lbl_velocity_x.rect.h = lbl_velocity_x.sprite.surface->h;
	lbl_velocity_x.rect.x = 0;
	lbl_velocity_x.rect.y = 0;

	String_copy_cstr(&lbl_velocity_x_val.text, "0");
	Label_update_sprite(&lbl_velocity_x_val);
	lbl_velocity_x_val.rect.w = lbl_velocity_x_val.sprite.surface->w;
	lbl_velocity_x_val.rect.h = lbl_velocity_x_val.sprite.surface->h;
	lbl_velocity_x_val.rect.x =
	    lbl_velocity_x.rect.x + lbl_velocity_x.rect.w + 10;
	lbl_velocity_x_val.rect.y = lbl_velocity_x.rect.y;

	String_copy_cstr(&lbl_velocity_y.text, "vel_y:");
	Label_update_sprite(&lbl_velocity_y);
	lbl_velocity_y.rect.w = lbl_velocity_y.sprite.surface->w;
	lbl_velocity_y.rect.h = lbl_velocity_y.sprite.surface->h;
	lbl_velocity_y.rect.x = lbl_velocity_x.rect.x;
	lbl_velocity_y.rect.y =
	    lbl_velocity_x.rect.y + lbl_velocity_x.rect.h + 10;

	String_copy_cstr(&lbl_velocity_y_val.text, "0");
	Label_update_sprite(&lbl_velocity_y_val);
	lbl_velocity_y_val.rect.w = lbl_velocity_y_val.sprite.surface->w;
	lbl_velocity_y_val.rect.h = lbl_velocity_y_val.sprite.surface->h;
	lbl_velocity_y_val.rect.x =
	    lbl_velocity_y.rect.x + lbl_velocity_y.rect.w + 10;
	lbl_velocity_y_val.rect.y = lbl_velocity_y.rect.y;

	String_copy_cstr(&lbl_pos_x.text, "pos_x:");
	Label_update_sprite(&lbl_pos_x);
	lbl_pos_x.rect.w = lbl_pos_x.sprite.surface->w;
	lbl_pos_x.rect.h = lbl_pos_x.sprite.surface->h;
	lbl_pos_x.rect.x = lbl_velocity_x.rect.x;
	lbl_pos_x.rect.y =
	    lbl_velocity_y_val.rect.y + lbl_velocity_y_val.rect.h + 10;

	String_copy_cstr(&lbl_pos_x_val.text, "0");
	Label_update_sprite(&lbl_pos_x_val);
	lbl_pos_x_val.rect.w = lbl_pos_x_val.sprite.surface->w;
	lbl_pos_x_val.rect.h = lbl_pos_x_val.sprite.surface->h;
	lbl_pos_x_val.rect.x = lbl_pos_x.rect.x + lbl_pos_x.rect.w + 10;
	lbl_pos_x_val.rect.y = lbl_pos_x.rect.y;

	String_copy_cstr(&lbl_pos_y.text, "pos_y:");
	Label_update_sprite(&lbl_pos_y);
	lbl_pos_y.rect.w = lbl_pos_y.sprite.surface->w;
	lbl_pos_y.rect.h = lbl_pos_y.sprite.surface->h;
	lbl_pos_y.rect.x = lbl_velocity_x.rect.x;
	lbl_pos_y.rect.y = lbl_pos_x_val.rect.y + lbl_pos_x_val.rect.h + 10;

	String_copy_cstr(&lbl_pos_y_val.text, "0");
	Label_update_sprite(&lbl_pos_y_val);
	lbl_pos_y_val.rect.w = lbl_pos_y_val.sprite.surface->w;
	lbl_pos_y_val.rect.h = lbl_pos_y_val.sprite.surface->h;
	lbl_pos_y_val.rect.x = lbl_pos_x.rect.x + lbl_pos_x.rect.w + 10;
	lbl_pos_y_val.rect.y = lbl_pos_y.rect.y;

	String_copy_cstr(&lbl_grounded.text, "grnd:");
	Label_update_sprite(&lbl_grounded);
	lbl_grounded.rect.w = lbl_grounded.sprite.surface->w;
	lbl_grounded.rect.h = lbl_grounded.sprite.surface->h;
	lbl_grounded.rect.x = lbl_velocity_x.rect.x;
	lbl_grounded.rect.y = lbl_pos_y_val.rect.y + lbl_pos_y_val.rect.h + 10;

	String_copy_cstr(&lbl_grounded_val.text, "0");
	Label_update_sprite(&lbl_grounded_val);
	lbl_grounded_val.rect.w = lbl_grounded_val.sprite.surface->w;
	lbl_grounded_val.rect.h = lbl_grounded_val.sprite.surface->h;
	lbl_grounded_val.rect.x =
	    lbl_grounded.rect.x + lbl_grounded.rect.w + 10;
	lbl_grounded_val.rect.y = lbl_grounded.rect.y;
#endif

	// mainloop
	while (game->active) {
		ts1 = now();

		// process events
		while (SDL_PollEvent(&game->event)) {
			// app events
			switch (game->event.type) {
			case SDL_QUIT:
				game->active = 0;
				break;
			}
		}

		// handle keyboard
		if (game->kbd[SDL_SCANCODE_A]) {
			player->velocity_x -=
			    DATA_ENTITIES[E_PLAYER].acceleration * delta;

			if (player->velocity_x <
			    DATA_ENTITIES[E_PLAYER].max_velocity * -1)
				player->velocity_x =
				    DATA_ENTITIES[E_PLAYER].max_velocity * -1;
		}

		if (game->kbd[SDL_SCANCODE_D]) {
			player->velocity_x +=
			    DATA_ENTITIES[E_PLAYER].acceleration * delta;

			if (player->velocity_x >
			    DATA_ENTITIES[E_PLAYER].max_velocity)
				player->velocity_x =
				    DATA_ENTITIES[E_PLAYER].max_velocity;
		}

		if (game->kbd[SDL_SCANCODE_SPACE]) {
			if (player->grounded)
				player->velocity_y -=
				    DATA_ENTITIES[E_PLAYER].jump_velocity;
		}
		// gravity
		player->velocity_y += ENTITY_GRAVITY * delta;

		// apply walking friction or stop at velocity threshold
		if (player->grounded) {
			if (player->velocity_x > ENTITY_VELOCITY_THRESHOLD)
				player->velocity_x -=
				    DATA_ENTITIES[E_PLAYER].decceleration *
				    delta;

			else if (player->velocity_x <
				 (ENTITY_VELOCITY_THRESHOLD * -1.0f))
				player->velocity_x +=
				    DATA_ENTITIES[E_PLAYER].decceleration *
				    delta;

			else
				player->velocity_x = 0.0f;
		}
		// movement proccessing
		if (player->velocity_x != 0.0f) {
			x_step = player->velocity_x * delta;
			Entity_move_x(player, x_step, &game->world);
		}

		if (player->velocity_y != 0.0f) {
			y_step = player->velocity_y * delta;
			Entity_move_y(player, y_step, &game->world);
		}
#ifdef _DEBUG
		sprintf(lbl_velocity_x_val.text.str, "%f", player->velocity_x);
		lbl_velocity_x_val.text.len =
		    strlen(lbl_velocity_x_val.text.str);
		Label_update_sprite(&lbl_velocity_x_val);
		lbl_velocity_x_val.rect.w =
		    lbl_velocity_x_val.sprite.surface->w;
		lbl_velocity_x_val.rect.h =
		    lbl_velocity_x_val.sprite.surface->h;

		sprintf(lbl_velocity_y_val.text.str, "%f", player->velocity_y);
		lbl_velocity_y_val.text.len =
		    strlen(lbl_velocity_y_val.text.str);
		Label_update_sprite(&lbl_velocity_y_val);
		lbl_velocity_y_val.rect.w =
		    lbl_velocity_y_val.sprite.surface->w;
		lbl_velocity_y_val.rect.h =
		    lbl_velocity_y_val.sprite.surface->h;

		sprintf(lbl_pos_x_val.text.str, "%f", player->rect.x);
		lbl_pos_x_val.text.len = strlen(lbl_pos_x_val.text.str);
		Label_update_sprite(&lbl_pos_x_val);
		lbl_pos_x_val.rect.w = lbl_pos_x_val.sprite.surface->w;
		lbl_pos_x_val.rect.h = lbl_pos_x_val.sprite.surface->h;

		sprintf(lbl_pos_y_val.text.str, "%f", player->rect.y);
		lbl_pos_y_val.text.len = strlen(lbl_pos_y_val.text.str);
		Label_update_sprite(&lbl_pos_y_val);
		lbl_pos_y_val.rect.w = lbl_pos_y_val.sprite.surface->w;
		lbl_pos_y_val.rect.h = lbl_pos_y_val.sprite.surface->h;

		sprintf(lbl_grounded_val.text.str, "%i", player->grounded);
		lbl_grounded_val.text.len = strlen(lbl_grounded_val.text.str);
		Label_update_sprite(&lbl_grounded_val);
		lbl_grounded_val.rect.w = lbl_grounded_val.sprite.surface->w;
		lbl_grounded_val.rect.h = lbl_grounded_val.sprite.surface->h;
#endif

		// update camera
		game->camera.x =
		    (player->rect.x + player->rect.w) - (game->camera.w / 2);
		game->camera.y =
		    (player->rect.y + player->rect.h) - (game->camera.h / 2);

		if (game->camera.x < 0)
			game->camera.x = 0;

		else if ((game->camera.x + game->camera.w) >=
			 (int)(game->world.width * BLOCK_SIZE))
			game->camera.x =
			    (game->world.width * BLOCK_SIZE) - game->camera.w;

		if (game->camera.y < 0)
			game->camera.y = 0;

		else if ((game->camera.y + game->camera.h) >=
			 (int)(game->world.height * BLOCK_SIZE))
			game->camera.y =
			    (game->world.height * BLOCK_SIZE) - game->camera.h;

		// update block draw range
		game->wld_draw_pts[0].x = (game->camera.x / BLOCK_SIZE);
		game->wld_draw_pts[0].y = (game->camera.y / BLOCK_SIZE);

		game->wld_draw_pts[1].x =
		    ((game->camera.x + game->camera.w) / BLOCK_SIZE) + 1;
		game->wld_draw_pts[1].y =
		    ((game->camera.y + game->camera.h) / BLOCK_SIZE) + 1;

		if (game->wld_draw_pts[1].x >= (i32) game->world.width)
			game->wld_draw_pts[1].x = game->world.width;

		if (game->wld_draw_pts[1].y >= (i32) game->world.height)
			game->wld_draw_pts[1].y = game->world.height;

		// draw background
		SDL_SetRenderDrawColor(game->renderer, 155, 219, 245, 255);
		SDL_RenderClear(game->renderer);

		// draw walls and blocks
		for (int x = game->wld_draw_pts[0].x;
		     x < game->wld_draw_pts[1].x; x++) {
			for (int y = game->wld_draw_pts[0].y;
			     y < game->wld_draw_pts[1].y; y++) {
				temp.x = (x * BLOCK_SIZE) - game->camera.x;
				temp.y = (y * BLOCK_SIZE) - game->camera.y;
				temp.w = BLOCK_SIZE;
				temp.h = BLOCK_SIZE;

				SDL_RenderCopy(game->renderer,
					       game->world.
					       block_textures[x][y][1], NULL,
					       &temp);

				SDL_RenderCopy(game->renderer,
					       game->world.
					       block_textures[x][y][0], NULL,
					       &temp);
			}
		}

		// draw entities
		for (size_t i = 0; i < game->world.ent_count; i++) {
			switch (game->world.entities[i].id) {
			case E_NONE:
			case E_PLAYER:
				break;
			}
		}

		// draw player
		temp.x = player->rect.x - game->camera.x;
		temp.y = player->rect.y - game->camera.y;
		temp.w = player->rect.w;
		temp.h = player->rect.h;

		SDL_RenderCopy(game->renderer,
			       game->spr_ents[E_PLAYER].texture, NULL, &temp);

#ifdef _DEBUG
		// draw debug menu
		Menu_draw(&mnu_debugvals);
#endif

		// show drawn image
		SDL_RenderPresent(game->renderer);

		// timestamp and delta
		ts2 = now();
		delta = ts2 - ts1;
		delta *= TIMESCALE;
	}

	// clear
	Game_clear(game);
}

void Game_edit(struct Game *game, const size_t width, const size_t height)
{
	struct String filepath = String_new(8);
	f32 edit_move_speed;
	SDL_Rect temp;
	u32 mouse_state;
	SDL_Rect mouse_pos;
	struct FPoint edit_pos = {
		.x = 0.0f,
		.y = 0.0f,
	};
	SDL_Rect edit_pt = {
		.x = 0,
		.y = 0,
	};
	enum Blocks edit_block = B_FIRST;
	f32 ts1, ts2, delta = 0.0f;
	f32 ts_ui_event = 0.0f;
	int edit_draw_grid = 1;
	int edit_draw_blocks = 1;
	int edit_draw_walls = 1;

	// if world does not yet exist, create
	if (get_world_path(&filepath) != 0)
		return;

	String_append_cstr(&filepath, game->world_name);
	String_append_cstr(&filepath, ".");
	String_append_cstr(&filepath, FILETYPE_WORLD);

	if (file_check_existence(filepath.str) == 0) {
		game->world = Ch_World_new(width, height);

		for (size_t x = 0; x < game->world.width; x++) {
			for (size_t y = 0; y < game->world.height; y++) {
				game->world.blocks[x][y][0] = B_NONE;
				game->world.blocks[x][y][1] = B_NONE;
			}
		}

		Ch_World_to_file(&game->world, game->world_name);

		World_clear(&game->world);
	}
	
	// setup
	Game_setup(game);
	
	if (game->world.invalid) {
		return;
	}

	// mainloop
	while (game->active) {
		ts1 = now();

		// process events
		while (SDL_PollEvent(&game->event)) {
			// app events
			switch (game->event.type) {
			case SDL_KEYDOWN:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
				// get mouse state
				mouse_state =
				    SDL_GetMouseState(&mouse_pos.x,
						      &mouse_pos.y);

				// calc edit_pos world coord
				edit_pt.x =
				    (game->camera.x + mouse_pos.x) / BLOCK_SIZE;
				edit_pt.y =
				    (game->camera.y + mouse_pos.y) / BLOCK_SIZE;

				// if left click, edit block
				if (mouse_state & SDL_BUTTON_LMASK) {
					game->world.blocks[edit_pt.x][edit_pt.
								      y][0] =
					    edit_block;
					game->world.block_textures[edit_pt.
								   x][edit_pt.
								      y][0] =
					    game->spr_blocks[edit_block].
					    texture;
				}
				// if right click, edit wall
				else if (mouse_state & SDL_BUTTON_RMASK) {
					game->world.blocks[edit_pt.x][edit_pt.
								      y][1] =
					    edit_block;
					game->world.block_textures[edit_pt.
								   x][edit_pt.
								      y][1] =
					    game->spr_walls[edit_block].texture;
				}
				break;

			case SDL_MOUSEWHEEL:
				//select block
				if (game->event.wheel.y > 0) {
					if (edit_block < B_LAST)
						edit_block++;
				}

				else if (game->event.wheel.y < 0) {
					if (edit_block > B_FIRST)
						edit_block--;
				}

				break;

			case SDL_QUIT:
				game->active = 0;
				break;
			}
		}

		// handle keyboard, movement
		if (game->kbd[SDL_SCANCODE_LSHIFT])
			edit_move_speed = EDIT_MOVE_SPEED * 6.0f;

		else if (game->kbd[SDL_SCANCODE_LCTRL])
			edit_move_speed = EDIT_MOVE_SPEED * 0.25f;

		else
			edit_move_speed = EDIT_MOVE_SPEED;

		if (game->kbd[SDL_SCANCODE_W])
			edit_pos.y -= edit_move_speed * delta;

		if (game->kbd[SDL_SCANCODE_A])
			edit_pos.x -= edit_move_speed * delta;

		if (game->kbd[SDL_SCANCODE_S])
			edit_pos.y += edit_move_speed * delta;

		if (game->kbd[SDL_SCANCODE_D])
			edit_pos.x += edit_move_speed * delta;

		// calc edit_pos world coord
		edit_pt.x = edit_pos.x / BLOCK_SIZE;
		edit_pt.y = edit_pos.y / BLOCK_SIZE;

		// keys with select delay
		if (now() > ts_ui_event + EDIT_SELECT_DELAY) {
			// arrow (left, right), select block
			if (game->kbd[SDL_SCANCODE_LEFT]) {
				if (edit_block > B_FIRST) {
					edit_block--;
					ts_ui_event = now();
				}
			}

			if (game->kbd[SDL_SCANCODE_RIGHT]) {
				if (edit_block < B_LAST) {
					edit_block++;
					ts_ui_event = now();
				}
			}
			// drawing control
			if (game->kbd[SDL_SCANCODE_F1]) {
				edit_draw_grid = !edit_draw_grid;
				ts_ui_event = now();
			}

			if (game->kbd[SDL_SCANCODE_F2]) {
				edit_draw_blocks = !edit_draw_blocks;
				ts_ui_event = now();
			}

			if (game->kbd[SDL_SCANCODE_F3]) {
				edit_draw_walls = !edit_draw_walls;
				ts_ui_event = now();
			}
		}
		// keys with save delay
		if (now() > ts_ui_event + EDIT_SAVE_DELAY) {
			// ctrl + s, save
			if (game->kbd[SDL_SCANCODE_LCTRL] &&
			    game->kbd[SDL_SCANCODE_S]) {
				World_to_file(&game->world, game->world_name);
				ts_ui_event = now();
			}
			// ctrl + q, quit
			if (game->kbd[SDL_SCANCODE_LCTRL] &&
			    game->kbd[SDL_SCANCODE_Q]) {
				game->active = 0;
				ts_ui_event = now();
			}
		}
		// arrow up, set wall
		if (game->kbd[SDL_SCANCODE_DOWN]) {
			game->world.blocks[edit_pt.x][edit_pt.y][1] =
			    edit_block;
			game->world.block_textures[edit_pt.x][edit_pt.y][1] =
			    game->spr_walls[edit_block].texture;
		}
		// arrow down, set block
		if (game->kbd[SDL_SCANCODE_UP]) {
			game->world.blocks[edit_pt.x][edit_pt.y][0] =
			    edit_block;
			game->world.block_textures[edit_pt.x][edit_pt.y][0] =
			    game->spr_blocks[edit_block].texture;
		}
		// update viewport
		game->camera.x = edit_pos.x - (game->camera.w / 2);
		game->camera.y = edit_pos.y - (game->camera.h / 2);

		if (game->camera.x < 0)
			game->camera.x = 0;

		else if ((game->camera.x + game->camera.w) >=
			 (int)(game->world.width * BLOCK_SIZE))
			game->camera.x =
			    (game->world.width * BLOCK_SIZE) - game->camera.w;

		if (game->camera.y < 0)
			game->camera.y = 0;

		else if ((game->camera.y + game->camera.h) >=
			 (int)(game->world.height * BLOCK_SIZE))
			game->camera.y =
			    (game->world.height * BLOCK_SIZE) - game->camera.h;

		// update block draw range
		game->wld_draw_pts[0].x = (game->camera.x / BLOCK_SIZE);
		game->wld_draw_pts[0].y = (game->camera.y / BLOCK_SIZE);

		game->wld_draw_pts[1].x =
		    ((game->camera.x + game->camera.w) / BLOCK_SIZE);
		game->wld_draw_pts[1].y =
		    ((game->camera.y + game->camera.h) / BLOCK_SIZE);

		if (game->wld_draw_pts[1].x >= (i32) game->world.width)
			game->wld_draw_pts[1].x = game->world.width;

		if (game->wld_draw_pts[1].y >= (i32) game->world.height)
			game->wld_draw_pts[1].y = game->world.height;

		// draw background
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
		SDL_RenderClear(game->renderer);

		// if enabled, draw walls
		if (edit_draw_walls) {
			for (int x = game->wld_draw_pts[0].x;
			     x < game->wld_draw_pts[1].x; x++) {
				for (int y = game->wld_draw_pts[0].y;
				     y < game->wld_draw_pts[1].y; y++) {
					temp.x =
					    (x * BLOCK_SIZE) - game->camera.x;
					temp.y =
					    (y * BLOCK_SIZE) - game->camera.y;
					temp.w = BLOCK_SIZE;
					temp.h = BLOCK_SIZE;

					SDL_RenderCopy(game->renderer,
						       game->world.
						       block_textures[x][y][1],
						       NULL, &temp);
				}
			}
		}
		// if enabled, draw blocks
		if (edit_draw_blocks) {
			for (int x = game->wld_draw_pts[0].x;
			     x < game->wld_draw_pts[1].x; x++) {
				for (int y = game->wld_draw_pts[0].y;
				     y < game->wld_draw_pts[1].y; y++) {
					temp.x =
					    (x * BLOCK_SIZE) - game->camera.x;
					temp.y =
					    (y * BLOCK_SIZE) - game->camera.y;
					temp.w = BLOCK_SIZE;
					temp.h = BLOCK_SIZE;

					SDL_RenderCopy(game->renderer,
						       game->world.
						       block_textures[x][y][0],
						       NULL, &temp);
				}
			}
		}
		// if enabled, draw grid
		if (edit_draw_grid) {
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 50);

			for (int x = game->wld_draw_pts[0].x;
			     x < game->wld_draw_pts[1].x; x++) {
				for (int y = game->wld_draw_pts[0].y;
				     y < game->wld_draw_pts[1].y; y++) {
					temp.x =
					    (x * BLOCK_SIZE) - game->camera.x;
					temp.y =
					    (y * BLOCK_SIZE) - game->camera.y;
					temp.w = BLOCK_SIZE;
					temp.h = BLOCK_SIZE;

					SDL_RenderDrawRect(game->renderer,
							   &temp);
				}
			}
		}
		// draw edit pos as crosshair
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(game->renderer,
				   edit_pos.x - EDIT_CROSSHAIR_SIZE -
				   game->camera.x, edit_pos.y - game->camera.y,
				   edit_pos.x + EDIT_CROSSHAIR_SIZE -
				   game->camera.x, edit_pos.y - game->camera.y);

		SDL_RenderDrawLine(game->renderer,
				   edit_pos.x - game->camera.x,
				   edit_pos.y - EDIT_CROSSHAIR_SIZE -
				   game->camera.y, edit_pos.x - game->camera.x,
				   edit_pos.y + EDIT_CROSSHAIR_SIZE -
				   game->camera.y);

		// draw currently selected block (border)
		temp.x = BLOCK_SIZE;
		temp.y = 0;
		temp.w = 2;
		temp.h = BLOCK_SIZE + 2;

		SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(game->renderer, &temp);

		temp.x = 0;
		temp.y = BLOCK_SIZE;
		temp.w = BLOCK_SIZE + 2;
		temp.h = 2;

		SDL_RenderFillRect(game->renderer, &temp);

		// (block)
		temp.x = 0;
		temp.y = 0;
		temp.w = BLOCK_SIZE;
		temp.h = BLOCK_SIZE;

		SDL_RenderCopy(game->renderer,
			       game->spr_blocks[edit_block].texture,
			       NULL, &temp);

		// show drawn image
		SDL_RenderPresent(game->renderer);

		// timestamp and delta
		ts2 = now();
		delta = ts2 - ts1;
	}

	// clear
	Game_clear(game);
}

void Game_clear(struct Game *game)
{
	// reset viewport
	SDL_RenderSetViewport(game->renderer, NULL);

	// sprites
	for (u32 i = 1; i <= B_LAST; i++) {
		Sprite_clear(&game->spr_blocks[i]);
		Sprite_clear(&game->spr_walls[i]);
	}

	for (u32 i = 1; i <= E_LAST; i++)
		Sprite_clear(&game->spr_ents[i]);

	// world
	World_clear(&game->world);

	// string
	String_clear(&game->msg);
}
