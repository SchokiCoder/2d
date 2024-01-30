/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _SSTRING_H
#define _SSTRING_H

#include <stddef.h>

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

void
String_copy(struct String       *string,
            const char          *src,
            const long unsigned  src_len);

void
String_append(struct String       *string,
              const char          *src,
              const long unsigned  src_len);

int
String_equal(const struct String *restrict a,
             const struct String *restrict b);

void String_empty(struct String *str);

void String_clear(struct String *str);

#endif				/* _SSTRING_H */
