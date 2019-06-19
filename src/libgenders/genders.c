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
  handle->maxattrs = 0;
  handle->maxnodelen = 0;
  handle->maxattrlen = 0;
  handle->maxvallen = 0;
  memset(handle->nodename,'\0',GENDERS_MAXHOSTNAMELEN+1);
  handle->valbuf = NULL;
  handle->node_index = NULL;
  handle->node_index_size = 0;
  handle->attr_index = NULL;
  handle->attr_index_size = 0;
  handle->attrval_index = NULL;
  handle->attrval_index_attr = NULL;

  /* Don't initialize the nodeslist, attrvalslist, or attrslist, they
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
  handle->nodeslist = NULL;
  handle->attrvalslist = NULL;
  handle->attrslist = NULL;
  handle->attrval_buflist = NULL;
  
  __list_create(handle->nodeslist, _genders_list_free_genders_node);
  __list_create(handle->attrvalslist, _genders_list_free_attrvallist);
  __list_create(handle->attrslist, free);

  /* node_index, attr_index created in genders_load_data.  Valbuf
   * created in genders_load_data after maxvallen is calculated
   */
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle;
  
 cleanup:
  if (handle) 
    {
      __list_destroy(handle->nodeslist);
      __list_destroy(handle->attrvalslist);
      __list_destroy(handle->attrslist);
      free(handle);
    }
  return NULL;
}

int 
genders_handle_destroy(genders_t handle)
{
  if (_genders_handle_error_check(handle) < 0)
    return -1;

  __list_destroy(handle->nodeslist);
  __list_destroy(handle->attrvalslist);
  __list_destroy(handle->attrslist);
  free(handle->valbuf);
  __hash_destroy(handle->node_index);
  __hash_destroy(handle->attr_index);
  __hash_destroy(handle->attrval_index);
  free(handle->attrval_index_attr);
  __list_destroy(handle->attrval_buflist);

  /* "clean" handle */
  _initialize_handle_info(handle);
  handle->magic = ~GENDERS_MAGIC_NUM; /* ~0xdeadbeef == 0xlivebeef :-) */
  handle->is_loaded = 0;
  handle->nodeslist = NULL;
  handle->attrvalslist = NULL;
  handle->attrslist = NULL;
  free(handle);
  return 0;
}

int
genders_load_data(genders_t handle, const char *filename) 
{
  char *temp;

  if (_genders_unloaded_handle_error_check(handle) < 0)
    goto cleanup;
  
  handle->node_index_size = GENDERS_NODE_INDEX_INIT_SIZE;
  
  __hash_create(handle->node_index,
                handle->node_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp, 
                NULL);
  
  handle->attr_index_size = GENDERS_ATTR_INDEX_INIT_SIZE;
  
  __hash_create(handle->attr_index,
                handle->attr_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp, 
                (hash_del_f)list_destroy);

  if (_genders_open_and_parse(handle, 
			      filename, 
			      &handle->numattrs,
			      &handle->maxattrs,
			      &handle->maxnodelen,
			      &handle->maxattrlen,
			      &handle->maxvallen,
			      handle->nodeslist, 
			      handle->attrvalslist,
			      handle->attrslist,
                              &(handle->node_index),
                              &(handle->node_index_size),
                              &(handle->attr_index),
                              &(handle->attr_index_size),
			      0, 
			      NULL) < 0)
    goto cleanup;

  handle->numnodes = list_count(handle->nodeslist);

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
       * will ensure everything is deleted
       */
      list_delete_all(handle->nodeslist, _genders_list_is_all, ""); 
      list_delete_all(handle->attrvalslist, _genders_list_is_all, ""); 
      list_delete_all(handle->attrslist, _genders_list_is_all, ""); 
      __hash_destroy(handle->node_index);
      __hash_destroy(handle->attr_index);
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
  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrs;
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
  ListIterator itr = NULL;
  genders_node_t n;
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
      List l;
      
      if (!(l = hash_find(handle->attrval_index, val))) 
	{
	  /* No attributes with this value */
	  handle->errnum = GENDERS_ERR_SUCCESS;
	  return 0;
	}

      __list_iterator_create(itr, l);
      while ((n = list_next(itr))) 
	{
	  if (_genders_put_in_array(handle, n->name, nodes, index++, len) < 0)
	    goto cleanup;
	}
    }
  else if (attr) 
    {
      /* Case B: atleast the attr was input, so use attr_index */
      List l;
      
      if (!handle->numattrs)
        {
	  /* No attributes, so no nodes have this attr */
	  handle->errnum = GENDERS_ERR_SUCCESS;
	  return 0;
        }

      if (!(l = hash_find(handle->attr_index, attr))) 
	{
	  /* No nodes have this attr */
	  handle->errnum = GENDERS_ERR_SUCCESS;
	  return 0;
	}

      __list_iterator_create(itr, l);
      while ((n = list_next(itr))) 
	{
	  genders_attrval_t av;
	  
	  /* val could be NULL */
	  if (_genders_find_attrval(handle, n, attr, val, &av) < 0)
	    goto cleanup;
	  
	  if (av && _genders_put_in_array(handle, n->name, nodes, index++, len) < 0)
	    goto cleanup;
	}
    }
  else 
    {
      /* Case C: get every node */
      __list_iterator_create(itr, handle->nodeslist);
      while ((n = list_next(itr))) 
	{
	  if (_genders_put_in_array(handle, n->name, nodes, index++, len) < 0)
	    goto cleanup;
	}
    }
  
  rv = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_iterator_destroy(itr);
  return rv;
}

