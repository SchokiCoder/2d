/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include "physics.h"
#include "types.h"

int fpoint_within_frect(struct FPoint *pt, struct FRect *rect)
{
	if (pt->x > rect->x && pt->x < (rect->x + rect->w) &&
	    pt->y > rect->y && pt->y < (rect->y + rect->h)) {
		return 1;
	}

	return 0;
}

int ipoint_within_irect(struct IPoint *pt, struct IRect *rect)
{
	if (pt->x > rect->x && pt->x < (rect->x + rect->w) &&
	    pt->y > rect->y && pt->y < (rect->y + rect->h)) {
		return 1;
	}

	return 0;
}

int frect_within_frect(struct FRect *a, struct FRect *b)
{
	struct FPoint pt;

	// check if box 1 within box 2
	pt.x = a->x;
	pt.y = a->y;

	if (fpoint_within_frect(&pt, b))
		return 1;

	pt.x = a->x + a->w;
	pt.y = a->y;

	if (fpoint_within_frect(&pt, b))
		return 1;

	pt.x = a->x;
	pt.y = a->y + a->h;

	if (fpoint_within_frect(&pt, b))
		return 1;

	pt.x = a->x + a->w;
	pt.y = a->y + a->h;

	if (fpoint_within_frect(&pt, b))
		return 1;

	// check if box 2 within box 1
	pt.x = b->x;
	pt.y = b->y;

	if (fpoint_within_frect(&pt, a))
		return 1;

	pt.x = b->x + a->w;
	pt.y = b->y;

	if (fpoint_within_frect(&pt, a))
		return 1;

	pt.x = b->x;
	pt.y = b->y + a->h;

	if (fpoint_within_frect(&pt, a))
		return 1;

	pt.x = b->x + a->w;
	pt.y = b->y + a->h;

	if (fpoint_within_frect(&pt, a))
		return 1;

	return 0;
}

int irect_within_irect(struct IRect *a, struct IRect *b)
{
	struct IPoint pt;

	// check if box 1 within box 2
	pt.x = a->x;
	pt.y = a->y;

	if (ipoint_within_irect(&pt, b))
		return 1;

	pt.x = a->x + a->w;
	pt.y = a->y;

	if (ipoint_within_irect(&pt, b))
		return 1;

	pt.x = a->x;
	pt.y = a->y + a->h;

	if (ipoint_within_irect(&pt, b))
		return 1;

	pt.x = a->x + a->w;
	pt.y = a->y + a->h;

	if (ipoint_within_irect(&pt, b))
		return 1;

	// check if box 2 within box 1
	pt.x = b->x;
	pt.y = b->y;

	if (ipoint_within_irect(&pt, a))
		return 1;

	pt.x = b->x + a->w;
	pt.y = b->y;

	if (ipoint_within_irect(&pt, a))
		return 1;

	pt.x = b->x;
	pt.y = b->y + a->h;

	if (ipoint_within_irect(&pt, a))
		return 1;

	pt.x = b->x + a->w;
	pt.y = b->y + a->h;

	if (ipoint_within_irect(&pt, a))
		return 1;

	return 0;
}
