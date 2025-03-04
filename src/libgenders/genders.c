/*****************************************************************************\
 *  $Id: genders.c,v 1.146 2010-02-02 00:04:34 chu11 Exp $
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
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <errno.h>

#include "genders.h"
#include "genders_api.h"
#include "genders_constants.h"
#include "genders_parsing.h"
#include "genders_util.h"
#include "hash.h"
#include "list.h"

/*
 * genders_errmsg
 *
 * store errormsg strings
 */
static char * genders_errmsg[] =
  {
    "success",
    "genders handle is null",
    "error opening genders file",
    "error reading genders file",
    "genders file parse error",
    "genders data not loaded",
    "genders data already loaded",
    "array or string passed in not large enough to store result",
    "incorrect parameters passed in",
    "null pointer reached in list",
    "node or attribute not found",
    "out of memory",
    "query syntax error",
    "genders handle magic number incorrect, improper handle passed in",
    "unknown internal error",
    "error number out of range",
  };

/*
 * _initialize_handle_info
 *
 * Initialize genders_t handle
 */
static void
_initialize_handle_info(genders_t handle)
{
  handle->magic = GENDERS_MAGIC_NUM;
  handle->is_loaded = 0;
  handle->flags = GENDERS_FLAG_DEFAULT;
  handle->numnodes = 0;
  handle->numattrs = 0;
  handle->maxnodelen = 0;
  handle->maxattrlen = 0;
  handle->maxvallen = 0;
  memset(handle->nodename,'\0',GENDERS_MAXHOSTNAMELEN+1);
  handle->valbuf = NULL;
  handle->attrval_index = NULL;
  handle->attrval_index_attr = NULL;
  handle->attrval_buflist = NULL;

  /* Don't initialize genders_hosts, hostnames, or attrs, they
   * should not be re-initialized on a load_data error.
   */
}

genders_t
genders_handle_create(void)
{
  genders_t handle = NULL;

  /* Don't use the wrapper here, no errnum to set */
  if (!(handle = (genders_t)malloc(sizeof(struct genders))))
    goto cleanup;

  _initialize_handle_info(handle);
  handle->genders_hosts = NULL;
  handle->hostnames = NULL;
  handle->attrs = NULL;
  handle->attrs_size = GENDERS_ATTR_INDEX_INIT_SIZE;

  __list_create(handle->genders_hosts, _genders_list_free_genders_hosts);
  __hostlist_create(handle->hostnames, NULL);
  __hash_create(handle->attrs,
                handle->attrs_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                (hash_del_f)free);

  /* valbuf created in genders_load_data after maxvallen is calculated */

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle;

 cleanup:
  genders_handle_destroy(handle);
  return NULL;
}

int
genders_handle_destroy(genders_t handle)
{
  if (_genders_handle_error_check(handle) < 0)
    return -1;

  __list_destroy(handle->genders_hosts);
  __hostlist_destroy(handle->hostnames);
  __hash_destroy(handle->attrs);
  free(handle->valbuf);
  __hash_destroy(handle->attrval_index);
  free(handle->attrval_index_attr);
  __list_destroy(handle->attrval_buflist);

  /* "clean" handle */
  _initialize_handle_info(handle);
  handle->magic = ~GENDERS_MAGIC_NUM; /* ~0xdeadbeef == 0xlivebeef :-) */
  handle->is_loaded = 0;
  handle->genders_hosts = NULL;
  handle->hostnames = NULL;
  handle->attrs = NULL;
  free(handle);
  return 0;
}

int
genders_load_data(genders_t handle, const char *filename)
{
  char *temp;

  if (_genders_unloaded_handle_error_check(handle) < 0)
    goto cleanup;

  if (_genders_open_and_parse(handle,
                              filename,
                              &handle->numattrs,
                              &handle->maxnodelen,
                              &handle->maxattrlen,
                              &handle->maxvallen,
                              handle->genders_hosts,
                              handle->hostnames,
                              &(handle->attrs),
                              &(handle->attrs_size),
                              0,
                              NULL) < 0)
    goto cleanup;

  hostlist_uniq(handle->hostnames);
  hostlist_sort(handle->hostnames);

  handle->numnodes = hostlist_count(handle->hostnames);

  if (gethostname(handle->nodename, GENDERS_MAXHOSTNAMELEN+1) < 0)
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }
  handle->nodename[GENDERS_MAXHOSTNAMELEN]='\0';

  /* shorten hostname if necessary */
