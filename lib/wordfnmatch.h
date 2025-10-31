/*
 * wordfnmatch.h: interface to fnmatch on word boundaries
 *
 * Copyright (C) 2001, 2003, 2008 Colin Watson.
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

bool word_fnmatch (const char *lowpattern, const char *string);
