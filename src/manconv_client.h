/*
 * manconv_client.h: interface to using manconv in a pipeline
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

#include "pipeline.h"

#include "decompress.h"

#include "sandbox.h"

extern man_sandbox *sandbox;

void add_manconv (struct pipeline *p, const char *source_encoding,
                  const char *target_encoding);
int manconv_inprocess (decompress *d, const char *source_encoding,
                       const char *target_encoding);
