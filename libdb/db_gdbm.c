/*
 * db_gdbm.c: low level gdbm interface routines for man.
 *
 * Copyright (C) 1994, 1995 Graeme W. Wilford. (Wilf.)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Mon Aug  8 20:35:30 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk)
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef GDBM

#  include <setjmp.h>
#  include <stdbool.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>

#  include "stat-time.h"
#  include "timespec.h"
#  include "xalloc.h"

#  include "manconfig.h"

#  include "cleanup.h"
#  include "debug.h"

#  include "db_xdbm.h"
#  include "mydbm.h"

/* setjmp/longjmp handling to defend against _gdbm_fatal exiting under our
 * feet.  Not thread-safe, but there is no plan for man-db to ever use
 * threads.
 */
static jmp_buf open_env;
static bool opening;

/* Mimic _gdbm_fatal's error output, but handle errors during open more
 * gracefully than exiting.
 */
static void trap_error (const char *val)
{
	if (opening) {
		debug ("gdbm error: %s\n", val);
		longjmp (open_env, 1);
	} else
		fprintf (stderr, "gdbm fatal: %s\n", val);
}

man_gdbm_wrapper man_gdbm_new (const char *name)
{
	man_gdbm_wrapper wrap;

	wrap = xmalloc (sizeof *wrap);
	wrap->name = xstrdup (name);
	wrap->file = NULL;
	wrap->mtime = NULL;

	return wrap;
}

bool man_gdbm_open_wrapper (man_gdbm_wrapper wrap, int flags)
{
	datum key, content;

	opening = true;
	if (setjmp (open_env))
		return false;
	wrap->file =
	        gdbm_open (wrap->name, BLK_SIZE, flags, DBMODE, trap_error);
	if (!wrap->file)
		return false;

	if ((flags & ~GDBM_FAST) != GDBM_NEWDB) {
		/* While the setjmp/longjmp guard is in effect, make sure we
		 * can read from the database at all.
		 */
		memset (&key, 0, sizeof key);
		MYDBM_SET (key, xstrdup (VER_KEY));
		content = MYDBM_FETCH (wrap, key);
		MYDBM_FREE_DPTR (key);
		MYDBM_FREE_DPTR (content);
	}

	opening = false;

	return true;
}

static datum unsorted_firstkey (man_gdbm_wrapper wrap)
{
	return gdbm_firstkey (wrap->file);
}

static datum unsorted_nextkey (man_gdbm_wrapper wrap, datum key)
{
	return gdbm_nextkey (wrap->file, key);
}

datum man_gdbm_firstkey (man_gdbm_wrapper wrap)
{
	return man_xdbm_firstkey (wrap, unsorted_firstkey, unsorted_nextkey);
}

datum man_gdbm_nextkey (man_gdbm_wrapper wrap, datum key)
{
	return man_xdbm_nextkey (wrap, key);
}

struct timespec man_gdbm_get_time (man_gdbm_wrapper wrap)
{
	struct stat st;

	if (!wrap->mtime) {
		wrap->mtime = XMALLOC (struct timespec);
		if (fstat (gdbm_fdesc (wrap->file), &st) < 0) {
			wrap->mtime->tv_sec = -1;
			wrap->mtime->tv_nsec = -1;
		} else
			*wrap->mtime = get_stat_mtime (&st);
	}

	return *wrap->mtime;
}

static void raw_close (man_gdbm_wrapper wrap)
{
	if (wrap->file)
		gdbm_close (wrap->file);
}

void man_gdbm_free (man_gdbm_wrapper wrap)
{
	man_xdbm_free (wrap, raw_close);
}

#  ifndef HAVE_GDBM_EXISTS

int gdbm_exists (GDBM_FILE file, datum key)
{
	char *memory;

	memory = MYDBM_DPTR (gdbm_fetch (file, key));
	if (memory) {
		free (memory);
		return 1;
	}

	return 0;
}

#  endif /* !HAVE_GDBM_EXISTS */

#endif /* GDBM */
