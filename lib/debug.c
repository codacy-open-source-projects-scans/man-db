/*
 * debug.c: debugging messages
 *
 * Copyright (C) 2007 Colin Watson.
 *
 * This file is part of man-db.
 *
 * man-db is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * man-db is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with man-db; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attribute.h"

#include "manconfig.h"

#include "debug.h"

bool debug_level = false;

void init_debug (void)
{
	const char *man_debug = getenv ("MAN_DEBUG");
	if (man_debug && STREQ (man_debug, "1"))
		debug_level = true;
}

static void ATTRIBUTE_FORMAT ((__printf__, 1, 0))
        vdebug (const char *message, va_list args)
{
	if (debug_level)
		vfprintf (stderr, message, args);
}

void debug (const char *message, ...)
{
	if (debug_level) {
		va_list args;

		va_start (args, message);
		vdebug (message, args);
		va_end (args);
	}
}

void debug_error (const char *message, ...)
{
	if (debug_level) {
		va_list args;

		va_start (args, message);
		vdebug (message, args);
		va_end (args);

		debug (": %s\n", strerror (errno));
	}
}
