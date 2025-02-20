/*****************************************************************************\
 *  $Id: genders_util.c,v 1.10 2010-02-02 00:04:34 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2001-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Jim Garlick <garlick@llnl.gov> and Albert Chu <chu11@llnl.gov>.
 *  UCRL-CODE-2003-004.
 *
 *  This file is part of Genders, a cluster configuration database.
 *  For details, see <https://github.com/chaos/genders>.
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
 *  with Genders.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include "genders.h"
#include "genders_api.h"
#include "genders_util.h"
#include "hash.h"
#include "hostlist.h"
#include "list.h"

int
_genders_list_is_all(void *x, void *key)
{
  return 1;
}

int
_genders_list_is_str(void *x, void *key)
{
  if (!strcmp((char *)x, (char *)key))
    return 1;
  return 0;
}

int
_genders_list_is_attr_in_attrvals(void *x, void *key)
{
  genders_attrval_t av;

  av = (genders_attrval_t)x;
  if (!strcmp(av->attr, (char *)key))
    return 1;
  return 0;
}

void
_genders_list_free_genders_hosts(void *x)
{
  genders_hosts_t gh;

  gh = (genders_hosts_t)x;
  __hostlist_destroy(gh->hl);
  __hash_destroy(gh->attrval_index);
  free(gh);
}

void
_genders_list_free_genders_attrval(void *x)
{
  genders_attrval_t av;

  av = (genders_attrval_t)x;
  free(av->attr);
  free(av->val);
  free(av);
}

int
_genders_hash_is_all(void *data, const void *key, void *arg)
{
  return 1;
}

void
_genders_hash_free_genders_attrval(void *x)
{
  genders_attrval_t av;

  av = (genders_attrval_t)x;
  free(av->attr);
  free(av->val);
  free(av);
}

void _genders_hostlist_delete_all(hostlist_t hl)
{
  int count = hostlist_count(hl);
  int i;
  for (i = 0; i < count; i++)
    hostlist_delete_nth(hl, 0);
}

int
_genders_handle_error_check(genders_t handle)
{
  if (!handle || handle->magic != GENDERS_MAGIC_NUM)
    return -1;
  return 0;
}

int
_genders_unloaded_handle_error_check(genders_t handle)
{
  if (_genders_handle_error_check(handle) < 0)
    return -1;

  if (handle->is_loaded)
    {
      handle->errnum = GENDERS_ERR_ISLOADED;
      return -1;
    }

  return 0;
}

int
_genders_loaded_handle_error_check(genders_t handle)
{
  if (_genders_handle_error_check(handle) < 0)
    return -1;

  if (!handle->is_loaded)
    {
      handle->errnum = GENDERS_ERR_NOTLOADED;
      return -1;
    }

  return 0;
}

int
_genders_put_in_array(genders_t handle,
                      char *str,
                      char **list,
                      int index,
                      int len)
{
  if (index >= len)
    {
      handle->errnum = GENDERS_ERR_OVERFLOW;
      return -1;
    }

  if (!list[index])
    {
      handle->errnum = GENDERS_ERR_NULLPTR;
      return -1;
    }

  strcpy(list[index], str);
  return 0;
}

int
_genders_get_valptr(genders_t handle,
                    const char *nodename,
                    genders_attrval_t av,
                    char **val,
                    int *subst_occurred)
{
  char *valptr, *nodenameptr, *valbufptr;

  if (!(av->val_contains_subst)
      || (handle->flags & GENDERS_FLAG_RAW_VALUES))
    {
      if (subst_occurred)
        *subst_occurred = 0;
      *val = av->val;
      return 0;
    }

  valbufptr = handle->valbuf;
  valptr = av->val;
  memset(valbufptr, '\0', handle->maxvallen + 1);
  while (*valptr != '\0')
    {
      if (*valptr == '%')
        {
          if ((*(valptr + 1)) == '%')
            {
              *(valbufptr)++ = '%';
              valptr++;
            }
          else if ((*(valptr + 1)) == 'n')
            {
              if ((strlen(av->val) - 2 + strlen(nodename)) >
                  (handle->maxvallen + 1))
                {
                  handle->errnum = GENDERS_ERR_INTERNAL;
                  return -1;
                }

              nodenameptr = nodename;
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

static int
_hash_reinsert(void *data, const void *key, void *arg)
{
  hash_t newhash;

  newhash = *((hash_t *)arg);

  /* return -1 will result in GENDERS_ERR_INTERNAL in caller */
  if (hash_insert(newhash, key, data) < 0)
    return -1;

  return 1;
}

static int
_hash_removeall(void *data, const void *key, void *arg)
{
  return 1;
}

int
_genders_rehash(genders_t handle,
                hash_t *hash_ptr,
                int *hash_size)
{
  hash_t new_hash = NULL;
  int hash_num;
  int retval = -1;

  (*hash_size) *= 2;
  __hash_create(new_hash,
                (*hash_size),
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                NULL);

  hash_num = hash_count(*hash_ptr);

  if (hash_for_each(*hash_ptr, _hash_reinsert, &new_hash) != hash_num)
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  if (hash_remove_if(*hash_ptr, _hash_removeall, NULL) != hash_num)
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  __hash_destroy(*hash_ptr);
  *hash_ptr = new_hash;
  retval = 0;
 cleanup:
  if (retval < 0)
    __hash_destroy(new_hash);
  return retval;
}