#ifndef WITH_NON_SHORTENED_HOSTNAMES
  if ((temp = strchr(handle->nodename,'.')))
    *temp = '\0';
#endif /* !WITH_NON_SHORTENED_HOSTNAMES */

  handle->maxnodelen = GENDERS_MAX(strlen(handle->nodename), handle->maxnodelen);

  /* Create a buffer for value substitutions */
  __xmalloc(handle->valbuf, char *, handle->maxvallen + 1);

  handle->is_loaded++;
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;

cleanup:
  if (handle && handle->magic == GENDERS_ERR_MAGIC)
    {
      free(handle->valbuf);

      /* Can't pass NULL for key, so pass junk, _genders_list_is_all()
       * and _genders_hash_is_all() will ensure everything is deleted
       */
      list_delete_all(handle->genders_hosts, _genders_list_is_all, "");
      _genders_hostlist_delete_all(handle->hostnames);
      hash_delete_if(handle->attrs, _genders_hash_is_all, "");
      _initialize_handle_info(handle);
    }
  return -1;
}

int
genders_errnum(genders_t handle)
{
  if (!handle)
    return GENDERS_ERR_NULLHANDLE;
  else if (handle->magic != GENDERS_MAGIC_NUM)
    return GENDERS_ERR_MAGIC;
  else
    return handle->errnum;
}

char *
genders_strerror(int errnum)
{
  if (errnum >= GENDERS_ERR_SUCCESS && errnum <= GENDERS_ERR_ERRNUMRANGE)
    return genders_errmsg[errnum];
  else
    return genders_errmsg[GENDERS_ERR_ERRNUMRANGE];
}

char *
genders_errormsg(genders_t handle)
{
  return genders_strerror(genders_errnum(handle));
}

void
genders_perror(genders_t handle, const char *msg)
{
  char *errormsg = genders_strerror(genders_errnum(handle));

  if (!msg)
    fprintf(stderr, "%s\n", errormsg);
  else
    fprintf(stderr, "%s: %s\n", msg, errormsg);
}

int
genders_get_flags(genders_t handle, unsigned int *flags)
{
  if (_genders_handle_error_check(handle) < 0)
    return -1;

  if (!flags)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  *flags = handle->flags;
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int
genders_set_flags(genders_t handle, unsigned int flags)
{
  unsigned int mask = (GENDERS_FLAG_DEFAULT
                       | GENDERS_FLAG_RAW_VALUES);

  if (_genders_handle_error_check(handle) < 0)
    return -1;

  if (flags & ~mask)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  handle->flags = flags;
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int
genders_getnumnodes(genders_t handle)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numnodes;
}

int
genders_getnumattrs(genders_t handle)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numattrs;
}

int
genders_getmaxattrs(genders_t handle)
{
  return genders_getnumattrs(handle);
}

int
genders_getmaxnodelen(genders_t handle)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxnodelen;
}

int
genders_getmaxattrlen(genders_t handle)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrlen;
}

int
genders_getmaxvallen(genders_t handle)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxvallen;
}

/*
 * _genders_list_create
 *
 * Generic list create for genders_nodelist_create,
 * genders_attrlist_create, and genders_vallist_create.
 *
 */
static int
_genders_list_create(genders_t handle, char ***list, int len, int buflen)
{
  char **templist = NULL;
  int i = 0;

  if (len > 0)
    {
      if (!list)
        {
          handle->errnum = GENDERS_ERR_PARAMETERS;
          return -1;
        }

      __xmalloc(templist, char **, sizeof(char *) * len);
      for (i = 0; i < len; i++)
        __xmalloc(templist[i], char *, buflen);
      *list = templist;
    }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return len;

 cleanup:
  if (templist)
    {
      int j;
      for (j = 0; j < i; j++)
        free(templist[j]);
      free(templist);
    }
  return -1;
}

/*
 * _genders_list_clear
 *
 * Generic list clear for genders_nodelist_clear,
 * genders_attrlist_clear, and genders_vallist_clear.
 *
 */
