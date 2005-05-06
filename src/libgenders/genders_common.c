/*****************************************************************************\
 *  $Id: genders_common.c,v 1.6 2005-05-06 23:09:32 achu Exp $
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

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <assert.h>

#include "genders.h"
#include "genders_common.h"
#include "list.h"
#include "hash.h"
#include "hostlist.h"

int 
_is_all(void *x, void *key) 
{
  assert(x && key);
  return 1;
}

int 
_is_str(void *x, void *key) 
{
  assert(x && key);
  if (!strcmp((char *)x, (char *)key))
    return 1;
  return 0;
}

int 
_is_node(void *x, void *key) 
{
  genders_node_t n;

  assert(x && key);

  n = (genders_node_t)x;
  if (!strcmp(n->name, (char *)key))
    return 1;
  return 0;
}

int
_is_attr_in_attrvals(void *x, void *key)
{
  genders_attrval_t av;

  assert(x && key);

  av = (genders_attrval_t)x;
  if (!strcmp(av->attr, (char *)key))
    return 1;
  return 0;
}

void 
_free_genders_node(void *x) 
{
  genders_node_t n;

  assert(x);

  n = (genders_node_t)x;
  __list_destroy(n->attrlist);
  free(n->name);
  free(n);
}

void 
_free_genders_attrval(void *x) 
{
  genders_attrval_t av;

  assert(x);

  av = (genders_attrval_t)x;
  free(av->attr);
  free(av->val);
  free(av);
}

void
_free_attrvallist(void *x)
{
  List attrvals;

  assert(x);

  attrvals = (List)x;
  __list_destroy(attrvals);
}

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
_put_in_array(genders_t handle, char *str, char **list, int index, int len)
{
  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(str);
  assert(list || (!list && !len));

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

/* _get_valptr
 * - Return av->val or handle->valbuf, depending on if substitution is
 * required.
 */
int
_get_valptr(genders_t handle, 
            genders_node_t n, 
            genders_attrval_t av, 
            char **val,
            int *subst_occurred)
{
  char *valptr, *nodenameptr, *valbufptr;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(n && av && val);
  assert(!(!av->val && av->val_contains_subst));

  if (!(av->val_contains_subst)) {
    if (subst_occurred)
      *subst_occurred = 0;
    *val = av->val;
    return 0;
  }

  valbufptr = handle->valbuf;
  valptr = av->val;
  memset(valbufptr, '\0', handle->maxvallen + 1);
  while (*valptr != '\0') {
    if (*valptr == '%') {
      if ((*(valptr + 1)) == '%') {
        *(valbufptr)++ = '%';
        valptr++;
      }
      else if ((*(valptr + 1)) == 'n') {
        if ((strlen(av->val) - 2 + strlen(n->name)) > 
            (handle->maxvallen + 1)) {
          handle->errnum = GENDERS_ERR_INTERNAL;
          return -1;
        }

        nodenameptr = n->name;
        while (*nodenameptr != '\0')
          *(valbufptr)++ = *nodenameptr++;
        valptr++;
      }
      else
        *(valbufptr)++ = *valptr;
    }
    else 
      *(valbufptr)++ = *valptr;

    valptr++;
  }

  if (subst_occurred)
    *subst_occurred = 1;
  *val = handle->valbuf;
  return 0;
}

/* _find_attrval
 * - Find/Return genders_attrval_t with attr or attr=val in a node
 */
int
_find_attrval(genders_t handle, 
              genders_node_t n, 
              const char *attr, 
              const char *val,
              genders_attrval_t *avptr)
{
  ListIterator itr = NULL;
  List attrvals;
  int retval = -1;
  
  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(n && attr && avptr);

  *avptr = NULL;
  __list_iterator_create(itr, n->attrlist);
  while ((attrvals = list_next(itr))) {
    genders_attrval_t av;
    
    if ((av = list_find_first(attrvals, _is_attr_in_attrvals, (char *)attr))) {
      if (!val) {
        *avptr = av;
        break;
      }
      else if (av->val) {
        char *valptr;

        if (_get_valptr(handle, n, av, &valptr, NULL) < 0)
          goto cleanup;
        if (!strcmp(valptr, val)) {
          *avptr = av;
          break;
        }
      }
    }
  }

  retval = 0;
 cleanup:
  __list_iterator_destroy(itr);
  return retval;  
}
