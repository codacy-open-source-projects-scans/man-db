/*
 * lexgrog.h: interface to extracting whatis info
 *
 * Copyright (C) 1994, 1995 Graeme W. Wilford. (Wilf.)
 * Copyright (C) 2001-2022 Colin Watson.
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

#define MANPAGE 0
#define CATPAGE 1

#include "decompress.h"

#include "sandbox.h"

typedef struct lexgrog {
	int type;
	char *whatis;
	char *filters;
} lexgrog;

extern man_sandbox *sandbox;

extern int find_name (const char *file, const char *filename, lexgrog *p_lg,
                      const char *encoding);
extern int find_name_decompressed (decompress *d, const char *filename,
                                   lexgrog *p_lg);
