/*
 * manconv.h: interface to converting manual page from one encoding to another
 *
 * Copyright (C) 2008 Colin Watson.
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

#include "gl_list.h"

#include "decompress.h"

struct manconv_outbuf {
	char *buf;
	size_t len, max;
};

char *check_preprocessor_encoding (decompress *decomp, const char *to_code,
                                   char **modified_line);
int manconv (decompress *decomp, gl_list_t from, const char *to,
             struct manconv_outbuf *outbuf);
