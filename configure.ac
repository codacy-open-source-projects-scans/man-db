dnl Process this file with autoconf to produce a configure script.
m4_pattern_forbid([^MAN_])

# Initialise and check we're in the correct directory.
AC_INIT([man-db], [2.13.0], [cjwatson@debian.org])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.14 -Wall -Wno-override -Werror foreign dist-xz no-dist-gzip parallel-tests])
AM_SILENT_RULES([yes])
AC_PREREQ([2.64])
AC_CONFIG_SRCDIR([src/man.c])
AC_USE_SYSTEM_EXTENSIONS
MAN_TAR_SORT_NAME

AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

# Define below date and version information to be put into man pages etc.
date=2024-08-29
AC_SUBST([date])dnl
roff_version=`echo AC_PACKAGE_VERSION | sed 's/-/\\-/g'`
AC_SUBST([roff_version])dnl

# Explicitly check for pkg-config early on, since otherwise the conditional
# calls in MAN_ARG_SYSTEMDTMPFILESDIR and MAN_ARG_SYSTEMDSYSTEMUNITDIR are
# problematic.
PKG_PROG_PKG_CONFIG

# We have to be a bit naughty here and supply options.
# The autoconf literature states that only features that can be separately
# 'built' should use --enable and friends. Oh well...
MAN_ARG_CACHE_OWNER
MAN_ARG_SETUID
MAN_ARG_UNDOC
MAN_ARG_DEVICE
MAN_ARG_DB
MAN_ARG_CONFIG_FILE
MAN_ARG_SECTIONS
MAN_ARG_AUTOMATIC_CREATE
MAN_ARG_AUTOMATIC_UPDATE
MAN_ARG_CATS
MAN_ARG_OVERRIDE_DIR
MAN_ARG_SYSTEMDTMPFILESDIR
MAN_ARG_SYSTEMDSYSTEMUNITDIR
MAN_ARG_MANDIRS
MAN_ARG_MANUAL
MAN_ARG_SNAPDIR

