/* modechange.c -- file mode manipulation

   Copyright (C) 1989, 1990, 1997, 1998, 1999, 2001, 2003, 2004, 2005
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by David MacKenzie <djm@ai.mit.edu> */

/* The ASCII mode string is compiled into an array of `struct
   modechange', which can then be applied to each file to be changed.
   We do this instead of re-parsing the ASCII string for each file
   because the compiled form requires less computation to use; when
   changing the mode of many files, this probably results in a
   performance gain.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "modechange.h"
#include <sys/stat.h>
#include "stat-macros.h"
#include "xalloc.h"
#include <stdlib.h>

/* The traditional octal values corresponding to each mode bit.  */
#define SUID 04000
#define SGID 02000
#define SVTX 01000
#define RUSR 00400
#define WUSR 00200
#define XUSR 00100
#define RGRP 00040
#define WGRP 00020
#define XGRP 00010
#define ROTH 00004
#define WOTH 00002
#define XOTH 00001
#define ALLM 07777 /* all octal mode bits */

/* Special operations flags.  */
enum
  {
    /* For the sentinel at the end of the mode changes array.  */
    MODE_DONE,

    /* The typical case.  */
    MODE_ORDINARY_CHANGE,

    /* In addition to the typical case, affect the execute bits if at
       least one execute bit is set already, or if the file is a
       directory.  */
    MODE_X_IF_ANY_X,

    /* Instead of the typical case, copy some existing permissions for
       u, g, or o onto the other two.  Which of u, g, or o is copied
       is determined by which bits are set in the `value' field.  */
    MODE_COPY_EXISTING
  };

/* Description of a mode change.  */
struct mode_change
{
  char op;			/* One of "=+-".  */
  char flag;			/* Special operations flag.  */
  mode_t affected;		/* Set for u, g, o, or a.  */
  mode_t value;			/* Bits to add/remove.  */
};

/* Return a mode_change array with the specified `=ddd'-style
   mode change operation, where NEW_MODE is `ddd'.  */

static struct mode_change *
make_node_op_equals (mode_t new_mode)
{
  struct mode_change *p = xmalloc (2 * sizeof *p);
  p->op = '=';
  p->flag = MODE_ORDINARY_CHANGE;
  p->affected = CHMOD_MODE_BITS;
  p->value = new_mode;
  p[1].flag = MODE_DONE;
  return p;
}

/* Return a pointer to an array of file mode change operations created from
   MODE_STRING, an ASCII string that contains either an octal number
   specifying an absolute mode, or symbolic mode change operations with
   the form:
   [ugoa...][[+-=][rwxXstugo...]...][,...]

   Return NULL if `mode_string' does not contain a valid
   representation of file mode change operations.  */

struct mode_change *
mode_compile (char const *mode_string)
{
  /* The array of mode-change directives to be returned.  */
  struct mode_change *mc;
  size_t used = 0;

  if ('0' <= *mode_string && *mode_string < '8')
    {
      mode_t mode;
      unsigned int octal_value = 0;

      do
	{
	  octal_value = 8 * octal_value + *mode_string++ - '0';
	  if (ALLM < octal_value)
	    return NULL;
	}
      while ('0' <= *mode_string && *mode_string < '8');

      /* Help the compiler optimize the usual case where mode_t uses
	 the traditional octal representation.  */
      mode = ((S_ISUID == SUID && S_ISGID == SGID && S_ISVTX == SVTX
	       && S_IRUSR == RUSR && S_IWUSR == WUSR && S_IXUSR == XUSR
	       && S_IRGRP == RGRP && S_IWGRP == WGRP && S_IXGRP == XGRP
	       && S_IROTH == ROTH && S_IWOTH == WOTH && S_IXOTH == XOTH)
	      ? octal_value
	      : (mode_t) ((octal_value & SUID ? S_ISUID : 0)
			  | (octal_value & SGID ? S_ISGID : 0)
			  | (octal_value & SVTX ? S_ISVTX : 0)
			  | (octal_value & RUSR ? S_IRUSR : 0)
			  | (octal_value & WUSR ? S_IWUSR : 0)
			  | (octal_value & XUSR ? S_IXUSR : 0)
			  | (octal_value & RGRP ? S_IRGRP : 0)
			  | (octal_value & WGRP ? S_IWGRP : 0)
			  | (octal_value & XGRP ? S_IXGRP : 0)
			  | (octal_value & ROTH ? S_IROTH : 0)
			  | (octal_value & WOTH ? S_IWOTH : 0)
			  | (octal_value & XOTH ? S_IXOTH : 0)));

      return make_node_op_equals (mode);
    }

  /* Allocate enough space to hold the result.  */
  {
    size_t needed = 1;
    char const *p;
    for (p = mode_string; *p; p++)
      needed += (*p == '=' || *p == '+' || *p == '-');
    mc = xnmalloc (needed, sizeof *mc);
  }

  /* One loop iteration for each `[ugoa]*([-+=]([rwxXst]*|[ugo]))+'.  */
  for (;; mode_string++)
    {
      /* Which bits in the mode are operated on.  */
      mode_t affected = 0;

      /* Turn on all the bits in `affected' for each group given.  */
      for (;; mode_string++)
	switch (*mode_string)
	  {
	  default:
	    goto invalid;
	  case 'u':
	    affected |= S_ISUID | S_IRWXU;
	    break;
	  case 'g':
	    affected |= S_ISGID | S_IRWXG;
	    break;
	  case 'o':
	    affected |= S_ISVTX | S_IRWXO;
	    break;
	  case 'a':
	    affected |= CHMOD_MODE_BITS;
	    break;
	  case '=': case '+': case '-':
	    goto no_more_affected;
	  }
    no_more_affected:;

      do
	{
	  char op = *mode_string++;
	  mode_t value;
	  char flag = MODE_COPY_EXISTING;
	  struct mode_change *change;

	  switch (*mode_string++)
	    {
	    case 'u':
	      /* Set the affected bits to the value of the `u' bits
		 on the same file.  */
	      value = S_IRWXU;
	      break;
	    case 'g':
	      /* Set the affected bits to the value of the `g' bits
		 on the same file.  */
	      value = S_IRWXG;
	      break;
	    case 'o':
	      /* Set the affected bits to the value of the `o' bits
		 on the same file.  */
	      value = S_IRWXO;
	      break;

	    default:
	      value = 0;
	      flag = MODE_ORDINARY_CHANGE;

	      for (mode_string--;; mode_string++)
		switch (*mode_string)
		  {
		  case 'r':
		    value |= S_IRUSR | S_IRGRP | S_IROTH;
		    break;
		  case 'w':
		    value |= S_IWUSR | S_IWGRP | S_IWOTH;
		    break;
		  case 'x':
		    value |= S_IXUSR | S_IXGRP | S_IXOTH;
		    break;
		  case 'X':
		    flag = MODE_X_IF_ANY_X;
		    break;
		  case 's':
		    /* Set the setuid/gid bits if `u' or `g' is selected.  */
		    value |= S_ISUID | S_ISGID;
		    break;
		  case 't':
		    /* Set the "save text image" bit if `o' is selected.  */
		    value |= S_ISVTX;
		    break;
		  default:
		    goto no_more_values;
		  }
	    no_more_values:;
	    }

	  change = &mc[used++];
	  change->op = op;
	  change->flag = flag;
	  change->affected = affected;
	  change->value = value;
	}
      while (*mode_string == '=' || *mode_string == '+'
	     || *mode_string == '-');

      if (*mode_string != ',')
	break;
    }

  if (*mode_string == 0)
    {
      mc[used].flag = MODE_DONE;
      return mc;
    }

invalid:
  free (mc);
  return NULL;
}

