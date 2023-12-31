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
	struct String result = {
		.len = 0,
		.str = malloc(inital_size),
		.size = inital_size
	};

	return result;
}

struct String String_from(const char *cstr)
{
	struct String result = String_new(STRING_IMPLICIT_INITIAL_SIZE);
	const size_t cstrlen = strlen(cstr);

	struct String temp = {
		.str = (char *)cstr,
		.len = cstrlen - 1,
		.size = cstrlen
	};

	String_copy(&result, &temp);

	return result;
}

struct String String_contain(const char *cstr)
{
	const size_t cstrlen = strlen(cstr);
	const struct String result = {
		.len = cstrlen - 1,
		.size = 0,
		.str = (char *)cstr
	};

	return result;
}

void String_copy(struct String *restrict dest, struct String *restrict src)
{
	String_ensure_size(dest, src->len + 1);

	strncpy(dest->str, src->str, src->len + 1);

	dest->len = src->len;
}

void
String_append(struct String *restrict dest,
              struct String *restrict addendum)
{
	String_ensure_size(dest, dest->len + addendum->len + 1);

	strncpy(&dest->str[dest->len], addendum->str, addendum->len + 1);

	dest->len += addendum->len;
}

void String_copy_cstr(struct String *restrict dest, const char *restrict src)
{
	const usize src_len = strlen(src);
	String_ensure_size(dest, src_len);

	strncpy(dest->str, src, src_len);

	dest->len = src_len - 1;
}

void String_append_cstr(struct String *restrict dest,
			   const char *restrict addendum)
{
	const usize add_len = strlen(addendum);
	String_ensure_size(dest, dest->len + add_len);

	strncpy(&dest->str[dest->len], addendum, add_len);

	dest->len += add_len - 1;
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