int 
genders_getattr(genders_t handle, 
		char *attrs[], 
		char *vals[],
                int len, 
		const char *node) 
{
  ListIterator attrlist_itr = NULL;
  ListIterator attrvals_itr = NULL;
  genders_attrvals_container_t avc;
  genders_node_t n;
  int index = 0, rv = -1;

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

  if (!(n = hash_find(handle->node_index, node))) 
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  __list_iterator_create(attrlist_itr, n->attrlist);
  while ((avc = list_next(attrlist_itr))) 
    {
      genders_attrval_t av;
      
      __list_iterator_create(attrvals_itr, avc->attrvals);
      while ((av = list_next(attrvals_itr))) 
	{
	  if (_genders_put_in_array(handle, av->attr, attrs, index, len) < 0)
	    goto cleanup;
      
	  if (vals && av->val) 
	    {
	      char *valptr;
	      if (_genders_get_valptr(handle, n, av, &valptr, NULL) < 0)
		goto cleanup;
	      if (_genders_put_in_array(handle, valptr, vals, index, len) < 0)
		goto cleanup;
	    }
	  index++;
	}
      __list_iterator_destroy(attrvals_itr);
    }
  attrvals_itr = NULL;
  
  rv = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_iterator_destroy(attrlist_itr);
  __list_iterator_destroy(attrvals_itr);
  return rv;  
}

int 
genders_getattr_all(genders_t handle, char *attrs[], int len) 
{
  ListIterator attrslist_itr = NULL; 
  char *attr;
  int index = 0, rv = -1;
  
  if (_genders_loaded_handle_error_check(handle) < 0)
    goto cleanup;

  if ((!attrs && len > 0) || len < 0) 
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  if (handle->numattrs > len) 
    {
      handle->errnum = GENDERS_ERR_OVERFLOW;
      goto cleanup;
    }

  __list_iterator_create(attrslist_itr, handle->attrslist);
  while ((attr = list_next(attrslist_itr))) 
    {
      if (_genders_put_in_array(handle, attr, attrs, index++, len) < 0)
	goto cleanup;
    }

  rv = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_iterator_destroy(attrslist_itr);
  return rv;  
}

