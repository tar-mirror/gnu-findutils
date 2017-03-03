## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = 1.5 gnits no-dependencies

noinst_LIBRARIES = libgnulib.a

libgnulib_a_SOURCES =
libgnulib_a_LIBADD = @LIBOBJS@
#libgnulib_la_LIBADD = @LTLIBOBJS@
EXTRA_DIST =
BUILT_SOURCES =
SUFFIXES =
MOSTLYCLEANFILES =
CLEANFILES =
DISTCLEANFILES =
MAINTAINERCLEANFILES =
EXTRA_DIST += alloca_.h

# The following is needed in order to create an <alloca.h> when the system
# doesn't have one that works with the given compiler.
all-local $(libgnulib_a_OBJECTS): @ALLOCA_H@
alloca.h: alloca_.h
	cp $(srcdir)/alloca_.h alloca.h
MOSTLYCLEANFILES += alloca.h

libgnulib_a_LIBADD += @ALLOCA@
#libgnulib_la_LIBADD += @LTALLOCA@
libgnulib_a_SOURCES += argmatch.h argmatch.c

libgnulib_a_SOURCES += dirname.h dirname.c basename.c stripslash.c



libgnulib_a_SOURCES += filemode.h filemode.c

EXTRA_DIST += fnmatch_.h fnmatch_loop.c

# The following is needed in order to create an <fnmatch.h> when the system
# doesn't have one that supports the required API.
all-local $(libgnulib_a_OBJECTS): fnmatch.h
fnmatch.h: fnmatch_.h
	cp $(srcdir)/fnmatch_.h fnmatch.h
MOSTLYCLEANFILES += fnmatch.h

libgnulib_a_SOURCES += getline.h

libgnulib_a_SOURCES += getopt.h getopt.c getopt1.c

libgnulib_a_SOURCES += gettext.h

libgnulib_a_SOURCES += human.h human.c

libgnulib_a_SOURCES += idcache.c






libgnulib_a_SOURCES += modechange.h modechange.c

libgnulib_a_SOURCES += pathmax.h

libgnulib_a_SOURCES += quote.h quote.c

libgnulib_a_SOURCES += quotearg.h quotearg.c


libgnulib_a_SOURCES += regex.h


libgnulib_a_SOURCES += savedir.h savedir.c


libgnulib_a_SOURCES += stpcpy.h


libgnulib_a_SOURCES += strftime.c








libgnulib_a_SOURCES += unlocked-io.h

libgnulib_a_SOURCES += xalloc.h xmalloc.c xstrdup.c

libgnulib_a_SOURCES += xgetcwd.h xgetcwd.c

libgnulib_a_SOURCES += xstrtol.h xstrtol.c xstrtoul.c

libgnulib_a_SOURCES += xstrtoumax.c

libgnulib_a_SOURCES += yesno.c

