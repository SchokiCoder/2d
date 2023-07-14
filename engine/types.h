/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef signed long int isize;
typedef unsigned long int usize;

typedef float f32;
typedef double f64;

struct FPoint {
	f32 x;
	f32 y;
};

struct IPoint {
	i32 x;
	i32 y;
};

struct UPoint {
	u32 x;
	u32 y;
};

struct FRect {
	f32 x;
	f32 y;
	f32 w;
	f32 h;
};

struct IRect {
	i32 x;
	i32 y;
	i32 w;
	i32 h;
};

struct URect {
	u32 x;
	u32 y;
	u32 w;
	u32 h;
};

#endif				// _TYPES_H