# Check $PATH for the following programs and append suitable options.
AC_PROG_CC
gl_EARLY
AC_PROG_CPP
CFLAGS="$CFLAGS -Wall"
case $host_os in
	ultrix4.3*)
		dnl When compiled for BSD environment, each running `man'
		dnl increases the system load as reported by uptime(1) by
		dnl one.  The reason for this behaviour is currently
		dnl unknown, but the load increase does *not* reflect actual
		dnl resource usage.  To avoid it, compile for POSIX
		dnl environment:
		CFLAGS="$CFLAGS -YPOSIX"
		;;
esac

AC_ARG_ENABLE([gcc-warnings],
  [AS_HELP_STRING([--enable-gcc-warnings=TYPE],
    [control generation of GCC warnings.  The TYPE 'no' disables
     warnings; 'yes' generates cheap warnings if available (default);
     'expensive' in addition generates expensive-to-compute warnings
     if available])],
  [case $enableval in
     no|yes|expensive) ;;
     *) AC_MSG_ERROR([bad value $enableval for gcc-warnings option]) ;;
   esac
   man_gcc_warnings=$enableval],
  [man_gcc_warnings=yes]
)
if test "$man_gcc_warnings" != no; then
	# Enable all reasonable GCC warnings.
	gl_MANYWARN_ALL_GCC([warnings])
	nw=
	nw="$nw -Wsystem-headers"
	nw="$nw -Wmissing-field-initializers"
	nw="$nw -Winline"
	if test "$man_gcc_warnings" != expensive; then
		nw="$nw -fanalyzer"
	fi
	gl_MANYWARN_COMPLEMENT([warnings], [$warnings], [$nw])
	for w in $warnings; do
		gl_WARN_ADD([$w])
	done
	gl_WARN_ADD([-Wno-missing-field-initializers])
	# Disable use of VLAs by Gnulib to avoid tripping over -Wvla.
	AC_DEFINE([GNULIB_NO_VLA], [1], [Define to 1 to disable use of VLAs.])
fi

AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_AR
LT_INIT([disable-static])

libman_export_ldflags=
if test "$with_gnu_ld" = yes; then
	case $host_os in
		cygwin*|mingw*|pw32*|cegcc*)
			libman_export_ldflags='-Wl,--export-all-symbols'
			;;
	esac
fi
AC_SUBST([LIBMAN_EXPORT_LDFLAGS], [$libman_export_ldflags])

AC_CHECK_PROGS([cat], [cat])
AC_DEFINE_UNQUOTED([PROG_CAT], ["$cat"], [Program to use as cat.])
MAN_CHECK_PROGS([browser], [BROWSER], [use BROWSER as default web browser], [www-browser lynx elinks w3m])
test -n "$browser" && browser="exec $browser"
AC_DEFINE_UNQUOTED([PROG_BROWSER], ["$browser"], [Program to use as browser.])
AC_CHECK_PROGS([tr], [tr])
AC_DEFINE_UNQUOTED([PROG_TR], ["$tr"], [Program to use as tr.])
AC_CHECK_PROGS([grep], [grep])
AC_DEFINE_UNQUOTED([PROG_GREP], ["$grep"], [Program to use as grep.])
MAN_CHECK_PROGS([pager], [PAGER], [use PAGER as default pager], [pager less more])
AC_DEFINE_UNQUOTED([PROG_PAGER], ["$pager"], [Program to use as pager.])

# Define below (in list of preference) *roff macros to check for.
macros="andoc an doc"

# We have problems here, as different systems have different *roff
# formatters and they accept different options and do different things :(
MAN_CHECK_PROGS([nroff], [NROFF], [use NROFF as roff formatter for character devices], [nroff gnroff groff])
if test -n "$nroff"
then
	MAN_PROG_GNU_NROFF([$nroff])
	if test "$man_cv_prog_gnu_nroff" != "yes"
	then
		MAN_PROG_HEIRLOOM_NROFF([$nroff])
	fi
	if test -n "$nroff_device"
	then
		AC_MSG_CHECKING([that nroff works with argument$nroff_device])
		# We cannot cache this result as it can change between runs
		# of configure.
		if $nroff $nroff_device </dev/null >/dev/null 2>&1 3>&1
		then
			AC_MSG_RESULT([yes])
		else
			AC_MSG_RESULT([no])
			AC_MSG_ERROR([nroff does not work with argument$nroff_device])
		fi
	fi
	AC_MSG_CHECKING([for appropriate nroff macro])
	AC_CACHE_VAL([man_cv_prog_nroff_macro], [

	for macro in $macros
	do
		if $nroff -m$macro $nroff_device </dev/null >/dev/null 2>&1 3>&1
		then
			man_cv_prog_nroff_macro=-m$macro
			break
		fi
	done])

	if test -n "$man_cv_prog_nroff_macro"
	then
		if test "$man_cv_prog_heirloom_nroff" = "yes"
		then
			# Heirloom works best with some extra options:
			#   -mg: groff compatibility
			#   -msafe: disable privileged operations
			#   -mpadj: clean up line breaking
			man_cv_prog_nroff_macro="-mg -msafe -mpadj ${man_cv_prog_nroff_macro}"
		fi
		nroff="$nroff ${man_cv_prog_nroff_macro}"
dnl		nroff="$nroff ${man_cv_prog_nroff_macro}${nroff_device}"
		AC_MSG_RESULT([$man_cv_prog_nroff_macro])
	else
		AC_MSG_RESULT([ambiguous])
		AC_MSG_WARN([please edit config.h and add nroff definition])
	fi
else
	AC_MSG_WARN([Cannot find an nroff-like program, formatting of manual page source will not be supported.])
	nroff="(nroff not installed)"
	AC_DEFINE([NROFF_MISSING], [1], [Define if you don't have nroff.])
fi
AC_DEFINE_UNQUOTED([PROG_NROFF], ["$nroff"], [Program to use as nroff.])

dnl It would be nice to use MAN_CHECK_PROGS here, but how do we determine
dnl TROFF_IS_GROFF?
AC_CHECK_PROGS([troff], [groff])
if test -n "$troff"
then
	troff_is_groff=yes
	AC_DEFINE([TROFF_IS_GROFF], [1], [Define if you are using groff as troff.])
	TROFF=groff
else
	troff_is_groff=no
	AC_CHECK_PROGS([troff], [troff gtroff])
	TROFF=troff
fi
AC_SUBST([troff_is_groff])
AC_SUBST([TROFF])
if test -n "$troff"
then
	AC_DEFINE([HAS_TROFF], [1], [Define if you have troff.])
	AC_MSG_CHECKING([for appropriate $TROFF options])
	AC_CACHE_VAL([man_cv_prog_troff_options], [
	# Do a quick test to see if -t works [AIX needs this], groff doesn't
	# as it indicates pre-process with tbl.
	test "$TROFF" = "troff" && $troff -t </dev/null >/dev/null 2>&1 3>&1 \
				&& troff_filter="-t "
	for macro in $macros
	do
		if $troff $troff_filter -m$macro </dev/null >/dev/null 2>&1 3>&1
		then
			man_cv_prog_troff_options="${troff_filter}-m${macro}"
			break
		fi
	done])
	if test -n "$man_cv_prog_troff_options"
	then
		if test "$man_cv_prog_heirloom_nroff" = "yes"
		then
			# Heirloom works best with some extra options:
			#   -mg: groff compatibility
			#   -msafe: disable privileged operations
			#   -mpadj: clean up line breaking
			man_cv_prog_troff_options="-mg -msafe -mpadj ${man_cv_prog_troff_options}"
		fi
		troff="$troff $man_cv_prog_troff_options"
		AC_MSG_RESULT([$man_cv_prog_troff_options])
	else
		AC_MSG_RESULT([ambiguous])
		AC_MSG_WARN([please edit config.h and add troff definition])
	fi
else
	troff="(troff not installed)"
fi
AC_DEFINE_UNQUOTED([PROG_TROFF], ["$troff"], [Program to use as troff.])
dnl We want to end up with "-" written as "\-" in manual pages.  This
dnl requires additional \-escaping corresponding to the following quote
dnl removal steps:
dnl   1) the following sed command
dnl   2) the following assignment
dnl   2) sed running on the substituted version of man/replace.sin.in
troff_as_troff_input=`echo "$troff" | sed 's/-/\\\\\\\\-/g'`
AC_SUBST([troff_as_troff_input])

if test -n "$nroff"
then
	AC_CACHE_CHECK([whether nroff supports warning control],
		       [man_cv_prog_nroff_warnings], [
		if test "x$troff_is_groff" = xyes && \
		   nroff -wmac </dev/null >/dev/null 2>&1
		then
			man_cv_prog_nroff_warnings=yes
		else
			man_cv_prog_nroff_warnings=no
		fi])
	if test "x$man_cv_prog_nroff_warnings" = xyes; then
		AC_DEFINE([NROFF_WARNINGS], [1],
			  [Define if nroff supports warning control.])
	fi
fi

MAN_CHECK_PROGS([eqn], [EQN], [use EQN to preprocess equations], [eqn geqn])
AC_DEFINE_UNQUOTED([PROG_EQN], ["$eqn"], [Program to use as eqn.])
MAN_CHECK_PROGS([neqn], [NEQN], [use NEQN to preprocess equations for character devices], [neqn gneqn])
# If we fail to find an neqn, use eqn and try to force it to output for an
# ascii device. As this is only relevant for equations (?), not using latin1
# should be acceptable. -Tlatin1 is ignored by some eqn implementations.
if test -z "$neqn"
then
	test -n "$eqn" &&
	(test -n "$nroff_device" && neqn="$eqn -T$nroff_device" || neqn="$eqn -Tascii")
fi
AC_DEFINE_UNQUOTED([PROG_NEQN], ["$neqn"], [Program to use as neqn.])
MAN_CHECK_PROGS([tbl], [TBL], [use TBL to preprocess tables], [tbl gtbl])
AC_DEFINE_UNQUOTED([PROG_TBL], ["$tbl"], [Program to use as tbl.])
TBL_X_FORMAT=
if test -n "$tbl"
then
	AC_CACHE_CHECK([whether tbl supports the 'x' format character],
		       [man_cv_tbl_x_format], [
		if (echo .TS; echo ';'; echo lx.; echo SENTINEL; echo .TE) | \
		   $tbl 2>/dev/null | grep SENTINEL >/dev/null 2>&1
		then
			man_cv_tbl_x_format=yes
		else
			man_cv_tbl_x_format=no
		fi])
	if test "x$man_cv_tbl_x_format" = xyes
	then
		TBL_X_FORMAT=x
	fi
fi
AC_SUBST([TBL_X_FORMAT])
MAN_CHECK_PROGS([col], [COL], [use COL to filter formatting characters from output], [col gcol])
AC_DEFINE_UNQUOTED([PROG_COL], ["$col"], [Program to use as col.])
MAN_CHECK_PROGS([vgrind], [VGRIND], [use VGRIND to preprocess program sources], [vgrind gvgrind])
AC_DEFINE_UNQUOTED([PROG_VGRIND], ["$vgrind"], [Program to use as vgrind.])
MAN_CHECK_PROGS([refer], [REFER], [use REFER to preprocess bibliographic references], [refer grefer])
AC_DEFINE_UNQUOTED([PROG_REFER], ["$refer"], [Program to use as refer.])
MAN_CHECK_PROGS([grap], [GRAP], [use GRAP to preprocess graphs], [grap])
AC_DEFINE_UNQUOTED([PROG_GRAP], ["$grap"], [Program to use as grap.])
MAN_CHECK_PROGS([pic], [PIC], [use PIC to preprocess pictures], [pic gpic])
test -n "$pic" && pic="$pic -S"
AC_DEFINE_UNQUOTED([PROG_PIC], ["$pic"], [Program to use as pic.])

MAN_CHECK_PROGS([gzip], [GZIP], [use GZIP as GNU compression utility], [gzip])
if test -n "$gzip"
then
	gunzip="$gzip -dc"
	compressor="$gzip -c7"
	compress_ext="gz"
fi
MAN_CHECK_PROGS([compress], [COMPRESS], [use COMPRESS as UNIX compression utility], [compress])
if test -n "$compress"
then
	uncompress="$compress -dc"
	if test -z "$gzip"
	then
		compressor="$compress -c"
		compress_ext="Z"
	fi
fi
MAN_CHECK_PROGS([bzip2], [BZIP2], [use BZIP2 as block-sorting compression utility], [bzip2])
if test -n "$bzip2"
then
	bunzip2="$bzip2 -dc"
fi
MAN_CHECK_PROGS([xz], [XZ], [use XZ as Lempel-Ziv-Markov chain-Algorithm compression utility], [xz])
if test -n "$xz"
then
	unxz="$xz -dc"
	unlzma=
else
	dnl lzma not used/needed if we have xz
	MAN_CHECK_PROGS([lzma], [LZMA], [use LZMA as Lempel-Ziv-Markov chain-Algorithm compression utility], [lzma])
	if test -n "$lzma"
	then
		unlzma="$lzma -dc"
	fi
fi
MAN_CHECK_PROGS([lzip], [LZIP], [use LZIP as Lempel-Ziv-Markov chain-Algorithm compression utility], [lzip])
if test -n "$lzip"
then
	unlzip="$lzip -dc"
fi
MAN_CHECK_PROGS([zstd], [ZSTD], [use ZSTD as LZ77/entropy-coding compression utility], [zstd])
if test -n "$zstd"
then
	unzstd="$zstd -dc"
fi
if test -n "$compressor"
then
	AC_DEFINE([COMP_CAT], [1], [Define if you have compressors and want to support compressed cat files.])
fi
dnl Compressors are used for compressing cat pages.
AC_SUBST([compressor])
AC_DEFINE_UNQUOTED(
	[PROG_COMPRESSOR], ["$compressor"],
	[Default compressor, used for compressing cat pages.])
AC_DEFINE_UNQUOTED(
	[COMPRESS_EXT], ["$compress_ext"],
	[Default compression extension, used for compressing cat pages.])
dnl Decompressors are used for decompressing cat pages and nroff source.
dnl To add further decompressors, you will also need to edit
dnl lib/compression.c.
AC_DEFINE_UNQUOTED([PROG_GUNZIP], ["$gunzip"], [Program to use as gunzip.])
AC_DEFINE_UNQUOTED(
	[PROG_UNCOMPRESS], ["$uncompress"], [Program to use as uncompress.])
AC_DEFINE_UNQUOTED([PROG_BUNZIP2], ["$bunzip2"], [Program to use as bunzip2.])
AC_DEFINE_UNQUOTED([PROG_UNLZMA], ["$unlzma"], [Program to use as unlzma.])
AC_DEFINE_UNQUOTED([PROG_UNXZ], ["$unxz"], [Program to use as unxz.])
AC_DEFINE_UNQUOTED([PROG_UNLZIP], ["$unlzip"], [Program to use as unlzip.])
AC_DEFINE_UNQUOTED([PROG_UNZSTD], ["$unzstd"], [Program to use as unzstd.])
MAN_COMPRESS_LIB([z], [gzopen])
dnl To add more decompressors just follow the scheme above.

# Check for various header files and associated libraries.
dnl The "noyywrap" argument is new in Autoconf 2.70, but this also works
dnl fine with older versions that ignore the argument.
AC_PROG_LEX([noyywrap])
if test "$LEX" = ":" && (test ! -e src/lexgrog.c || test ! -e src/zsoelim.c)
then
	AC_MSG_ERROR([flex is required when building from revision control])
fi
gl_INIT
AC_CHECK_HEADERS([sys/file.h linux/fiemap.h])
AC_CHECK_FUNCS([posix_fadvise])

# Internationalization support.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.3])
AC_SUBST([LINGUAS])
AM_ICONV
AC_SUBST([HAVE_ICONV], [$am_func_iconv])
MAN_PO4A
MAN_LINGUAS

# Checks for structures and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_UID_T
AC_TYPE_SIZE_T

# Check for pipeline library.
PKG_CHECK_MODULES([libpipeline], [libpipeline >= 1.5.0])

# Find a suitable database interface header and library.
#
# Check for GNU dbm routines.
if test "$db" = "no" || test "$db" = "gdbm"
then
  AC_CHECK_HEADER([gdbm.h], [
    for lib in gdbm c dbm
    do
      AC_CHECK_LIB([$lib], [gdbm_fetch],
		   test "$lib" = "c" || DBLIBS="-l$lib"
		   [AC_DEFINE([GDBM], [1], [Define if you have, and want to use, gdbm interface routines.])
		    AC_SUBST([DBTYPE], [gdbm])]
		   db=yes, db=no)
      if test "$db" = "yes"
      then
        man_save_LIBS="$LIBS"
        LIBS="$LIBS $DBLIBS"
        AC_CHECK_FUNCS([gdbm_exists])
        LIBS="$man_save_LIBS"
        break
      fi
    done], db=no)
fi

# Check for Berkeley db routines (first version API).
MAN_CHECK_BDB([db5 db], [db5/db_185.h db_185.h], [db5 db-5])
MAN_CHECK_BDB([db4 db], [db4/db_185.h db_185.h], [db4 db-4])
MAN_CHECK_BDB([db3 db], [db3/db_185.h db_185.h], [db3])
MAN_CHECK_BDB([db2 db], [db_185.h db2/db_185.h db2_185.h], [db2 db])
MAN_CHECK_BDB([db1 db], [db/db.h db.h db1/db.h], [db db1 c])

dnl MAN_CHECK_BDB([db2], [db2_185.h], [db2 db c], [AC_DEFINE(DB_ON_LIBC)])
dnl MAN_CHECK_BDB([db2], [db2/db_185.h], [db2 db c])

# Check for UNIX ndbm routines.
if test "$db" = "no" || test "$db" = "ndbm"
then
  AC_CHECK_HEADER([ndbm.h], [
    for lib in ndbm c dbm
    do
      AC_CHECK_LIB([$lib], [dbm_fetch],
		   test "$lib" = "c" || DBLIBS="-l$lib"
		   [AC_DEFINE([NDBM], [1], [Define if you have, and want to use, ndbm interface routines.])
		    AC_SUBST([DBTYPE], [ndbm])]
		   db=yes, db=no)
      test "$db" = "yes" && break
    done], db=no)
  if test "$db" = no
  then
    AC_CHECK_HEADER([gdbm-ndbm.h], [
      for lib in gdbm_compat c dbm
      do
	AC_CHECK_LIB([$lib], [dbm_fetch],
		     test "$lib" = "c" || DBLIBS="-l$lib"
		     [AC_DEFINE([NDBM], [1], [Define if you have, and want to use, ndbm interface routines.])
		      AC_SUBST([DBTYPE], [ndbm])]
		     db=yes, db=no)
	test "$db" = "yes" && break
      done], db=no)
  fi
fi

if test "$db" != "yes"
then
    if test "$db" = "no"
    then
      AC_MSG_ERROR([Fatal: no supported database library/header found])
    else
      AC_MSG_ERROR([Fatal: $db: unsupported database library])
    fi
fi
AC_SUBST([DBLIBS])

# Check for libseccomp library.
MAN_LIBSECCOMP

dnl MAN_ECHO_VAR(ENV-VARIABLE)
define([MAN_ECHO_VAR], [AC_MSG_NOTICE([default $1 = "$$1"])])dnl
dnl
MAN_ECHO_VAR([CC])
MAN_ECHO_VAR([CPP])
MAN_ECHO_VAR([CPPFLAGS])
MAN_ECHO_VAR([CFLAGS])
MAN_ECHO_VAR([LDFLAGS])
MAN_ECHO_VAR([LIBS])
MAN_ECHO_VAR([DBLIBS])

# Transformed versions of program names for use in Automake variables.
MAN_TRANS_SUBST([apropos])
MAN_TRANS_SUBST([catman])
MAN_TRANS_SUBST([lexgrog])
MAN_TRANS_SUBST([man])
MAN_TRANS_SUBST([man-recode])
MAN_TRANS_SUBST([manconv])
MAN_TRANS_SUBST([mandb])
MAN_TRANS_SUBST([manpath])
MAN_TRANS_SUBST([whatis])
MAN_TRANS_SUBST([zsoelim])

# If we're cross-compiling, tests won't work.
AM_CONDITIONAL([CROSS_COMPILING], [test "x$cross_compiling" = xyes])

# Are Gnulib translations available?
AM_CONDITIONAL([HAVE_GNULIB_PO], [test -f "$srcdir/gl/po/POTFILES.in"])

AC_CONFIG_FILES([Makefile
	gl/lib/Makefile
	init/Makefile
	init/systemd/Makefile
	lib/Makefile
	src/Makefile
	src/man_db.conf
	src/tests/Makefile
	man/Makefile
	man/replace.sin
	man/po4a/Makefile
	man/da/Makefile
	man/de/Makefile
	man/es/Makefile
	man/fr/Makefile
	man/id/Makefile
	man/it/Makefile
	man/ja/Makefile
	man/ko/Makefile
	man/nl/Makefile
	man/pl/Makefile
	man/pt/Makefile
	man/pt_BR/Makefile
	man/ro/Makefile
	man/ru/Makefile
	man/sr/Makefile
	man/sv/Makefile
	man/tr/Makefile
	man/zh_CN/Makefile
	manual/Makefile
	libdb/Makefile
	docs/Makefile
	tools/Makefile
	po/Makefile.in])
if test -f "$srcdir/gl/po/Makefile.in.in"; then
	AC_CONFIG_FILES([gl/po/Makefile.in])
fi
AC_OUTPUT
