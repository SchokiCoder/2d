/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>

#include "config.h"
#include "engine/log.h"
#include "engine/sstring.h"
#include "engine/types.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/menu.h"
#include "gui/theme.h"
#include "game.h"
#include "motd.h"
#include "world.h"

static const i16 FONT_SIZE = 16;

static const f32 MENU_FRAMERATE = 30.0f;

static const u32 MNU_MAIN_X = 50;
static const u32 MNU_MAIN_Y = 200;

static const u32 MNU_SUB_X = MNU_MAIN_X + 25;
static const u32 MNU_SUB_Y = MNU_MAIN_Y + 0;

static const struct Theme THEME_MASTER = {
	.menu = {
		 .bg_color = {.r = 155,.g = 219,.b = 245,.a = 255},
		 },

	.label = {
		  .font_color = {.r = 50,.g = 50,.b = 200,.a = 255},
		  .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		  },

	.button = {
		   .font_color = {.r = 50,.g = 50,.b = 50,.a = 255},
		   .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
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

static const struct Theme THEME_MAIN = {
	.menu = {
		 .bg_color = {.r = 0,.g = 0,.b = 0,.a = 0},
	},

	.label = {
		  .font_color = {.r = 50,.g = 50,.b = 200,.a = 255},
		  .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
	},

	.button = {
		   .font_color = {.r = 50,.g = 50,.b = 50,.a = 255},
		   .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
		   .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
		   .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
	},

	.entry = {
		  .font_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .bg_color = {.r = 240,.g = 240,.b = 240,.a = 255},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
	}
};

static const struct Theme THEME_SUB = {
	.menu = {
		 .bg_color = {.r = 0,.g = 0,.b = 0,.a = 0},
	},

	.label = {
		  .font_color = {.r = 50,.g = 50,.b = 200,.a = 255},
		  .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
		  .border_color = {.r = 0,.g = 0,.b = 0,.a = 0},
	},

	.button = {
		   .font_color = {.r = 20,.g = 255,.b = 20,.a = 255},
		   .bg_color = {.r = 255,.g = 255,.b = 255,.a = 255},
		   .border_color = {.r = 255,.g = 255,.b = 255,.a = 20},
		   .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
	},

	.entry = {
		  .font_color = {.r = 0,.g = 0,.b = 0,.a = 255},
		  .bg_color = {.r = 240,.g = 240,.b = 240,.a = 255},
		  .border_color = {.r = 200,.g = 200,.b = 200,.a = 255},
		  .disabled_color = {.r = 0,.g = 0,.b = 0,.a = 50},
	}
};

struct MenuData {
	SDL_Event *event;
	struct Menu *mnu_main;
	struct Menu *mnu_start_game;
	struct Menu *mnu_editor;
	struct Menu *mnu_settings;
	struct Menu *mnu_license;
};

struct BtnSettingsData {
	struct MenuData *menu_data;
	struct Config *cfg;
	struct Entry *txt_gfx_window_w;
	struct Entry *txt_gfx_window_h;
	struct Entry *txt_gfx_window_fullscreen;
};

struct BtnStartEditData {
	struct Game game;

	struct Entry *txt_edit_name;
	struct Entry *txt_edit_width;
	struct Entry *txt_edit_height;
};

void btn_start_game_click(void *ptr)
{
	struct MenuData *data = (struct MenuData*) ptr;

	// toggle menus, clear event
	data->mnu_main->visible = 0;
	data->mnu_start_game->visible = 1;
	data->event->type = SDL_USEREVENT;
}

void btn_editor_click(void *ptr)
{
	struct MenuData *data = (struct MenuData*) ptr;

	// toggle menus, clear event
	data->mnu_main->visible = 0;
	data->mnu_editor->visible = 1;
	data->event->type = SDL_USEREVENT;
}

void btn_settings_click(void *ptr)
{
	struct BtnSettingsData *data = (struct BtnSettingsData*) ptr;

	// toggle menus, clear event
	data->menu_data->mnu_main->visible = 0;
	data->menu_data->mnu_settings->visible = 1;
	data->menu_data->event->type = SDL_USEREVENT;

	// update setting entries
	sprintf(data->txt_gfx_window_w->text.str, "%i",
		data->cfg->gfx_window_w);
	data->txt_gfx_window_w->text.len =
	    strlen(data->txt_gfx_window_w->text.str) - 1;
	Entry_update_sprites(data->txt_gfx_window_w);

	sprintf(data->txt_gfx_window_h->text.str, "%i",
		data->cfg->gfx_window_h);
	data->txt_gfx_window_h->text.len =
	    strlen(data->txt_gfx_window_h->text.str) - 1;
	Entry_update_sprites(data->txt_gfx_window_h);

	sprintf(data->txt_gfx_window_fullscreen->text.str, "%i",
		data->cfg->gfx_window_fullscreen);
	data->txt_gfx_window_fullscreen->text.len =
	    strlen(data->txt_gfx_window_fullscreen->text.str) - 1;
	Entry_update_sprites(data->txt_gfx_window_fullscreen);
}

void btn_exit_click(void *ptr)
{
	int *active = (int*) ptr;

	*active = 0;
}

void btn_version_click(void *ptr)
{
	struct MenuData *data = (struct MenuData*) ptr;

	// toggle menus, clear event
	data->mnu_main->visible = 0;
	data->mnu_license->visible = 1;
	data->event->type = SDL_USEREVENT;
}

void btn_start_game_close_click(void *ptr)
{
	struct MenuData *data = (struct MenuData*) ptr;

	// toggle menus, clear event
	data->mnu_main->visible = 1;
	data->mnu_start_game->visible = 0;
	data->event->type = SDL_USEREVENT;
}

void btn_editor_close_click(void *ptr)
{
	struct MenuData *data = (struct MenuData*) ptr;

	// toggle menus, clear event
	data->mnu_main->visible = 1;
	data->mnu_editor->visible = 0;
	data->event->type = SDL_USEREVENT;
}

void btn_settings_close_click(void *ptr)
{
	struct BtnSettingsData *data = (struct BtnSettingsData*) ptr;

	// toggle menus, clear event
	data->menu_data->mnu_main->visible = 1;
	data->menu_data->mnu_settings->visible = 0;
	data->menu_data->event->type = SDL_USEREVENT;

	// save changes to cfg
	data->cfg->gfx_window_w =
		strtol(data->txt_gfx_window_w->text.str, NULL, 10);
	data->cfg->gfx_window_h =
	    strtol(data->txt_gfx_window_h->text.str, NULL, 10);
	data->cfg->gfx_window_fullscreen =
	    strtol(data->txt_gfx_window_fullscreen->text.str, NULL, 10);
}

void btn_license_close_click(void *ptr)
{
	struct MenuData *data = (struct MenuData*) ptr;

	// toggle menus, clear event
	data->mnu_main->visible = 1;
	data->mnu_license->visible = 0;
	data->event->type = SDL_USEREVENT;
}

void btn_chapter1_click(void *ptr)
{
	struct Game *data = (struct Game*) ptr;

	Game_run(data);
}

void btn_start_edit_click(void *ptr)
{
	struct BtnStartEditData *data = (struct BtnStartEditData*) ptr;
	size_t world_width;
	size_t world_height;

	// parse input
	world_width = strtoul(data->txt_edit_width->text.str, NULL, 10);
	world_height = strtoul(data->txt_edit_height->text.str, NULL, 10);
	data->game.world_name = data->txt_edit_name->text.str;

	// start editor
	Game_edit(&data->game, world_width, world_height);
}

#include "entity.h"
void gen_demo_horizontal(void)
{
	struct World out = Ch_World_new(128, 128);

	out.blocks[0][0][0] = B_STONE;
	out.blocks[0][1][0] = B_STONE;
	out.blocks[0][5][0] = B_DIRT;

	out.blocks[1][5][0] = B_DIRT;
	out.blocks[1][6][0] = B_STONE;

	out.blocks[2][5][0] = B_DIRT;
	out.blocks[2][6][0] = B_STONE;

	out.blocks[3][5][0] = B_DIRT;
	out.blocks[3][6][0] = B_STONE;

	out.blocks[4][5][0] = B_DIRT;
	out.blocks[4][6][0] = B_STONE;

	out.blocks[5][5][0] = B_DIRT;
	out.blocks[5][6][0] = B_STONE;

	out.blocks[6][6][0] = B_STONE;

	out.blocks[7][6][0] = B_STONE;

	out.blocks[10][0][0] = B_STONE;
	out.blocks[10][1][0] = B_STONE;

	out.blocks[0][3][1] = B_DIRT;
	out.blocks[0][4][1] = B_DIRT;

	out.blocks[1][3][1] = B_DIRT;
	out.blocks[1][4][1] = B_DIRT;

	out.blocks[2][3][1] = B_DIRT;
	out.blocks[2][4][1] = B_DIRT;

	out.blocks[3][3][1] = B_DIRT;
	out.blocks[3][4][1] = B_DIRT;

	out.blocks[4][4][1] = B_DIRT;

	out.entities[0].rect.x = 2.0f * (f32)BLOCK_SIZE;
	out.entities[0].rect.y = 1.0f * (f32)BLOCK_SIZE;

	World_to_file(&out, "test");
	World_clear(&out);
}

int main()
{
	struct String window_title = String_new(64);
	struct String msg = String_new(16);
	SDL_Window *window;
	SDL_Renderer *renderer = NULL;
	TTF_Font *font;
	int main_active = 1;
	SDL_Event event;
	struct Config cfg = Config_new();

	struct Menu mnu_master;
	struct Label lbl_app_name;
	struct Button btn_version;

	struct Menu mnu_main;
	struct Button btn_start_game;
	struct Button btn_editor;
	struct Button btn_settings;
	struct Button btn_exit;

	struct Menu mnu_start_game;
	struct Button btn_start_game_close;
	struct Button btn_chapter1;

	struct Menu mnu_editor;
	struct Button btn_editor_close;
	struct Label lbl_edit_name;
	struct Entry txt_edit_name;
	struct Label lbl_edit_width;
	struct Entry txt_edit_width;
	struct Label lbl_edit_height;
	struct Entry txt_edit_height;
	struct Button btn_start_edit;

	struct Menu mnu_settings;
	struct Button btn_settings_close;
	struct Label lbl_gfx_window_w;
	struct Entry txt_gfx_window_w;
	struct Label lbl_gfx_window_h;
	struct Entry txt_gfx_window_h;
	struct Label lbl_gfx_window_fullscreen;
	struct Entry txt_gfx_window_fullscreen;

	struct Menu mnu_license;
	struct Button btn_license_close;
	struct Label lbl_license;
	struct Label lbl_notice1;
	struct Label lbl_notice2;
	struct Label lbl_source1;
	struct Label lbl_source2;

	struct MenuData menu_data = {
		.event = &event,
		.mnu_main = &mnu_main,
		.mnu_start_game = &mnu_start_game,
		.mnu_editor = &mnu_editor,
		.mnu_settings = &mnu_settings,
		.mnu_license = &mnu_license,
	};

	struct BtnSettingsData btn_settings_data = {
		.cfg = &cfg,
		.menu_data = &menu_data,
		.txt_gfx_window_w = &txt_gfx_window_w,
		.txt_gfx_window_h = &txt_gfx_window_h,
		.txt_gfx_window_fullscreen = &txt_gfx_window_fullscreen,
	};

	// open log file and check
	log_open();

	if (logfile == NULL) {
		printf("Log file \"%s\" could not be opened.\nAbort.\n",
		       PATH_LOG);
		goto main_clear;
	}
	// load config
	Config_from_file(&cfg);

	// parse config values
	int window_mode = SDL_WINDOW_SHOWN;

	if (cfg.gfx_window_fullscreen)
		window_mode = SDL_WINDOW_FULLSCREEN;

	// seed random generator
	srand(time(NULL));

	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		log_err("SDL could not initialize.");
		goto main_clear;
	}
	// init TTF
	if (TTF_Init() != 0) {
		log_err("TTF could not initialize.");
		goto main_clear;
	}
	// load font
	font = TTF_OpenFont(
		"/usr/share/fonts/liberation-mono/LiberationMono-Regular.ttf",
		FONT_SIZE);

	// create window title
	String_copy_cstr(&window_title, APP_NAME);
	String_append_cstr(&window_title, ": ");
	String_append_cstr(&window_title,
			      MOTDS[rand() %
				    (sizeof(MOTDS) / sizeof(MOTDS[0]))]);

	// create window and renderer
	window = SDL_CreateWindow(window_title.str,
				  cfg.gfx_window_x, cfg.gfx_window_y,
				  cfg.gfx_window_w, cfg.gfx_window_h,
				  window_mode);
	renderer = SDL_CreateRenderer(window, -1, 0);

	// check window and renderer
	if (window == NULL || renderer == NULL) {
		log_err("SDL could not open a window and renderer.");
		goto main_clear;
	}
	// game data
	struct Game game = {
		.world_name = "test",
		.renderer = renderer,
		.cfg = &cfg,
	};

	struct BtnStartEditData btn_start_edit_data = {
		.game = {
			 .renderer = renderer,
			 .cfg = &cfg,
			 },
		.txt_edit_name = &txt_edit_name,
		.txt_edit_width = &txt_edit_width,
		.txt_edit_height = &txt_edit_height,
	};

	// enable alpha blending
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// make menus
	mnu_master = Menu_new(renderer, THEME_MASTER.menu);
	Label_new(&lbl_app_name, &mnu_master, font, THEME_MASTER.label);
	Button_new(&btn_version, &mnu_master, font, THEME_MASTER.button);

	mnu_main = Menu_new(renderer, THEME_MAIN.menu);
	Button_new(&btn_start_game, &mnu_main, font, THEME_MAIN.button);
	Button_new(&btn_editor, &mnu_main, font, THEME_MAIN.button);
	Button_new(&btn_settings, &mnu_main, font, THEME_MAIN.button);
	Button_new(&btn_exit, &mnu_main, font, THEME_MAIN.button);

	mnu_start_game = Menu_new(renderer, THEME_SUB.menu);
	Button_new(&btn_start_game_close, &mnu_start_game, font,
			THEME_SUB.button);
	Button_new(&btn_chapter1, &mnu_start_game, font, THEME_SUB.button);

	mnu_editor = Menu_new(renderer, THEME_SUB.menu);
	Button_new(&btn_editor_close, &mnu_editor, font, THEME_SUB.button);
	Label_new(&lbl_edit_name, &mnu_editor, font, THEME_SUB.label);
	Entry_new(&txt_edit_name, &mnu_editor, font, THEME_SUB.entry);
	Label_new(&lbl_edit_width, &mnu_editor, font, THEME_SUB.label);
	Entry_new(&txt_edit_width, &mnu_editor, font, THEME_SUB.entry);
	Label_new(&lbl_edit_height, &mnu_editor, font, THEME_SUB.label);
	Entry_new(&txt_edit_height, &mnu_editor, font, THEME_SUB.entry);
	Button_new(&btn_start_edit, &mnu_editor, font, THEME_SUB.button);

	mnu_settings = Menu_new(renderer, THEME_SUB.menu);
	Button_new(&btn_settings_close, &mnu_settings, font,
			THEME_SUB.button);
	Label_new(&lbl_gfx_window_w, &mnu_settings, font, THEME_SUB.label);
	Entry_new(&txt_gfx_window_w, &mnu_settings, font, THEME_SUB.entry);
	Label_new(&lbl_gfx_window_h, &mnu_settings, font, THEME_SUB.label);
	Entry_new(&txt_gfx_window_h, &mnu_settings, font, THEME_SUB.entry);
	Label_new(&lbl_gfx_window_fullscreen, &mnu_settings, font,
		       THEME_SUB.label);
	Entry_new(&txt_gfx_window_fullscreen, &mnu_settings, font,
		       THEME_SUB.entry);

	mnu_license = Menu_new(renderer, THEME_SUB.menu);
	Button_new(&btn_license_close, &mnu_license, font,
			THEME_SUB.button);
	Label_new(&lbl_license, &mnu_license, font, THEME_SUB.label);
	Label_new(&lbl_notice1, &mnu_license, font, THEME_SUB.label);
	Label_new(&lbl_notice2, &mnu_license, font, THEME_SUB.label);
	Label_new(&lbl_source1, &mnu_license, font, THEME_SUB.label);
	Label_new(&lbl_source2, &mnu_license, font, THEME_SUB.label);

	// define menu mnu_master
	mnu_master.rect.x = 0;
	mnu_master.rect.y = 0;
	mnu_master.rect.w = cfg.gfx_window_w;
	mnu_master.rect.h = cfg.gfx_window_h;

	String_copy_cstr(&lbl_app_name.text, APP_NAME);
	Label_update_sprite(&lbl_app_name);
	lbl_app_name.rect.w = lbl_app_name.sprite.surface->w;
	lbl_app_name.rect.h = lbl_app_name.sprite.surface->h;
	lbl_app_name.rect.x = 25;
	lbl_app_name.rect.y = 150;

	String_append_cstr(&btn_version.text, APP_VERSION);
	Button_update_sprite(&btn_version);
	btn_version.rect.w = btn_version.sprite.surface->w;
	btn_version.rect.h = btn_version.sprite.surface->h;
	btn_version.rect.x = cfg.gfx_window_w - btn_version.rect.w - 5;
	btn_version.rect.y = cfg.gfx_window_h - btn_version.rect.h - 5;
	btn_version.func_click = btn_version_click;
	btn_version.data_click = &menu_data;

	// mnu_main
	mnu_main.rect.x = MNU_MAIN_X;
	mnu_main.rect.y = MNU_MAIN_Y;
	mnu_main.rect.w = 0;
	mnu_main.rect.h = 0;

	String_copy_cstr(&btn_start_game.text, "Start game ->");
	Button_update_sprite(&btn_start_game);
	btn_start_game.rect.w = btn_start_game.sprite.surface->w;
	btn_start_game.rect.h = btn_start_game.sprite.surface->h;
	btn_start_game.rect.x = mnu_main.rect.x;
	btn_start_game.rect.y = mnu_main.rect.y;
	btn_start_game.func_click = btn_start_game_click;
	btn_start_game.data_click = &menu_data;

	String_copy_cstr(&btn_editor.text, "Editor ->");
	Button_update_sprite(&btn_editor);
	btn_editor.rect.w = btn_editor.sprite.surface->w;
	btn_editor.rect.h = btn_editor.sprite.surface->h;
	btn_editor.rect.x = mnu_main.rect.x;
	btn_editor.rect.y = btn_start_game.rect.y + btn_start_game.rect.h;
	btn_editor.func_click = btn_editor_click;
	btn_editor.data_click = &menu_data;

	String_copy_cstr(&btn_settings.text, "Settings ->");
	Button_update_sprite(&btn_settings);
	btn_settings.rect.w = btn_settings.sprite.surface->w;
	btn_settings.rect.h = btn_settings.sprite.surface->h;
	btn_settings.rect.x = mnu_main.rect.x;
	btn_settings.rect.y = btn_editor.rect.y + btn_editor.rect.h;
	btn_settings.func_click = btn_settings_click;
	btn_settings.data_click = &btn_settings_data;

	String_copy_cstr(&btn_exit.text, "Exit");
	Button_update_sprite(&btn_exit);
	btn_exit.rect.w = btn_exit.sprite.surface->w;
	btn_exit.rect.h = btn_exit.sprite.surface->h;
	btn_exit.rect.x = mnu_main.rect.x;
	btn_exit.rect.y = btn_settings.rect.y + btn_settings.rect.h;
	btn_exit.func_click = btn_exit_click;
	btn_exit.data_click = &main_active;

	// mnu_start_game
	mnu_start_game.rect.x = MNU_SUB_X;
	mnu_start_game.rect.y = MNU_SUB_Y;
	mnu_start_game.rect.w = 0;
	mnu_start_game.rect.h = 0;
	mnu_start_game.visible = 0;

	String_copy_cstr(&btn_start_game_close.text, "<- Main");
	Button_update_sprite(&btn_start_game_close);
	btn_start_game_close.rect.w = btn_start_game_close.sprite.surface->w;
	btn_start_game_close.rect.h = btn_start_game_close.sprite.surface->h;
	btn_start_game_close.rect.x = mnu_start_game.rect.x;
	btn_start_game_close.rect.y = mnu_start_game.rect.y;
	btn_start_game_close.func_click = btn_start_game_close_click;
	btn_start_game_close.data_click = &menu_data;

	String_copy_cstr(&btn_chapter1.text, "Chapter 1: Test");
	Button_update_sprite(&btn_chapter1);
	btn_chapter1.rect.w = btn_chapter1.sprite.surface->w;
	btn_chapter1.rect.h = btn_chapter1.sprite.surface->h;
	btn_chapter1.rect.x = mnu_start_game.rect.x;
	btn_chapter1.rect.y =
	    btn_start_game_close.rect.y + btn_start_game_close.rect.h;
	btn_chapter1.func_click = btn_chapter1_click;
	btn_chapter1.data_click = &game;

	// mnu_editor
	mnu_editor.rect.x = MNU_SUB_X;
	mnu_editor.rect.y = MNU_SUB_Y;
	mnu_editor.rect.w = 0;
	mnu_editor.rect.h = 0;
	mnu_editor.visible = 0;

	String_copy_cstr(&btn_editor_close.text, "<- Main");
	Button_update_sprite(&btn_editor_close);
	btn_editor_close.rect.w = btn_editor_close.sprite.surface->w;
	btn_editor_close.rect.h = btn_editor_close.sprite.surface->h;
	btn_editor_close.rect.x = mnu_editor.rect.x;
	btn_editor_close.rect.y = mnu_editor.rect.y;
	btn_editor_close.func_click = btn_editor_close_click;
	btn_editor_close.data_click = &menu_data;

	String_copy_cstr(&lbl_edit_name.text, "Name:");
	Label_update_sprite(&lbl_edit_name);
	lbl_edit_name.rect.w = lbl_edit_name.sprite.surface->w;
	lbl_edit_name.rect.h = lbl_edit_name.sprite.surface->h;
	lbl_edit_name.rect.x = mnu_editor.rect.x;
	lbl_edit_name.rect.y =
	    btn_editor_close.rect.y + btn_editor_close.rect.h;

	txt_edit_name.rect.w = 200;
	txt_edit_name.rect.h = FONT_SIZE + 4;
	txt_edit_name.rect.x = lbl_edit_name.rect.x + lbl_edit_name.rect.w;
	txt_edit_name.rect.y = lbl_edit_name.rect.y;

	String_copy_cstr(&lbl_edit_width.text, "Width:");
	Label_update_sprite(&lbl_edit_width);
	lbl_edit_width.rect.w = lbl_edit_width.sprite.surface->w;
	lbl_edit_width.rect.h = lbl_edit_width.sprite.surface->h;
	lbl_edit_width.rect.x = mnu_editor.rect.x;
	lbl_edit_width.rect.y = lbl_edit_name.rect.y + lbl_edit_name.rect.h;

	txt_edit_width.rect.w = 200;
	txt_edit_width.rect.h = FONT_SIZE + 4;
	txt_edit_width.rect.x = lbl_edit_width.rect.x + lbl_edit_width.rect.w;
	txt_edit_width.rect.y = lbl_edit_width.rect.y;

	String_copy_cstr(&lbl_edit_height.text, "Height:");
	Label_update_sprite(&lbl_edit_height);
	lbl_edit_height.rect.w = lbl_edit_height.sprite.surface->w;
	lbl_edit_height.rect.h = lbl_edit_height.sprite.surface->h;
	lbl_edit_height.rect.x = mnu_editor.rect.x;
	lbl_edit_height.rect.y = lbl_edit_width.rect.y + lbl_edit_width.rect.h;

	txt_edit_height.rect.w = 200;
	txt_edit_height.rect.h = FONT_SIZE + 4;
	txt_edit_height.rect.x =
	    lbl_edit_height.rect.x + lbl_edit_height.rect.w;
	txt_edit_height.rect.y = lbl_edit_height.rect.y;

	String_copy_cstr(&btn_start_edit.text, "Start");
	Button_update_sprite(&btn_start_edit);
	btn_start_edit.rect.w = btn_start_edit.sprite.surface->w;
	btn_start_edit.rect.h = btn_start_edit.sprite.surface->h;
	btn_start_edit.rect.x = mnu_editor.rect.x;
	btn_start_edit.rect.y = lbl_edit_height.rect.y + lbl_edit_height.rect.h;
	btn_start_edit.func_click = btn_start_edit_click;
	btn_start_edit.data_click = &btn_start_edit_data;

	// mnu_settings
	mnu_settings.rect.x = MNU_SUB_X;
	mnu_settings.rect.y = MNU_SUB_Y;
	mnu_settings.rect.w = 0;
	mnu_settings.rect.h = 0;
	mnu_settings.visible = 0;

	String_copy_cstr(&btn_settings_close.text, "<- Main");
	Button_update_sprite(&btn_settings_close);
	btn_settings_close.rect.w = btn_settings_close.sprite.surface->w;
	btn_settings_close.rect.h = btn_settings_close.sprite.surface->h;
	btn_settings_close.rect.x = mnu_settings.rect.x;
	btn_settings_close.rect.y = mnu_settings.rect.y;
	btn_settings_close.func_click = btn_settings_close_click;
	btn_settings_close.data_click = &btn_settings_data;

	String_copy_cstr(&lbl_gfx_window_w.text, "Resolution X:");
	Label_update_sprite(&lbl_gfx_window_w);
	lbl_gfx_window_w.rect.w = lbl_gfx_window_w.sprite.surface->w;
	lbl_gfx_window_w.rect.h = lbl_gfx_window_w.sprite.surface->h;
	lbl_gfx_window_w.rect.x = mnu_settings.rect.x;
	lbl_gfx_window_w.rect.y =
	    btn_settings_close.rect.y + btn_settings_close.rect.h;

	txt_gfx_window_w.rect.w = 200;
	txt_gfx_window_w.rect.h = FONT_SIZE + 4;
	txt_gfx_window_w.rect.x =
	    lbl_gfx_window_w.rect.x + lbl_gfx_window_w.rect.w;
	txt_gfx_window_w.rect.y = lbl_gfx_window_w.rect.y;

	String_copy_cstr(&lbl_gfx_window_h.text, "Resolution Y:");
	Label_update_sprite(&lbl_gfx_window_h);
	lbl_gfx_window_h.rect.w = lbl_gfx_window_h.sprite.surface->w;
	lbl_gfx_window_h.rect.h = lbl_gfx_window_h.sprite.surface->h;
	lbl_gfx_window_h.rect.x = mnu_settings.rect.x;
	lbl_gfx_window_h.rect.y =
	    lbl_gfx_window_w.rect.y + lbl_gfx_window_w.rect.h;

	txt_gfx_window_h.rect.w = 200;
	txt_gfx_window_h.rect.h = FONT_SIZE + 4;
	txt_gfx_window_h.rect.x =
	    lbl_gfx_window_h.rect.x + lbl_gfx_window_h.rect.w;
	txt_gfx_window_h.rect.y = lbl_gfx_window_h.rect.y;

	String_copy_cstr(&lbl_gfx_window_fullscreen.text, "Fullscreen:");
	Label_update_sprite(&lbl_gfx_window_fullscreen);
	lbl_gfx_window_fullscreen.rect.w = lbl_gfx_window_h.sprite.surface->w;
	lbl_gfx_window_fullscreen.rect.h = lbl_gfx_window_h.sprite.surface->h;
	lbl_gfx_window_fullscreen.rect.x = mnu_settings.rect.x;
	lbl_gfx_window_fullscreen.rect.y =
	    lbl_gfx_window_h.rect.y + lbl_gfx_window_w.rect.h;

	txt_gfx_window_fullscreen.rect.w = 20;
	txt_gfx_window_fullscreen.rect.h = FONT_SIZE + 4;
	txt_gfx_window_fullscreen.rect.x =
	    lbl_gfx_window_fullscreen.rect.x + lbl_gfx_window_fullscreen.rect.w;
	txt_gfx_window_fullscreen.rect.y = lbl_gfx_window_fullscreen.rect.y;

	// mnu_license
	mnu_license.rect.x = MNU_SUB_X;
	mnu_license.rect.y = MNU_SUB_Y;
	mnu_license.rect.w = 0;
	mnu_license.rect.h = 0;
	mnu_license.visible = 0;

	String_copy_cstr(&btn_license_close.text, "<- Main");
	Button_update_sprite(&btn_license_close);
	btn_license_close.rect.w = btn_license_close.sprite.surface->w;
	btn_license_close.rect.h = btn_license_close.sprite.surface->h;
	btn_license_close.rect.x = mnu_license.rect.x;
	btn_license_close.rect.y = mnu_license.rect.y;
	btn_license_close.func_click = btn_license_close_click;
	btn_license_close.data_click = &menu_data;

	String_copy_cstr(&lbl_license.text, APP_NAME);
	String_append_cstr(&lbl_license.text, APP_VERSION);
	String_append_cstr(&lbl_license.text, " is licensed under the ");
	String_append_cstr(&lbl_license.text, APP_LICENSE);
	lbl_license.text.len = strlen(btn_license_close.text.str) - 1;
	Label_update_sprite(&lbl_license);
	lbl_license.rect.w = lbl_license.sprite.surface->w;
	lbl_license.rect.h = lbl_license.sprite.surface->h;
	lbl_license.rect.x = mnu_license.rect.x;
	lbl_license.rect.y =
	    btn_license_close.rect.y + btn_license_close.rect.h;

	String_copy_cstr(&lbl_notice1.text, APP_LICENSE_SOURCE1);
	Label_update_sprite(&lbl_notice1);
	lbl_notice1.rect.w = lbl_notice1.sprite.surface->w;
	lbl_notice1.rect.h = lbl_notice1.sprite.surface->h;
	lbl_notice1.rect.x = mnu_license.rect.x;
	lbl_notice1.rect.y = lbl_license.rect.y + lbl_license.rect.h;

	String_copy_cstr(&lbl_notice2.text, APP_LICENSE_SOURCE2);
	Label_update_sprite(&lbl_notice2);
	lbl_notice2.rect.w = lbl_notice2.sprite.surface->w;
	lbl_notice2.rect.h = lbl_notice2.sprite.surface->h;
	lbl_notice2.rect.x = mnu_license.rect.x;
	lbl_notice2.rect.y = lbl_notice1.rect.y + lbl_notice1.rect.h;

	String_copy_cstr(&lbl_source1.text,
			    "The source code of this program is available at");
	Label_update_sprite(&lbl_source1);
	lbl_source1.rect.w = lbl_source1.sprite.surface->w;
	lbl_source1.rect.h = lbl_source1.sprite.surface->h;
	lbl_source1.rect.x = mnu_license.rect.x;
	lbl_source1.rect.y = lbl_notice2.rect.y + lbl_notice2.rect.h;

	String_copy_cstr(&lbl_source2.text, APP_SOURCE);
	Label_update_sprite(&lbl_source2);
	lbl_source2.rect.w = lbl_source2.sprite.surface->w;
	lbl_source2.rect.h = lbl_source2.sprite.surface->h;
	lbl_source2.rect.x = mnu_license.rect.x;
	lbl_source2.rect.y = lbl_source1.rect.y + lbl_source1.rect.h;

	// mainloop
	f32 ts_draw = 0.0f, ts_now;

	while (main_active) {
		// process events
		while (SDL_PollEvent(&event)) {
			// menu events
			Menu_handle_event(&mnu_master, &event);
			Menu_handle_event(&mnu_main, &event);
			Menu_handle_event(&mnu_start_game, &event);
			Menu_handle_event(&mnu_editor, &event);
			Menu_handle_event(&mnu_settings, &event);
			Menu_handle_event(&mnu_license, &event);

			// app events
			switch (event.type) {
			case SDL_QUIT:
				main_active = 0;
				break;
			}
		}

		ts_now = now();

		// draw menus
		if (ts_now > ts_draw + (1.0f / MENU_FRAMERATE)) {
			Menu_draw(&mnu_master);
			Menu_draw(&mnu_main);
			Menu_draw(&mnu_start_game);
			Menu_draw(&mnu_editor);
			Menu_draw(&mnu_settings);
			Menu_draw(&mnu_license);

			SDL_RenderPresent(renderer);

			ts_draw = now();
		}
	}

	// save config
	Config_to_file(&cfg);

 main_clear:

	// clear strings
	String_clear(&window_title);
	String_clear(&msg);

	// clear menus
	Menu_clear(&mnu_master);
	Menu_clear(&mnu_main);
	Menu_clear(&mnu_start_game);
	Menu_clear(&mnu_editor);
	Menu_clear(&mnu_settings);
	Menu_clear(&mnu_license);

	// quit TTF
	TTF_Quit();

	// quit SDL
	SDL_Quit();

	//close log file
	fclose(logfile);

	return 0;
}
