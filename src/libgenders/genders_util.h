/*****************************************************************************\
 *  $Id: genders_util.h,v 1.7 2010-02-02 00:04:34 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2001-2007 The Regents of the University of California.
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
 *  with Genders.  If not, see <http://www.gnu.org/licenses/>.
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

/* 
 * _genders_list_is_all
 *
 * Returns 1
 */
int _genders_list_is_all(void *x, void *key); 

/* 
 * _genders_list_is_str
 *
 * x points to a string
 *  
 * Returns 1 if string x is equal to key, 0 otherwise
 */
int _genders_list_is_str(void *x, void *key); 

/* 
 * _genders_list_is_attr_in_attrvals
 *
 * x points to a genders_attrval_t
 *
 * Returns 1 if attr in 'x' is equal to key, 0 otherwise
 */
int _genders_list_is_attr_in_attrvals(void *x, void *key);

/* 
 * _genders_list_free_genders_node
 *
 * Free genders_node_t structure
 */
void _genders_list_free_genders_node(void *x); 

/* 
 * _genders_list_free_genders_attrval
 *
 * Free genders_attrval_t structure
 */
void _genders_list_free_genders_attrval(void *x); 

/* 
 * _genders_list_free_genders_attrvalllist
 *
 * Free list of genders_attrval_t structures
 */
void _genders_list_free_attrvallist(void *x);

/* 
 * Common helper functions 
 */

/*  
 * _genders_handle_error_check
 *
 * Check if handle is proper
 *
 * Returns 0 on success, -1 on error
 */
int _genders_handle_error_check(genders_t handle);

/* 
 * _genders_unloaded_handle_error_check
 *
 * Check if handle is proper and unloaded
 *
 * Returns 0 on success, -1 on error
 */
int _genders_unloaded_handle_error_check(genders_t handle);

/* 
 * _genders_loaded_handle_error_check
 *
 * Check if handle is proper and loaded
 *
 * Returns 0 on success, -1 on error
 */
int _genders_loaded_handle_error_check(genders_t handle);

/* 
 * _genders_put_in_array
 *
 * Put str in list, doing appropriate checks
 *
 * Return 0 on success, -1 on error
 */
int _genders_put_in_array(genders_t handle, 
			  char *str, 
			  char **list, 
			  int index, 
			  int len);

/* 
 * _genders_get_valptr
 *
 * Return av->val or handle->valbuf depending on if substitution if required.
 *
 * Return 0 on success, -1 on error
 */
int _genders_get_valptr(genders_t handle, 
			genders_node_t n, 
			genders_attrval_t av, 
			char **val,
			int *subst_occurred);

/* 
 * _genders_find_attrval
 *
 * Find genders_attrval_t with attr or attr=val in a node
 *
 * Return 0 on success, -1 on error
 */
int _genders_find_attrval(genders_t handle, 
			  genders_node_t n, 
			  const char *attr, 
			  const char *val,
			  genders_attrval_t *avptr);

/* 
 * _genders_rehash
 *
 * Rehash the specified hash into a larger hash.  Both hash_ptr and
 * size are in/out parameters.
 *
 * Returns 0 on success, -1 on error
 */
int _genders_rehash(genders_t handle,
                    hash_t *hash_ptr,
                    int *hash_size);

/*
 * _genders_hash_copy
 *
 * Copy contents from src hash to dest hash.
 *
 * Returns 0 on success, -1 on error.
 */
int _genders_hash_copy(genders_t handle,
		       hash_t *hash_src,
		       hash_t *hash_dest);

#endif /* _GENDERS_COMMON_H */
