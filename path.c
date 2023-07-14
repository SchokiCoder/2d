/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <SM_log.h>
#include "app.h"
#include "path.h"

int32_t get_base_path(SM_String * out)
{
	int32_t rc;

	// get path
	SM_String_append_cstr(out, getenv("HOME"));
	SM_String_append_cstr(out, SLASH);
	SM_String_append_cstr(out, ".");
	SM_String_append_cstr(out, APP_NAME);
	SM_String_append_cstr(out, SLASH);

	/* in case, create dir */
	errno = 0;

#ifdef _WIN32
	rc = mkdir(out->str);
#else
	rc = mkdir(out->str, S_IRWXU);
#endif

	if (rc == -1) {
		if (errno != EEXIST) {
			SM_log_err("Could not get userfiles path.");
			return 1;
		}
	}

	return 0;
}

int32_t get_world_path(SM_String * out)
{
	int32_t rc;

	/* get base path */
	rc = get_base_path(out);

	if (rc != 0)
		return rc;

	/* get path */
	SM_String_append_cstr(out, PATH_WORLDS);
	SM_String_append_cstr(out, SLASH);

	/* in case, create dir */
	errno = 0;

#ifdef _WIN32
	rc = mkdir(out->str);
#else
	rc = mkdir(out->str, S_IRWXU);
#endif

	if (rc == -1) {
		if (errno != EEXIST) {
			SM_log_err("Could not create worlds directory.");
			return 1;
		}
	}

	return 0;
}

int32_t get_config_path(SM_String * out)
{
	int32_t rc;

	/* get base path */
	rc = get_base_path(out);

	if (rc != 0)
		return rc;

	/* get path */
	SM_String_append_cstr(out, PATH_CONFIG);

	return 0;
}

bool file_check_existence(const char *path)
{
	FILE *f = fopen(path, "r");

	if (f == NULL)
		return false;

	fclose(f);

	return true;
}
