/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "engine/log.h"
#include "path.h"

i32 get_base_path(struct String *out)
{
	i32 ret;

	// get path
	String_append_cstr(out, getenv("HOME"));
	String_append_cstr(out, SLASH);
	String_append_cstr(out, ".");
	String_append_cstr(out, APP_NAME);
	String_append_cstr(out, SLASH);

	/* in case, create dir */
	errno = 0;

#ifdef _WIN32
	ret = mkdir(out->str);
#else
	ret = mkdir(out->str, S_IRWXU);
#endif

	if (ret == -1) {
		if (errno != EEXIST) {
			log_err("Could not get userfiles path.");
			return 1;
		}
	}

	return 0;
}

i32 get_world_path(struct String *out)
{
	i32 ret;

	/* get base path */
	ret = get_base_path(out);

	if (ret != 0)
		return ret;

	/* get path */
	String_append_cstr(out, PATH_WORLDS);
	String_append_cstr(out, SLASH);

	/* in case, create dir */
	errno = 0;

#ifdef _WIN32
	ret = mkdir(out->str);
#else
	ret = mkdir(out->str, S_IRWXU);
#endif

	if (ret == -1) {
		if (errno != EEXIST) {
			log_err("Could not create worlds directory.");
			return 1;
		}
	}

	return 0;
}

i32 get_config_path(struct String *out)
{
	i32 ret;

	/* get base path */
	ret = get_base_path(out);

	if (ret != 0)
		return ret;

	/* get path */
	String_append_cstr(out, PATH_CONFIG);

	return 0;
}

int file_check_existence(const char *path)
{
	FILE *f = fopen(path, "r");

	if (f == NULL)
		return 0;

	fclose(f);

	return 1;
}
