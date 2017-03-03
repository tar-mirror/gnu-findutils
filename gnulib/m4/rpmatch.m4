# rpmatch.m4 serial 1
dnl Copyright (C) 2002 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_FUNC_RPMATCH],
[
  AC_REPLACE_FUNCS(rpmatch)
  if test $ac_cv_func_rpmatch = no; then
    gl_PREREQ_RPMATCH
  fi
])

# Prerequisites of lib/rpmatch.c.
AC_DEFUN([gl_PREREQ_RPMATCH], [
  AC_REQUIRE([AC_HEADER_STDC])
  AC_CHECK_HEADERS_ONCE(limits.h)
])

