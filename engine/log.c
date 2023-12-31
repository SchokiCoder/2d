/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2023  Andy Frank Schoknecht
 */

#include <stdio.h>
#include <time.h>

#include "log.h"

FILE *logfile;

void log_open(void)
{
	logfile = fopen(PATH_LOG, "a");
	setbuf(logfile, NULL);
}

void log_time(void)
{
	time_t t = time(NULL);
	struct tm *dt;
	dt = localtime(&t);

	fprintf(logfile,
		"[%i-%02i-%02i %02i:%02i:%02i] ",
		dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday,
		dt->tm_hour, dt->tm_min, dt->tm_sec);
}

void log_err(const char *msg)
{
	log_time();
	fprintf(logfile, "ERROR: %s\n", msg);
}

void log_warn(const char *msg)
{
	log_time();
	fprintf(logfile, "WARNING: %s\n", msg);
}

void log_close(void)
{
	fclose(logfile);
}
