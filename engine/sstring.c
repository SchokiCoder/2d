/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <stdlib.h>
#include <string.h>

#include "sstring.h"
#include "types.h"

int strequal(const char *restrict a, const char *restrict b)
{
	const usize a_len = strlen(a);
	const usize b_len = strlen(b);

	if (a_len != b_len)
		return 0;

	for (usize i = 0; i < a_len; i++) {
		if (a[i] != b[i]) {
			return 0;
		}
	}

	return 1;
}

void String_grow(struct String *str)
{
	str->size *= 2;
	str->str = realloc(str->str, str->size);
}

void String_ensure_size(struct String *str, usize size)
{
	while (str->size < size)
		String_grow(str);
}

struct String String_new(const usize inital_size)
{
	struct String ret = {
		.size = inital_size,
		.len = 0,
		.str = malloc(inital_size)
	};
	
	ret.str[0] = '\0';
	
	return ret;
}

struct String String_from(const char *cstr)
{
	struct String ret = String_new(STRING_IMPLICIT_INITIAL_SIZE);

	String_copy(&ret, cstr, strlen(cstr));

	return ret;
}

struct String String_contain(const char *cstr)
{
	const size_t cstrlen = strlen(cstr);
	const struct String ret = {
		.len = cstrlen - 1,
		.size = 0,
		.str = (char *)cstr
	};

	return ret;
}

void
String_copy(struct String       *string,
            const char          *src,
            const long unsigned  src_len)
{
	string->len = 0;
	String_append(string, src, src_len);
}

void
String_append(struct String       *string,
              const char          *src,
              const long unsigned  src_len)
{
	const long unsigned new_len = src_len + string->len;
	
	while ((new_len + 1) > string->size) {
		String_grow(string);
	}
	
	strncpy(&string->str[string->len], src, src_len);
	string->len = new_len;
	string->str[string->len] = '\0';
}

int
String_equal(const struct String *restrict a,
             const struct String *restrict b)
{
	if (a->len != b->len)
		return 0;

	for (usize i = 0; i < a->len; i++)
		if (a->str[i] != b->str[i])
			return 0;

	return 1;
}

void String_empty(struct String *str)
{
	str->str[0] = '\0';
	str->len = 0;
}

void String_clear(struct String *str)
{
	free(str->str);
	str->len = 0;
	str->size = 0;
}