/* Return a file mode change operation that sets permissions to match those
   of REF_FILE.  Return NULL (setting errno) if REF_FILE can't be accessed.  */

struct mode_change *
mode_create_from_ref (const char *ref_file)
{
  struct stat ref_stats;

  if (stat (ref_file, &ref_stats) != 0)
    return NULL;
  return make_node_op_equals (ref_stats.st_mode);
}

/* Return file mode OLDMODE, adjusted as indicated by the list of change
   operations CHANGES, which are interpreted assuming the umask is
   UMASK_VALUE.  If OLDMODE is a directory, the type `X'
   change affects it even if no execute bits were set in OLDMODE.
   The returned value has the S_IFMT bits cleared.  */

mode_t
mode_adjust (mode_t oldmode, struct mode_change const *changes,
	     mode_t umask_value)
{
  /* The adjusted mode.  */
  mode_t newmode = oldmode & CHMOD_MODE_BITS;

  for (; changes->flag != MODE_DONE; changes++)
    {
      mode_t affected = changes->affected;
      mode_t value = changes->value;

      switch (changes->flag)
	{
	case MODE_ORDINARY_CHANGE:
	  break;

	case MODE_COPY_EXISTING:
	  /* Isolate in `value' the bits in `newmode' to copy.  */
	  value &= newmode;

	  /* Copy the isolated bits to the other two parts.  */
	  value |= ((value & (S_IRUSR | S_IRGRP | S_IROTH)
		     ? S_IRUSR | S_IRGRP | S_IROTH : 0)
		    | (value & (S_IWUSR | S_IWGRP | S_IWOTH)
		       ? S_IWUSR | S_IWGRP | S_IWOTH : 0)
		    | (value & (S_IXUSR | S_IXGRP | S_IXOTH)
		       ? S_IXUSR | S_IXGRP | S_IXOTH : 0));
	  break;

	case MODE_X_IF_ANY_X:
	  /* Affect the execute bits if execute bits are already set
	     or if the file is a directory.  */
	  if ((newmode & (S_IXUSR | S_IXGRP | S_IXOTH)) || S_ISDIR (oldmode))
	    value |= S_IXUSR | S_IXGRP | S_IXOTH;
	  break;
	}

      /* If WHO was specified, limit the change to the affected bits.
	 Otherwise, apply the umask.  */
      value &= (affected ? affected : ~umask_value);

      switch (changes->op)
	{
	case '=':
	  /* If WHO was specified, preserve the previous values of
	     bits that are not affected by this change operation.
	     Otherwise, clear all the bits.  */
	  newmode = (affected ? newmode & ~affected : 0);
	  /* Fall through.  */
	case '+':
	  newmode |= value;
	  break;

	case '-':
	  newmode &= ~value;
	  break;
	}
    }

  return newmode;
}
