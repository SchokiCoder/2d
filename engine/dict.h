/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _DICT_H
#define _DICT_H

#include "sstring.h"
#include "types.h"

struct DictPair {
	usize key_djb2;
	struct String key;
	struct String value;
};

struct Dict {
	int invalid;
	usize len;
	usize size;
	struct DictPair *data;
};

struct Dict Dict_new(const usize inital_size);

struct Dict Dict_from_file(const char *filepath);

void Dict_add(struct Dict *dict, const char *restrict key, const char *restrict value);

int Dict_find(const struct Dict *dict, const char *key, usize *index);

int Dict_write(const struct Dict *dict, const char *filepath);

void Dict_clear(struct Dict *dict);

#endif				/* _DICT_H */
