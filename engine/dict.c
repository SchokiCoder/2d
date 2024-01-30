/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dict.h"

usize djb2_encode(const char *str)
{
	// credits to Daniel J. Bernstein for this algorithm
	usize result = 5381;
	char temp;

	while ((temp = *(str++)))
		result += ((result << 5) + result) + temp;

	return result;
}

struct DictPair DictPair_new(const char *restrict key, const char *restrict value)
{
	struct DictPair result = {
		.key_djb2 = djb2_encode(key),
		.key = String_from(key),
		.value = String_from(value),
	};

	return result;
}

void DictPair_clear(struct DictPair *dictpair)
{
	String_clear(&dictpair->key);
	String_clear(&dictpair->value);
}

void Dict_grow(struct Dict *dict)
{
	dict->size *= 2;
	dict->data = realloc(dict->data, sizeof(struct DictPair) *dict->size);
}

void Dict_ensure_size(struct Dict *dict, usize size)
{
	while (dict->size < size)
		Dict_grow(dict);
}

struct Dict Dict_new(const usize inital_size)
{
	struct Dict result = {
		.invalid = 0,
		.len = 0,
		.size = inital_size,
		.data = malloc(sizeof(struct DictPair) *inital_size),
	};

	return result;
}

struct Dict Dict_from_file(const char *filepath)
{
	FILE *f;
	struct Dict dict = Dict_new(1);

	// open file
	f = fopen(filepath, "r");

	if (f == NULL) {
		dict.invalid = 1;
		return dict;
	}
	// read each character
	char buf[2] = "\0\0";
	int read_key = 1;
	struct String key = String_new(8);
	struct String value = String_new(8);

	while ((buf[0] = fgetc(f)) != EOF) {
		switch (buf[0]) {
		case ' ':
			// ignore spaces
			break;

		case '=':
			// set to read value
			read_key = 0;
			break;

		case '\n':
			// save pair
			Dict_add(&dict, key.str, value.str);

			// set to read key, reset strings
			read_key = 1;
			String_empty(&key);
			String_empty(&value);
			break;

		default:
			// append character to key or value
			if (read_key == 1)
				String_append(&key, buf, strlen(buf));
			else
				String_append(&value, buf, strlen(buf));
		}
	}

	fclose(f);
	String_clear(&key);
	String_clear(&value);
	return dict;
}

void Dict_add(struct Dict *dict, const char *restrict key,
		 const char *restrict value)
{
	Dict_ensure_size(dict, dict->len + 1);

	dict->data[dict->len] = DictPair_new(key, value);

	dict->len++;
}

int Dict_find(const struct Dict *dict, const char *key, usize *index)
{
	const usize key_djb2 = djb2_encode(key);

	for (usize i = 0; i < dict->len; i++) {
		if (dict->data[i].key_djb2 == key_djb2) {
			*index = i;
			return 1;
		}
	}

	return 0;
}

int Dict_to_file(const struct Dict *dict, const char *filepath)
{
	FILE *f;

	// open
	f = fopen(filepath, "w");

	if (f == NULL)
		return 0;

	// write
	for (usize i = 0; i < dict->len; i++) {
		fprintf(f, "%s", dict->data[i].key.str);
		fputs(" = ", f);
		fprintf(f, "%s", dict->data[i].value.str);
		fputc('\n', f);
	}

	fclose(f);
	return 1;
}

void Dict_clear(struct Dict *dict)
{
	for (usize i = 0; i < dict->len; i++)
		DictPair_clear(&dict->data[i]);

	free(dict->data);

	dict->size = 0;
	dict->len = 0;
}
