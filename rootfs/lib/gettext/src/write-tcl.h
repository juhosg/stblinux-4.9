/* Writing tcl/msgcat .msg files.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2002.

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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _WRITE_TCL_H
#define _WRITE_TCL_H

#include "message.h"

/* Write a Tcl msg file.  mlp is a list containing the messages to be output.
   locale_name is the locale name (with underscore separators), directory is
   the base directory.
   Return 0 if ok, nonzero on error.  */
extern int
       msgdomain_write_tcl PARAMS ((message_list_ty *mlp,
				    const char *locale_name,
				    const char *directory));

#endif /* _WRITE_TCL_H */