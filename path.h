/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PATH_H
#define PATH_H

#include <stdint.h>
#include <stdbool.h>
#include <SM_string.h>

#ifdef _WIN32
static const char SLASH[] = "\\";
#else
static const char SLASH[] = "/";
#endif				/* _WIN32 */

static const char PATH_WORLDS[] = "worlds";
static const char PATH_CONFIG[] = "config.cfg";
static const char PATH_TEXTURE_ICON[] = PATH_TEXTURES "icon.png";

static const char FILETYPE_WORLD[] = "wld";
static const char FILETYPE_BACKUP[] = "bkp";

int32_t get_base_path(SM_String * out);

int32_t get_world_path(SM_String * out);

int32_t get_config_path(SM_String * out);

bool file_check_existence(const char *path);

#endif				/* PATH_H */
