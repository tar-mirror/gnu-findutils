# human.m4 serial 1
dnl Copyright (C) 2002 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_HUMAN],
[
  dnl Prerequisites of lib/human.h.
  AC_REQUIRE([jm_AC_TYPE_UINTMAX_T])

  dnl Prerequisites of lib/human.c.
  AC_CHECK_HEADERS_ONCE(limits.h stdlib.h string.h)
  AC_CHECK_DECLS_ONCE([getenv])
])
