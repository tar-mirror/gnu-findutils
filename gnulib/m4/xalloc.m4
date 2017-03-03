# xalloc.m4 serial 11
dnl Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_XALLOC],
[
  gl_PREREQ_XALLOC
  gl_PREREQ_XMALLOC
])

# Prerequisites of lib/xalloc.h.
AC_DEFUN([gl_PREREQ_XALLOC], [
  :
])

# Prerequisites of lib/xmalloc.c.
AC_DEFUN([gl_PREREQ_XMALLOC], [
  AC_REQUIRE([AC_C_INLINE])
  :
])
