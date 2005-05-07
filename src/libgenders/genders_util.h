/*****************************************************************************\
 *  $Id: genders_util.h,v 1.1 2005-05-07 15:30:42 achu Exp $
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

#ifndef _GENDERS_COMMON_H
#define _GENDERS_COMMON_H 1

#include "list.h"
#include "hash.h"
#include "hostlist.h"

/* Helper Macros */

#define __list_create(dest, delF) \
        do { \
          if (!((dest) = list_create((delF)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
        } while (0)

#define __list_append(list, item) \
        do { \
          if (!list_append((list), (item))) { \
            handle->errnum = GENDERS_ERR_INTERNAL; \
            goto cleanup; \
          } \
        } while (0)

#define __list_destroy(__list) \
        if ((__list)) list_destroy((__list))

#define __list_iterator_create(dest, list) \
        do { \
          if (!((dest) = list_iterator_create((list)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
        } while (0)

#define __list_iterator_destroy(__itr) \
        if ((__itr)) list_iterator_destroy((__itr))


#define __hash_create(dest, size, key_f, cmp_f, del_f) \
        do { \
          if (!((dest) = hash_create((size),(key_f),(cmp_f), (del_f)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
        } while (0)

#define __hash_insert(hash, key, data) \
        do { \
          if (!hash_insert((hash), (key), (data))) { \
            if (errno == ENOMEM) \
              handle->errnum = GENDERS_ERR_OUTMEM; \
            else \
              handle->errnum = GENDERS_ERR_INTERNAL; \
            goto cleanup; \
          } \
        } while (0)

#define __hash_destroy(__hash) \
        if ((__hash)) hash_destroy((__hash))

#define __hostlist_create(dest, str) \
        do { \
          if (!((dest) = hostlist_create((str)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
        } while (0)

#define __hostlist_destroy(__hostlist) \
        if ((__hostlist)) hostlist_destroy((__hostlist))

#define __hostlist_iterator_create(dest, hl) \
        do { \
          if (!((dest) = hostlist_iterator_create((hl)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
        } while (0)

#define __hostlist_iterator_destroy(__hostlist) \
        if ((__hostlist)) hostlist_iterator_destroy((__hostlist))

#define __xmalloc(dest, cast, size) \
        do { \
          if (!((dest) = (cast)malloc((size)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
          memset((dest), '\0', (size)); \
        } while (0)

#define __xstrdup(dest, src) \
        do { \
          if (!((dest) = strdup((src)))) { \
            handle->errnum = GENDERS_ERR_OUTMEM; \
            goto cleanup; \
          } \
        } while (0)

#define GENDERS_MAX(x,y) ((x > y) ? x : y)

/* 
 * List API Helper Functions 
 */

int _is_all(void *x, void *key); 
int _is_str(void *x, void *key); 
int _is_node(void *x, void *key); 
int _is_attr_in_attrvals(void *x, void *key);
void _free_genders_node(void *x); 
void _free_genders_attrval(void *x); 
void _free_attrvallist(void *x);

/* Common functions */

int _handle_error_check(genders_t handle);
int _unloaded_handle_error_check(genders_t handle);
int _loaded_handle_error_check(genders_t handle);
int _put_in_array(genders_t handle, char *str, char **list, int index, int len);

int _get_valptr(genders_t handle, 
                genders_node_t n, 
                genders_attrval_t av, 
                char **val,
                int *subst_occurred);

int _find_attrval(genders_t handle, 
                  genders_node_t n, 
                  const char *attr, 
                  const char *val,
                  genders_attrval_t *avptr);

#endif /* _GENDERS_COMMON_H */