static int
_genders_list_clear(genders_t handle, char **list, int len, int buflen)
{
  if (len > 0)
    {
      int i;

      if (!list)
        {
          handle->errnum = GENDERS_ERR_PARAMETERS;
          return -1;
        }

      for (i = 0; i < len; i++)
        {
          if (!list[i])
            {
              handle->errnum = GENDERS_ERR_NULLPTR;
              return -1;
            }
          memset(list[i], '\0', buflen);
        }
    }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

/*
 * _genders_list_destroy
 *
 * Generic list destroy for genders_nodelist_destroy,
 * genders_attrlist_destroy, and genders_vallist_destroy.
 *
 */
static int
_genders_list_destroy(genders_t handle, char **list, int len)
{
  if (len > 0)
    {
      int i;

      if (!list)
        {
          handle->errnum = GENDERS_ERR_PARAMETERS;
          return -1;
        }

      for (i = 0; i < len; i++)
        free(list[i]);
      free(list);
    }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int
genders_nodelist_create(genders_t handle, char ***list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_create(handle,
                              list,
                              handle->numnodes,
                              handle->maxnodelen+1);
}

int
genders_nodelist_clear(genders_t handle, char **list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_clear(handle,
                             list,
                             handle->numnodes,
                             handle->maxnodelen+1);
}

int
genders_nodelist_destroy(genders_t handle, char **list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_destroy(handle,
                               list,
                               handle->numnodes);
}

int
genders_attrlist_create(genders_t handle, char ***list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_create(handle,
                              list,
                              handle->numattrs,
                              handle->maxattrlen+1);
}

int
genders_attrlist_clear(genders_t handle, char **list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_clear(handle,
                             list,
                             handle->numattrs,
                             handle->maxattrlen+1);
}

int
genders_attrlist_destroy(genders_t handle, char **list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_destroy(handle,
                               list,
                               handle->numattrs);
}

int
genders_vallist_create(genders_t handle, char ***list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_create(handle,
                              list,
                              handle->numattrs,
                              handle->maxvallen+1);
}

int
genders_vallist_clear(genders_t handle, char **list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_clear(handle,
                             list,
                             handle->numattrs,
                             handle->maxvallen+1);
}

int
genders_vallist_destroy(genders_t handle, char **list)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  return _genders_list_destroy(handle,
                               list,
                               handle->numattrs);
}

int
genders_getnodename(genders_t handle, char *node, int len)
{
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!node || len < 0)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  if ((strlen(handle->nodename) + 1) > len)
    {
      handle->errnum = GENDERS_ERR_OVERFLOW;
      return -1;
    }

  strcpy(node, handle->nodename);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int
genders_getnodes(genders_t handle, char *nodes[], int len,
                 const char *attr, const char *val)
{
  hostlist_iterator_t hlitr = NULL;
  char *node = NULL;
  ListIterator hosts_itr = NULL;
  ListIterator itr = NULL;
  int index = 0, rv = -1;

  if (_genders_loaded_handle_error_check(handle) < 0)
    goto cleanup;

  if ((!nodes && len > 0) || len < 0)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  if (attr && !strlen(attr))
    attr = NULL;

  if (val && !strlen(val))
    val = NULL;

  if (handle->attrval_index
      && attr
      && val
      && !strcmp(handle->attrval_index_attr, attr))
    {
      /* Case A: Use attrval_index to find nodes */
      hostlist_t hl;

      if (!(hl = hash_find(handle->attrval_index, val)))
        {
          /* No attributes with this value */
          handle->errnum = GENDERS_ERR_SUCCESS;
          return 0;
        }

      __hostlist_iterator_create(hlitr, hl);
      while ((node = hostlist_next(hlitr)))
        {
          if (_genders_put_in_array(handle, node, nodes, index++, len) < 0)
            goto cleanup;
          free(node);
        }
    }
  else if (attr)
    {
      /* Case B: atleast the attr was input */
      genders_hosts_t gh;

      if (!handle->numattrs)
        {
          /* No attributes, so no nodes have this attr */
          handle->errnum = GENDERS_ERR_SUCCESS;
          return 0;
        }

      __list_iterator_create(hosts_itr, handle->genders_hosts);
      while ((gh = list_next(hosts_itr)))
        {
          genders_attrval_t av;
          if ((av = hash_find(gh->attrval_index, attr)))
            {
              if (val)
                {
                  if (av->val)
                    {
                      if (av->val_contains_subst)
                        {
                          char *valptr;
                          __hostlist_iterator_create(hlitr, gh->hl);
                          while ((node = hostlist_next(hlitr)))
                            {
                              if (_genders_get_valptr(handle, node, av, &valptr, NULL) < 0)
                                goto cleanup;
                              if (!strcmp(val, valptr))
                                {
                                  if (_genders_put_in_array(handle, node, nodes, index++, len) < 0)
                                    goto cleanup;
                                }
                              free(node);
                            }
                          node = NULL;
                        }
                      else
                        {
                          if (!strcmp(val, av->val))
                            {
                              __hostlist_iterator_create(hlitr, gh->hl);
                              while ((node = hostlist_next(hlitr)))
                                {
                                  if (_genders_put_in_array(handle, node, nodes, index++, len) < 0)
                                    goto cleanup;
                                  free(node);
                                }
                              node = NULL;
                            }
                        }
                    }
                }
              else
                {
                  __hostlist_iterator_create(hlitr, gh->hl);
                  while ((node = hostlist_next(hlitr)))
                    {
                      if (_genders_put_in_array(handle, node, nodes, index++, len) < 0)
                        goto cleanup;
                      free(node);
                    }
                  node = NULL;
                }
            }
        }
    }
  else
    {
      __hostlist_iterator_create(hlitr, handle->hostnames);
      while ((node = hostlist_next(hlitr)))
        {
          if (_genders_put_in_array(handle, node, nodes, index++, len) < 0)
            goto cleanup;
          free(node);
        }
      node = NULL;
    }

  rv = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __hostlist_iterator_destroy(hlitr);
  free(node);

  __list_iterator_destroy(itr);
  __list_iterator_destroy(hosts_itr);
  return rv;
}

struct getattr_container
{
  genders_t handle;
  const char *node;
  char **attrs;
  char **vals;
  int index;
  int len;
  int errnum;
};

/*
 * _store_attr_val
 *
 * store attr & val into user results
 */
static int
_store_attr_val(void *data, const void *key, void *arg)
{
  struct getattr_container *gc = arg;
  genders_attrval_t av = data;

  if (_genders_put_in_array(gc->handle,
                            (char *)key,
                            gc->attrs,
                            gc->index,
                            gc->len) < 0)
    goto cleanup;
  if (gc->vals && av->val)
    {
      char *valptr;
      if (_genders_get_valptr(gc->handle, gc->node, av, &valptr, NULL) < 0)
        goto cleanup;
      if (_genders_put_in_array(gc->handle,
                                valptr,
                                gc->vals,
                                gc->index,
                                gc->len) < 0)
        goto cleanup;
    }
  gc->index++;
  return 1;

 cleanup:
  /* we save the first error hit */
  if (!gc->errnum)
    gc->errnum = gc->handle->errnum;
  return -1;
}

int
genders_getattr(genders_t handle,
                char *attrs[],
                char *vals[],
                int len,
                const char *node)
{
  struct getattr_container gc = {0};
  ListIterator hosts_itr = NULL;
  genders_hosts_t gh;
  int rv = -1;

  if (_genders_loaded_handle_error_check(handle) < 0)
    goto cleanup;

  if ((!attrs && len > 0) || len < 0)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  if (!node || !strlen(node))
    node = handle->nodename;

  if (!handle->numnodes)
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  if (hostlist_find(handle->hostnames, node) < 0)
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  gc.handle = handle;
  gc.node = node;
  gc.attrs = attrs;
  gc.vals = vals;
  gc.index = 0;
  gc.len = len;
  gc.errnum = 0;
  __list_iterator_create(hosts_itr, handle->genders_hosts);
  while ((gh = list_next(hosts_itr)))
    {
      int ret = hostlist_find(gh->hl, node);
      if (ret >= 0)
        {
          ret = hash_for_each(gh->attrval_index, _store_attr_val, &gc);
          if (ret != hash_count(gh->attrval_index))
            {
              handle->errnum = gc.errnum;
              goto cleanup;
            }
        }
    }

  rv = gc.index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_iterator_destroy(hosts_itr);
  return rv;
}

struct getattr_all_container
{
  genders_t handle;
  char **attrs;
  int index;
  int len;
  int errnum;
};

/*
 * _store_attr_val
 *
 * store attr into user results
 */
static int
_store_attr(void *data, const void *key, void *arg)
{
  struct getattr_all_container *gac = arg;
  if (_genders_put_in_array(gac->handle,
                            (char *)key,
                            gac->attrs,
                            gac->index++,
                            gac->len) < 0)
    {
      /* we save the first error hit */
      if (!gac->errnum)
        gac->errnum = gac->handle->errnum;
      return -1;
    }
  return 1;
}

int
genders_getattr_all(genders_t handle, char *attrs[], int len)
{
  struct getattr_all_container gac;
  int ret;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((!attrs && len > 0) || len < 0)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  if (handle->numattrs > len)
    {
      handle->errnum = GENDERS_ERR_OVERFLOW;
      return -1;
    }

  gac.handle = handle;
  gac.attrs = attrs;
  gac.index = 0;
  gac.len = len;
  gac.errnum = 0;
  ret = hash_for_each(handle->attrs, _store_attr, &gac);
  if (ret != handle->numattrs)
    {
      handle->errnum = gac.errnum;
      return -1;
    }
  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numattrs;
}

int
genders_testattr(genders_t handle,
                 const char *node,
                 const char *attr,
                 char *val,
                 int len)
{
  ListIterator hosts_itr = NULL;
  genders_hosts_t gh;
  int rv = -1;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!attr
      || !strlen(attr)
      || (val && len < 0))
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  if (!node || !strlen(node))
    node = handle->nodename;

  if (!handle->numnodes)
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  if (hostlist_find(handle->hostnames, node) < 0)
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  __list_iterator_create(hosts_itr, handle->genders_hosts);
  while ((gh = list_next(hosts_itr)))
    {
      int ret = hostlist_find(gh->hl, node);
      if (ret >= 0)
        {
          genders_attrval_t av;
          if ((av = hash_find(gh->attrval_index, attr)))
            {
              if (val)
                {
                  if (av->val)
                    {
                      char *valptr;
                      if (_genders_get_valptr(handle, node, av, &valptr, NULL) < 0)
                        return -1;
                      if ((strlen(valptr) + 1) > len)
                        {
                          handle->errnum = GENDERS_ERR_OVERFLOW;
                          return -1;
                        }
                      strcpy(val, valptr);
                    }
                  else
                    memset(val, '\0', len);
                }
              rv = 1;
              goto out;
            }
        }
    }

  rv = 0;
 out:
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_iterator_destroy(hosts_itr);
  return rv;
}

