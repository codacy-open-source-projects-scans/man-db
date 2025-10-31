/*
 * compression.h: interface to finding decompressor / compression extension
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
 * along with man-db.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdbool.h>

struct compression {
	/* The following are const because they should be pointers to parts
	 * of strings allocated elsewhere and should not be written through
	 * or freed themselves.
	 */
	const char *prog;
	const char *ext;
	/* The following should be freed when discarding an instance of this
	 * structure.
	 */
	char *stem;
};

extern struct compression comp_list[];

extern struct compression *comp_info (const char *filename, bool want_stem);
extern struct compression *comp_file (const char *filename);
