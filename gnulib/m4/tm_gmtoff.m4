# tm_gmtoff.m4 serial 1
dnl Copyright (C) 2002 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_TM_GMTOFF],
[
 AC_CHECK_MEMBER([struct tm.tm_gmtoff],
                 [AC_DEFINE(HAVE_TM_GMTOFF, 1,
                            [Define if struct tm has the tm_gmtoff member.])],
                 ,
                 [#include <time.h>])
])