int
genders_testattrval(genders_t handle,
                    const char *node,
                    const char *attr,
                    const char *val)
{
  char *tmpbuf = NULL;
  int ret, rv = -1;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!attr || !strlen(attr))
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  if (val && !strlen(val))
    val = NULL;

  __xmalloc(tmpbuf, char *, handle->maxvallen + 1);

  if ((ret = genders_testattr(handle,
                              node,
                              attr,
                              tmpbuf,
                              handle->maxvallen + 1)) < 0)
    return -1;

  if (ret && (!val || !strcmp(tmpbuf, val)))
    rv = 1;
  else
    rv = 0;

  free(tmpbuf);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return rv;

 cleanup:
  free(tmpbuf);
  return -1;
}

int
genders_isnode(genders_t handle, const char *node)
{
  int ret;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!node || !strlen(node))
    node = handle->nodename;

  if (!handle->numnodes)
    {
      /* No nodes, so node not found */
      handle->errnum = GENDERS_ERR_SUCCESS;
      return 0;
    }

  ret = hostlist_find(handle->hostnames, node);
  return ((ret >= 0) ? 1 : 0);
}

int
genders_isattr(genders_t handle, const char *attr)
{
  void *ptr;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!attr || !strlen(attr))
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  if (!handle->numattrs)
    {
      /* No attributes, so attr not found */
      handle->errnum = GENDERS_ERR_SUCCESS;
      return 0;
    }

  ptr = hash_find(handle->attrs, attr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return ((ptr) ? 1 : 0);
}

