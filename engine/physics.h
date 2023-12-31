/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _PHYSICS_H
#define _PHYSICS_H

struct FPoint;
struct IPoint;
struct FRect;
struct IRect;

int fpoint_within_frect(struct FPoint *pt, struct FRect *rect);

int ipoint_within_irect(struct IPoint *pt, struct IRect *rect);

int frect_within_frect(struct FRect *a, struct FRect *b);

int irect_within_irect(struct IRect *a, struct IRect *b);

#endif				/* _PHYSICS_H */
