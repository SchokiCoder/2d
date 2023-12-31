/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

#define PATH_LOG "log.txt"

extern FILE *logfile;

void log_open(void);

void log_err(const char *msg);

void log_warn(const char *msg);

void log_close(void);

#endif				/* _LOG_H */