int
genders_isattrval(genders_t handle, const char *attr, const char *val)
{
  ListIterator hosts_itr = NULL;
  hostlist_iterator_t hlitr = NULL;
  char *node = NULL;
  genders_hosts_t gh;
  int rv = -1;

  if (_genders_loaded_handle_error_check(handle) < 0)
    goto cleanup;

  if (!attr
      || !strlen(attr)
      || !val
      || !strlen(val))
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  if (handle->attrval_index
      && !strcmp(handle->attrval_index_attr, attr))
    {
      if (!hash_find(handle->attrval_index, val))
        rv = 0;
      else
        rv = 1;

      handle->errnum = GENDERS_ERR_SUCCESS;
      return rv;
    }
  else
    {
      if (!handle->numattrs)
        goto out;

      __list_iterator_create(hosts_itr, handle->genders_hosts);
      while ((gh = list_next(hosts_itr)))
        {
          genders_attrval_t av;
          if ((av = hash_find(gh->attrval_index, attr)))
            {
              if (!strcmp(av->attr, attr))
                {
                  if (av->val)
                    {
                      __hostlist_iterator_create(hlitr, gh->hl);
                      while ((node = hostlist_next(hlitr)))
                        {
                          char *valptr;
                          if (_genders_get_valptr(handle, node, av, &valptr, NULL) < 0)
                            goto cleanup;
                          if (!strcmp(valptr, val))
                            {
                              rv = 1;
                              handle->errnum = GENDERS_ERR_SUCCESS;
                              goto cleanup;
                            }
                          free(node);
                        }
                      node = NULL;
                    }
                }
            }
        }
    }

 out:
  rv = 0;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __hostlist_iterator_destroy(hlitr);
  __list_iterator_destroy(hosts_itr);
  free(node);
  return rv;
}

