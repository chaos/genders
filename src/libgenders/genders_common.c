/*****************************************************************************\
 *  $Id: genders_common.c,v 1.1 2004-06-07 20:14:37 achu Exp $
 *****************************************************************************
 *  Copyright (C) 2001-2003 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Jim Garlick <garlick@llnl.gov> and Albert Chu <chu11@llnl.gov>.
 *  UCRL-CODE-2003-004.
 *
 *  This file is part of Genders, a cluster configuration database.
 *  For details, see <http://www.llnl.gov/linux/genders/>.
 *
 *  Genders is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  Genders is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Genders; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
\*****************************************************************************/

#include "genders.h"
#include "genders_common.h"
#include "list.h"
#include "hash.h"
#include "hostlist.h"

int 
_handle_error_check(genders_t handle) 
{
  if (!handle || handle->magic != GENDERS_MAGIC_NUM)
    return -1;
  return 0;
}

int 
_unloaded_handle_error_check(genders_t handle) 
{
  if (_handle_error_check(handle) < 0)
    return -1;

  if (handle->is_loaded) {
    handle->errnum = GENDERS_ERR_ISLOADED;
    return -1;
  }
  
  return 0;
}

int 
_loaded_handle_error_check(genders_t handle) 
{
  if (_handle_error_check(handle) < 0)
    return -1;

  if (!handle->is_loaded) {
    handle->errnum = GENDERS_ERR_NOTLOADED;
    return -1;
  }
  
  return 0;
}

int
_put_in_list(genders_t handle, char *str, char **list, int index, int len)
{
  if (index >= len) {
    handle->errnum = GENDERS_ERR_OVERFLOW;
    return -1;
  }
                                                                                      
  if (!list[index]) {
    handle->errnum = GENDERS_ERR_NULLPTR;
    return -1;
  }
                                                                                      
  strcpy(list[index], str);
  return 0;
}
