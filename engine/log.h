/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