int
genders_index_attrvals(genders_t handle, const char *attr)
{
  hostlist_iterator_t hlitr = NULL;
  ListIterator hosts_itr = NULL;
  genders_hosts_t gh = NULL;
  char *node = NULL;
  hostlist_t hl = NULL;
  List attrval_buflist = NULL;
  char *valbuf = NULL;
  hash_t attrval_index = NULL;
  char *attrval_index_attr = NULL;
  int rv;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!attr || !strlen(attr))
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  /* check if attr is legit */

  if ((rv = genders_isattr(handle, attr)) < 0)
    goto cleanup;

  if (!rv)
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      goto cleanup;
    }

  /* check if index already created */
  if (handle->attrval_index && !strcmp(handle->attrval_index_attr, attr))
    {
      handle->errnum = GENDERS_ERR_SUCCESS;
      return 0;
    }

  /* Nothing to index if there are no nodes */
  if (!handle->numnodes)
    {
      handle->errnum = GENDERS_ERR_SUCCESS;
      return 0;
    }

  /* Max possible hash size is number of nodes, so pick upper boundary */
  __hash_create(attrval_index,
                handle->numnodes,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                (hash_del_f)hostlist_destroy);

  /* Create a List to store buffers for later freeing */
  __list_create(attrval_buflist, free);

  __list_iterator_create(hosts_itr, handle->genders_hosts);
  while ((gh = list_next(hosts_itr)))
    {
      genders_attrval_t av;

      if ((av = hash_find(gh->attrval_index, attr)))
        {
          if (av->val_contains_subst)
            {
              __hostlist_iterator_create(hlitr, gh->hl);
              while ((node = hostlist_next(hlitr)))
                {
                  char *valptr;
                  int subst_occurred;
                  if (_genders_get_valptr(handle,
                                          node,
                                          av,
                                          &valptr,
                                          &subst_occurred) < 0)
                    goto cleanup;
                  if (!(hl = hash_find(attrval_index, valptr)))
                    {
                      __hostlist_create(hl, NULL);
                      /* If a substitution occurred, we cannot use the av->val
                       * pointer as the key, b/c the key contains some nonsense
                       * characters (i.e. %n).  So we have to copy this buffer and
                       * store it somewhere to be freed later.
                       */
                      if (subst_occurred)
                        {
                          __xstrdup(valbuf, valptr);
                          __list_append(attrval_buflist, valbuf);
                          valptr = valbuf;
                          valbuf = NULL;
                        }
                      __hash_insert(attrval_index, valptr, hl);
                    }
                  if (!hostlist_push_host(hl, node))
                    {
                      handle->errnum = GENDERS_ERR_INTERNAL;
                      goto cleanup;
                    }
                  hl = NULL;
                  free(node);
                }
              node = NULL;
              __hostlist_iterator_destroy(hlitr);
            }
          else
            {
              if (!(hl = hash_find(attrval_index,
                                   av->val ? av->val : GENDERS_NOVALUE)))
                {
                  __hostlist_create(hl, NULL);
                  __hash_insert(attrval_index,
                                av->val ? av->val : GENDERS_NOVALUE,
                                hl);
                }
              if (!hostlist_push_list(hl, gh->hl))
                {
                  handle->errnum = GENDERS_ERR_INTERNAL;
                  goto cleanup;
                }
              hl = NULL;
            }
        }
    }

  __xstrdup(attrval_index_attr, attr);

  /* Delete/free previous index */
  __hash_destroy(handle->attrval_index);
  handle->attrval_index = NULL;
  free(handle->attrval_index_attr);
  handle->attrval_index_attr = NULL;
  __list_destroy(handle->attrval_buflist);
  handle->attrval_buflist = NULL;

  handle->attrval_index = attrval_index;
  handle->attrval_index_attr = attrval_index_attr;
  handle->attrval_buflist = attrval_buflist;

  __list_iterator_destroy(hosts_itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;

 cleanup:
  __list_iterator_destroy(hosts_itr);
  __hostlist_iterator_destroy(hlitr);
  __hostlist_destroy(hl);
  free(node);

  __hash_destroy(attrval_index);
  __list_destroy(attrval_buflist);
  free(attrval_index_attr);
  free(valbuf);
  return -1;
}

