# strtoimax.m4 serial 2
dnl Copyright (C) 2002-2003 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_FUNC_STRTOIMAX],
[
  dnl Work around a bug of AC_EGREP_CPP in autoconf-2.57.
  AC_REQUIRE([AC_PROG_CPP])
  AC_REQUIRE([AC_PROG_EGREP])

  AC_CACHE_CHECK([whether <inttypes.h> defines strtoimax as a macro],
    jm_cv_func_strtoimax_macro,
    [AC_EGREP_CPP([inttypes_h_defines_strtoimax], [#include <inttypes.h>
#ifdef strtoimax
 inttypes_h_defines_strtoimax
#endif],
       jm_cv_func_strtoimax_macro=yes,
       jm_cv_func_strtoimax_macro=no)])

  if test "$jm_cv_func_strtoimax_macro" != yes; then
    AC_REPLACE_FUNCS(strtoimax)
    if test $ac_cv_func_strtoimax = no; then
      gl_PREREQ_STRTOIMAX
    fi
  fi
])

# Prerequisites of lib/strtoimax.c.
AC_DEFUN([gl_PREREQ_STRTOIMAX], [
  jm_AC_TYPE_INTMAX_T
  AC_CHECK_HEADERS_ONCE(stdlib.h)
  AC_CHECK_DECLS_ONCE(strtol)
  AC_CHECK_DECLS(strtoll)
  AC_REQUIRE([jm_AC_TYPE_LONG_LONG])
])
