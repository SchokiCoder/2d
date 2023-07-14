/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _SSTRING_H
#define _SSTRING_H

#include "types.h"

static const usize STRING_IMPLICIT_INITIAL_SIZE = 8;

int strequal(const char *restrict a, const char *restrict b);

struct String {
	size_t len;
	size_t size;
	char *str;
};

struct String String_new(const usize initial_size);

struct String String_from(const char *cstr);

struct String String_contain(const char *cstr);

void String_copy(struct String *restrict dest, struct String *restrict src);

void
String_append(struct String *restrict dest,
              struct String *restrict addendum);

void String_copy_cstr(struct String *restrict dest, const char *restrict src);

void
String_append_cstr(struct String *restrict dest,
                   const char *restrict addendum);

int
String_equal(const struct String *restrict a,
             const struct String *restrict b);

void String_empty(struct String *str);

void String_clear(struct String *str);

#endif				/* _SSTRING_H */