int
genders_parse(genders_t handle, const char *filename, FILE *stream)
{
  int errcount, rv = -1;
  int debugnumattrs = 0;
  int debugmaxnodelen = 0;
  int debugmaxattrlen = 0;
  int debugmaxvallen = 0;
  List debuggenders_hosts = NULL;
  hostlist_t debughostnames = NULL;
  hash_t debugattrs = NULL;
  int debugattrs_size = GENDERS_ATTR_INDEX_INIT_SIZE;

  if (_genders_handle_error_check(handle) < 0)
    goto cleanup;

  if (!stream)
    stream = stderr;

  __list_create(debuggenders_hosts, _genders_list_free_genders_hosts);
  __hostlist_create(debughostnames, NULL);
  __hash_create(debugattrs,
                debugattrs_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                (hash_del_f)free);

  if ((errcount = _genders_open_and_parse(handle,
                                          filename,
                                          &debugnumattrs,
                                          &debugmaxnodelen,
                                          &debugmaxattrlen,
                                          &debugmaxvallen,
                                          debuggenders_hosts,
                                          debughostnames,
                                          &(debugattrs),
                                          &(debugattrs_size),
                                          1,
                                          stream)) < 0)
    goto cleanup;

  rv = errcount;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_destroy(debuggenders_hosts);
  __hostlist_destroy(debughostnames);
  __hash_destroy(debugattrs);
  return rv;
}

void
genders_set_errnum(genders_t handle, int errnum)
{
  if (_genders_handle_error_check(handle) < 0)
    return;

  handle->errnum = errnum;
}

struct genders_copy_attrvals_container
{
  genders_t handle;
  genders_hosts_t ghcopy;
  int errnum;
};

/*
 * _copy_attrvals
 *
 * copy attrvals into new handle
 */
static int
_copy_attrvals(void *data, const void *key, void *arg)
{
  struct genders_copy_attrvals_container *gcavc = arg;
  genders_t handle = gcavc->handle;
  genders_hosts_t ghcopy = gcavc->ghcopy;
  genders_attrval_t av = data;
  genders_attrval_t avcopy;

  __xmalloc(avcopy, genders_attrval_t, sizeof(struct genders_attrval));
  avcopy->val_contains_subst = av->val_contains_subst;

  __xstrdup(avcopy->attr, av->attr);
  if (av->val)
    __xstrdup(avcopy->val, av->val);
  else
    avcopy->val = NULL;

  __hash_insert(ghcopy->attrval_index, avcopy->attr, avcopy);

  return 1;

 cleanup:
  if (avcopy)
    {
      free(avcopy->attr);
      free(avcopy->val);
      free(avcopy);
    }
  /* we save the first error hit */
  if (!gcavc->errnum)
    gcavc->errnum = handle->errnum;
  return -1;
}

/*
 * _genders_copy_fill_node_index
 *
 * Add keys into the node_index
 *
 */
