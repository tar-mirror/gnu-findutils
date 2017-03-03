# xstrtol.m4 serial 1
dnl Copyright (C) 2002 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_XSTRTOL],
[
  AC_REQUIRE([gl_PREREQ_XSTRTOL])
  AC_REQUIRE([gl_PREREQ_XSTRTOUL])
])

# Prerequisites of lib/xstrtol.h.
AC_DEFUN([gl_PREREQ_XSTRTOL_H],
[
  AC_REQUIRE([jm_AC_TYPE_INTMAX_T])
  AC_REQUIRE([jm_AC_TYPE_UINTMAX_T])
])

# Prerequisites of lib/xstrtol.c.
AC_DEFUN([gl_PREREQ_XSTRTOL],
[
  AC_REQUIRE([gl_PREREQ_XSTRTOL_H])
  AC_REQUIRE([AC_HEADER_STDC])
  AC_CHECK_HEADERS_ONCE(limits.h string.h)
  AC_CHECK_FUNCS_ONCE(isascii)
  AC_CHECK_DECLS_ONCE(strtol strtoul)
  AC_CHECK_DECLS([strtoimax, strtoumax])
])

# Prerequisites of lib/xstrtoul.c.
AC_DEFUN([gl_PREREQ_XSTRTOUL],
[
  AC_REQUIRE([gl_PREREQ_XSTRTOL])
])