int 
genders_testattr(genders_t handle, 
		 const char *node, 
		 const char *attr, 
                 char *val, 
		 int len) 
{
  genders_node_t n;
  genders_attrval_t av;

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

  if (!(n = hash_find(handle->node_index, node))) 
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  if (_genders_find_attrval(handle, n, attr, NULL, &av) < 0)
    return -1;

  if (av) 
    {
      if (val) 
	{
	  if (av->val) 
	    {
	      char *valptr;
	      if (_genders_get_valptr(handle, n, av, &valptr, NULL) < 0)
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
    }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return ((av) ? 1 : 0);
}

int 
genders_testattrval(genders_t handle, 
		    const char *node, 
                    const char *attr, 
		    const char *val) 
{
  genders_node_t n;
  genders_attrval_t av;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!attr || !strlen(attr)) 
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

  if (!node || !strlen(node))
    node = handle->nodename;

  if (val && !strlen(val))
    val = NULL;

  if (!handle->numnodes)
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  if (!(n = hash_find(handle->node_index, node))) 
    {
      handle->errnum = GENDERS_ERR_NOTFOUND;
      return -1;
    }

  if (_genders_find_attrval(handle, n, attr, val, &av) < 0)
    return -1;
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return ((av) ? 1 : 0);
}

int 
genders_isnode(genders_t handle, const char *node) 
{
  genders_node_t n;

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

  n = hash_find(handle->node_index, node);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return ((n) ? 1 : 0);
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

  ptr = hash_find(handle->attr_index, attr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return ((ptr) ? 1 : 0);
}

int 
genders_isattrval(genders_t handle, const char *attr, const char *val) 
{
  ListIterator nodeslist_itr = NULL;
  genders_node_t n;
  genders_attrval_t av;
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
      List l;

      if (!handle->numattrs)
        goto out;

      if (!(l = hash_find(handle->attr_index, attr)))
        goto out;

      __list_iterator_create(nodeslist_itr, l);
      while ((n = list_next(nodeslist_itr))) 
	{
	  if (_genders_find_attrval(handle, n, attr, val, &av) < 0) 
	    goto cleanup;
	  if (av) 
	    {
	      rv = 1;
	      handle->errnum = GENDERS_ERR_SUCCESS;
	      goto cleanup;
	    }
	}
    }

 out:
  rv = 0;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_iterator_destroy(nodeslist_itr);
  return rv;
}

int
genders_index_attrvals(genders_t handle, const char *attr)
{
  ListIterator nodeslist_itr = NULL;
  List l = NULL;
  List attrval_buflist = NULL;
  genders_node_t n;
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
                (hash_del_f)list_destroy);

  /* Create a List to store buffers for later freeing */
  __list_create(attrval_buflist, free);

  __list_iterator_create(nodeslist_itr, handle->nodeslist);
  while ((n = list_next(nodeslist_itr))) 
    {
      int subst_occurred = 0;
      genders_attrval_t av;
      
      if (_genders_find_attrval(handle, n, attr, NULL, &av) < 0)
	goto cleanup;

      if (av) 
	{
	  char *valptr;
	  
	  if (av->val) 
	    {
	      if (_genders_get_valptr(handle, 
				      n, 
				      av, 
				      &valptr, 
				      &subst_occurred) < 0)
		goto cleanup;
	    }
	  else
	    valptr = GENDERS_NOVALUE;

	  if (!(l = hash_find(attrval_index, valptr))) 
	    {
	      __list_create(l, NULL);
	      
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
	      
	      __hash_insert(attrval_index, valptr, l);
	    }
      
	  __list_append(l, n);
	  l = NULL;
	}
    }

  __xstrdup(attrval_index_attr, attr);

  /* Delete/free previous index */
  __hash_destroy(handle->attrval_index);
  free(handle->attrval_index_attr);
  __list_destroy(handle->attrval_buflist);
  handle->attrval_index = NULL;
  handle->attrval_index_attr = NULL;

  handle->attrval_index = attrval_index;
  handle->attrval_index_attr = attrval_index_attr;
  handle->attrval_buflist = attrval_buflist;

  __list_iterator_destroy(nodeslist_itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
  
 cleanup:
  __list_iterator_destroy(nodeslist_itr);
  __list_destroy(l);
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
  int debugmaxattrs = 0;
  int debugmaxnodelen = 0;
  int debugmaxattrlen = 0;
  int debugmaxvallen = 0;
  List debugnodeslist = NULL;
  List debugattrvalslist = NULL;
  List debugattrslist = NULL;
  hash_t debugnode_index = NULL;
  int debugnode_index_size = GENDERS_NODE_INDEX_INIT_SIZE;
  hash_t debugattr_index = NULL;
  int debugattr_index_size = GENDERS_ATTR_INDEX_INIT_SIZE;

  if (_genders_handle_error_check(handle) < 0)
    goto cleanup;

  if (!stream)
    stream = stderr;

  __list_create(debugnodeslist, _genders_list_free_genders_node);
  __list_create(debugattrvalslist, _genders_list_free_attrvallist);
  __list_create(debugattrslist, free);
  __hash_create(debugnode_index,
                debugnode_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp, 
                NULL);
  __hash_create(debugattr_index,
                debugattr_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp, 
                (hash_del_f)list_destroy);

  if ((errcount = _genders_open_and_parse(handle, 
					  filename,
					  &debugnumattrs,
					  &debugmaxattrs,
					  &debugmaxnodelen,
					  &debugmaxattrlen,
					  &debugmaxvallen,
					  debugnodeslist, 
					  debugattrvalslist,
					  debugattrslist,
                                          &(debugnode_index),
                                          &(debugnode_index_size),
                                          &(debugattr_index),
                                          &(debugattr_index_size),
					  1, 
					  stream)) < 0)
    goto cleanup;

  rv = errcount;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __list_destroy(debugnodeslist);
  __list_destroy(debugattrvalslist);
  __list_destroy(debugattrslist);
  __hash_destroy(debugnode_index);
  __hash_destroy(debugattr_index);
  return rv;
}

void 
genders_set_errnum(genders_t handle, int errnum) 
{
  if (_genders_handle_error_check(handle) < 0)
    return;

  handle->errnum = errnum;
}

/* 
 * _genders_copy_nodeslist
 *
 * Copy contents of the nodeslist list into the handlecopy.
 *
 */
static int
_genders_copy_nodeslist(genders_t handle, genders_t handlecopy)
{
  ListIterator itr = NULL;
  genders_node_t n = NULL;
  genders_node_t newn = NULL;
  int rv = -1;

  __list_iterator_create(itr, handle->nodeslist);
  while ((n = list_next(itr)))
    {
      __xmalloc(newn, genders_node_t, sizeof(struct genders_node));
      __xstrdup(newn->name, n->name);
      __list_create(newn->attrlist, NULL);
      newn->attrcount = n->attrcount;
      newn->attrlist_index_size = n->attrlist_index_size;
      __hash_create(newn->attrlist_index,
                    newn->attrlist_index_size,
                    (hash_key_f)hash_key_string,
                    (hash_cmp_f)strcmp,
                    NULL);

      __list_append(handlecopy->nodeslist, newn);
      newn = NULL;
    }
  
  rv = 0;
 cleanup:
  if (rv < 0)
    {
      if (newn)
	{
	  free(newn->name);
	  __list_destroy(newn->attrlist);
	  __hash_destroy(newn->attrlist_index);
	  free(newn);
	}
    }
  __list_iterator_destroy(itr);
  return rv;
}

/* 
 * _genders_copy_fill_node_index
 *
 * Add keys into the node_index
 *
 */
static int _genders_copy_fill_node_index(genders_t handle, genders_t handlecopy)
{
  List l = NULL;
  ListIterator itr = NULL;
  genders_node_t n;
  int rv = -1;

  __list_iterator_create(itr, handlecopy->nodeslist);
  while ((n = list_next(itr)))
    __hash_insert(handlecopy->node_index, n->name, n);
  
  rv = 0;
 cleanup:
  if (rv < 0)
    __list_destroy(l);
  __list_iterator_destroy(itr);
  return rv;
}

/* 
 * _genders_copy_attrvalslist
 *
 * Copy contents of the attrvalslist list into the handlecopy.
 *
 */
static int
_genders_copy_attrvalslist(genders_t handle, genders_t handlecopy)
{
  ListIterator attrvalslistitr = NULL;
  ListIterator attrvalsitr = NULL;
  genders_attrvals_container_t avc;
  genders_attrvals_container_t newavc = NULL;
  genders_attrval_t newav = NULL;
  int rv = -1;

  __list_iterator_create(attrvalslistitr, handle->attrvalslist);
  while ((avc = list_next(attrvalslistitr)))
    {
      genders_attrval_t av = NULL;

      __list_iterator_create(attrvalsitr, avc->attrvals);

      __xmalloc(newavc,
		genders_attrvals_container_t,
		sizeof(struct genders_attrvals_container));
      __list_create(newavc->attrvals, _genders_list_free_genders_attrval);
      newavc->index = avc->index;

      while ((av = list_next(attrvalsitr)))
	{
	  __xmalloc(newav, genders_attrval_t, sizeof(struct genders_attrval));
	  __xstrdup(newav->attr, av->attr);
	  if (av->val)
	    __xstrdup(newav->val, av->val);
	  else
	    newav->val = NULL;
	  newav->val_contains_subst = av->val_contains_subst;
	  __list_append(newavc->attrvals, newav);
	  newav = NULL;
	}
      
      __list_append(handlecopy->attrvalslist, newavc);
      newavc = NULL;
  }
  
  rv = 0;
 cleanup:
  if (rv < 0)
    {
      if (newav)
	{
	  free(newav->attr);
	  free(newav->val);
	  free(newav);
	}
      if (newavc)
	{
	  __list_destroy(newavc->attrvals);
	  free(newavc);
	}
    }
  __list_iterator_destroy(attrvalslistitr);
  __list_iterator_destroy(attrvalsitr);
  return rv;
}

/* 
 * _genders_copy_attrslist
 *
 * Copy contents of the attrslist list into the handlecopy.
 *
 */
static int
_genders_copy_attrslist(genders_t handle, genders_t handlecopy)
{
  ListIterator itr = NULL;
  char *newattr = NULL;
  char *attr;
  int rv = -1;

  __list_iterator_create(itr, handle->attrslist);
  while ((attr = list_next(itr)))
    {
      __xstrdup(newattr, attr);
      __list_append(handlecopy->attrslist, newattr);
      newattr = NULL;
    }
  
  rv = 0;
 cleanup:
  if (rv < 0)
    free(newattr);
  __list_iterator_destroy(itr);
  return rv;
}

/* 
 * _genders_copy_fill_attr_index
 *
 * Add keys into the attr_index
 *
 */
static int _genders_copy_fill_attr_index(genders_t handle, genders_t handlecopy)
{
  List l = NULL;
  ListIterator itr = NULL;
  char *attr;
  int rv = -1;

  __list_iterator_create(itr, handlecopy->attrslist);
  while ((attr = list_next(itr)))
    {
      __list_create(l, NULL);
      __hash_insert(handlecopy->attr_index, attr, l);
      l = NULL;
    }

  rv = 0;
 cleanup:
  if (rv < 0)
    __list_destroy(l);
  __list_iterator_destroy(itr);
  return rv;
}

/*
 * _genders_copy_find_attrvals_container
 *
 * Find the genders_attrvals_container pointer indicated by the index.
 */
static genders_attrvals_container_t
_genders_copy_find_attrvals_container(genders_t handle,
				      genders_t handlecopy,
				      unsigned int index)
{
  ListIterator attrvalsitr = NULL;
  genders_attrvals_container_t rv = NULL;
  genders_attrvals_container_t avc;
  
  __list_iterator_create(attrvalsitr, handlecopy->attrvalslist);
  while ((avc = list_next(attrvalsitr)))
    {
      if (avc->index == index)
	{
	  rv = avc;
	  goto cleanup;
	}
    }

 cleanup:
  __list_iterator_destroy(attrvalsitr);
  return rv;
}

/* 
 * _genders_copy_fill_node
 *
 * Fill node attrlist and attrlist index.
 *
 */
static int
_genders_copy_fill_node(genders_t handle,
			genders_t handlecopy,
			genders_node_t nodehandle,
			genders_node_t nodecopy)
{
  ListIterator attrlistitr = NULL;
  ListIterator attrvalsitr = NULL;
  genders_attrvals_container_t avc;
  int rv = -1;

  __list_iterator_create(attrlistitr, nodehandle->attrlist);
  while ((avc = list_next(attrlistitr)))
    {
      genders_attrvals_container_t tmpavc;
      genders_attrval_t av;

      if (!(tmpavc = _genders_copy_find_attrvals_container(handle,
							   handlecopy,
							   avc->index)))
	goto cleanup;
      
      __list_append(nodecopy->attrlist, tmpavc);
      
      __list_iterator_create(attrvalsitr, tmpavc->attrvals);
      while ((av = list_next(attrvalsitr)))
	{
	  List l;

	  __hash_insert(nodecopy->attrlist_index,
			av->attr,
			tmpavc);
	  
	  if (!(l = hash_find(handlecopy->attr_index, av->attr)))
	    {
	      handle->errnum = GENDERS_ERR_INTERNAL;
	      goto cleanup;
	    }
	  
	  __list_append(l, nodecopy);
	}
      
      __list_iterator_destroy(attrvalsitr);
      attrvalsitr = NULL;
    }
  
  rv = 0;
 cleanup:
  __list_iterator_destroy(attrlistitr);
  __list_iterator_destroy(attrvalsitr);
  return rv;
}

/* 
 * _genders_copy_fill_node_data
 *
 * Fill node attrlist and attrlist index.
 *
 */
static int
_genders_copy_fill_node_data(genders_t handle, genders_t handlecopy)
{
  ListIterator nodeslistitr = NULL;
  genders_node_t nodecopy;
  int rv = -1;

  __list_iterator_create(nodeslistitr, handlecopy->nodeslist);
  while ((nodecopy = list_next(nodeslistitr)))
    {
      genders_node_t nodehandle;

      if (!(nodehandle = hash_find(handle->node_index, nodecopy->name)))
	{
	  /* Shouldn't be possible to error here */
	  handle->errnum = GENDERS_ERR_INTERNAL;
	  goto cleanup;
	}

      if (_genders_copy_fill_node(handle,
				  handlecopy,
				  nodehandle,
				  nodecopy) < 0)
	goto cleanup;
    }
  
  rv = 0;
 cleanup:
  __list_iterator_destroy(nodeslistitr);
  return rv;
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
  handlecopy->maxattrs = handle->maxattrs;
  handlecopy->maxnodelen = handle->maxnodelen;
  handlecopy->maxattrlen = handle->maxattrlen;
  handlecopy->maxvallen = handle->maxvallen;

  memcpy(handlecopy->nodename, handle->nodename, GENDERS_MAXHOSTNAMELEN+1);
  
  if (_genders_copy_nodeslist(handle, handlecopy) < 0)
    goto cleanup;

  handlecopy->node_index_size = handle->node_index_size;

  __hash_create(handlecopy->node_index,
                handlecopy->node_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                NULL);

  if (_genders_copy_fill_node_index(handle, handlecopy) < 0)
    goto cleanup;

  if (_genders_copy_attrvalslist(handle, handlecopy) < 0)
    goto cleanup;

  if (_genders_copy_attrslist(handle, handlecopy) < 0)
    goto cleanup;

  handlecopy->attr_index_size = handle->attr_index_size;
  
  __hash_create(handlecopy->attr_index,
                handlecopy->attr_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                (hash_del_f)list_destroy);

  if (_genders_copy_fill_attr_index(handle, handlecopy) < 0)
    goto cleanup;
  
  if (_genders_copy_fill_node_data(handle, handlecopy) < 0)
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