static int _genders_copy_genders_hosts(genders_t handle, genders_t handlecopy)
{
  ListIterator hosts_itr = NULL;
  struct genders_copy_attrvals_container gcavc;
  genders_hosts_t ghcopy = NULL;
  genders_hosts_t gh;
  int rv = -1;

  __list_iterator_create(hosts_itr, handle->genders_hosts);
  while ((gh = list_next(hosts_itr)))
    {
      int ret;
      __xmalloc(ghcopy, genders_hosts_t, sizeof(struct genders_hosts));
      if (!(ghcopy->hl = hostlist_copy(gh->hl)))
        {
          handle->errnum = GENDERS_ERR_OUTMEM;
          goto cleanup;
        }
      ghcopy->attrval_index_size = gh->attrval_index_size;
      __hash_create(ghcopy->attrval_index,
                    ghcopy->attrval_index_size,
                    (hash_key_f)hash_key_string,
                    (hash_cmp_f)strcmp,
                    (hash_del_f)_genders_hash_free_genders_attrval);
      gcavc.handle = handle;
      gcavc.ghcopy = ghcopy;
      gcavc.errnum = 0;
      ret = hash_for_each(gh->attrval_index, _copy_attrvals, &gcavc);
      if (ret != hash_count(gh->attrval_index))
        {
          handle->errnum = gcavc.errnum;
          goto cleanup;
        }
      __list_append(handlecopy->genders_hosts, ghcopy);
      ghcopy = NULL;
    }

  rv = 0;
 cleanup:
  __list_iterator_destroy(hosts_itr);
  if (ghcopy)
    {
      __hostlist_destroy(ghcopy->hl);
      __hash_destroy(ghcopy->attrval_index);
      free(ghcopy);
    }
  return rv;
}

struct genders_copy_attrs_container
{
  genders_t handle;
  genders_t handlecopy;
  int errnum;
};

/*
 * _copy_attr
 *
 * copy attr into new handle
 */
static int
_copy_attr(void *data, const void *key, void *arg)
{
  struct genders_copy_attrs_container *gcac = arg;
  genders_t handle = gcac->handle;
  genders_t handlecopy = gcac->handlecopy;
  char *attrcopy;

  __xstrdup(attrcopy, (char *)key);
  if (!hash_insert(handlecopy->attrs, attrcopy, attrcopy))
    {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }
  return 1;

 cleanup:
  /* we save the first error hit */
  if (!gcac->errnum)
    gcac->errnum = handle->errnum;
  return -1;
}

/*
 * _genders_copy_attrs
 *
 * Add keys into the attrs hash
 *
 */
static int _genders_copy_attrs(genders_t handle, genders_t handlecopy)
{
  struct genders_copy_attrs_container gcac;
  int ret;

  gcac.handle = handle;
  gcac.handlecopy = handlecopy;
  gcac.errnum = 0;
  ret = hash_for_each(handle->attrs, _copy_attr, &gcac);
  if (ret != handle->numattrs)
    {
      handle->errnum = gcac.errnum;
      return -1;
    }
  return 0;
}

genders_t
genders_copy(genders_t handle)
{
  genders_t handlecopy = NULL;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return NULL;

  if (!(handlecopy = genders_handle_create()))
    {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }

  handlecopy->is_loaded = handle->is_loaded;
  handlecopy->flags = handle->flags;
  handlecopy->numnodes = handle->numnodes;
  handlecopy->numattrs = handle->numattrs;
  handlecopy->maxnodelen = handle->maxnodelen;
  handlecopy->maxattrlen = handle->maxattrlen;
  handlecopy->maxvallen = handle->maxvallen;

  memcpy(handlecopy->nodename, handle->nodename, GENDERS_MAXHOSTNAMELEN+1);

  if (_genders_copy_genders_hosts(handle, handlecopy) < 0)
    goto cleanup;

  if (!hostlist_push_list(handlecopy->hostnames, handle->hostnames))
    {
      /* Except if no hosts in handle->hostnames */
      if (hostlist_count(handle->hostnames) > 0)
        {
          handle->errnum = GENDERS_ERR_INTERNAL;
          goto cleanup;
        }
      /* else fallthrough */
    }

  if (_genders_copy_attrs(handle, handlecopy) < 0)
    goto cleanup;

  /* Create a buffer for value substitutions */
  __xmalloc(handlecopy->valbuf, char *, handlecopy->maxvallen + 1);

  /* attrval_index, attrval_index_attr, and attrval_buflist
   * set/re-created by genders_index_attrvals
   */
  if (handle->attrval_index)
    {
      if (genders_index_attrvals(handlecopy, handle->attrval_index_attr) < 0)
        {
          handle->errnum = GENDERS_ERR_INTERNAL;
          goto cleanup;
        }
    }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handlecopy;

 cleanup:
  if (handlecopy)
    (void)genders_handle_destroy(handlecopy);
  return NULL;
}
