/*****************************************************************************\
 *  $Id: genders_parsing.c,v 1.4 2004-12-22 18:10:13 achu Exp $
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "genders.h"
#include "genders_common.h"
#include "fd.h"
#include "list.h"
#include "hash.h"
#include "hostlist.h"

int 
_readline(genders_t handle, int fd, char *buf, int buflen) 
{
  int retval; 

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(buf && buflen > 0);

  if ((retval = fd_read_line(fd, buf, buflen)) < 0) {
    handle->errnum = GENDERS_ERR_READ;
    return -1;
  }
  
  /* buflen - 1 b/c fd_read_line guarantees null termination */
  if (retval >= (buflen-1)) {
    handle->errnum = GENDERS_ERR_PARSE;
    return -1;
  }

  return retval;
}

static genders_node_t
_insert_node(genders_t handle, List nodelist, char *nodename)
{
  genders_node_t n = NULL;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(nodelist && nodename);

  /* must create node if node doesn't exist in the nodelist */ 
  if (!(n = list_find_first(nodelist, _is_node, nodename))) {
    __xmalloc(n, genders_node_t, sizeof(struct genders_node));
    __xstrdup(n->name, nodename);
    n->attrcount = 0;

    __list_create(n->attrlist, NULL);
    __list_append(nodelist, n);
  }
  return n;

 cleanup:
  if (n) { 
    __list_destroy(n->attrlist);
    free(n);
  }
  return NULL;
}

static int 
_insert_attrval(genders_t handle, List attrvals, char *attr, char *val) 
{
  genders_attrval_t av = NULL;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(attrvals && attr);

  __xmalloc(av, genders_attrval_t, sizeof(struct genders_attrval));
  av->val_contains_subst = 0;

  __xstrdup(av->attr, attr);
  if (val) {
    __xstrdup(av->val, val);
    if (strstr(av->val, "%n") || strstr(av->val, "%%"))
      av->val_contains_subst = 1;
  }
  else 
    av->val = NULL; 

  __list_append(attrvals, av);

  return 0;
 cleanup:
  if (av) {
    free(av->attr);
    free(av->val);
  }
  free(av);
  return -1;
}

static int 
_insert_ptr(genders_t handle, List ptrlist, void *ptr) 
{
  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(ptrlist && ptr);

  __list_append(ptrlist, ptr);
  return 0;
 cleanup:
  return -1;
}

static int 
_insert_attr(genders_t handle, char *attr) 
{
  char *attr_new = NULL;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(attr);

  if (list_find_first(handle->attrslist, _is_str, attr))
    return 0;

  __xstrdup(attr_new, attr);
  __list_append(handle->attrslist, attr_new);
  return 0;
 cleanup:
  free(attr_new);
  return -1;
}

static int
_whitespace_check(genders_t handle, 
                  char *line, 
                  int line_num,
                  FILE *stream)
{
  int retval = -1;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(line);
  assert(!line_num || (line_num && stream));

  if (strchr(line,' ') || strchr(line,'\t')) {
    if (line_num > 0) {
      fprintf(stream, "Line %d: white space in attribute list\n", line_num);
      retval = 1;
    }
    handle->errnum = GENDERS_ERR_PARSE;
    goto cleanup;
  }

  retval = 0;
 cleanup:
  return retval;
}

static int
_duplicate_line_attr_check(genders_t handle, 
                           char *attr, 
                           List attrvals,
                           int line_num,
                           FILE *stream)
{
  int retval = -1;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(attr && attrvals);
  assert(!line_num || (line_num && stream));

  if (list_find_first(attrvals, _is_attr_in_attrvals, attr) != NULL) {
    if (line_num > 0) {
      fprintf(stream, "Line %d: duplicate attribute \"%s\" listed\n",
              line_num, attr);
      retval = 1;
    }
    handle->errnum = GENDERS_ERR_PARSE;
    goto cleanup;
  }

  retval = 0;
 cleanup:
  return retval;
}

static int
_nodename_check(genders_t handle, 
                char *nodename, 
                int line_num,
                FILE *stream)
{
  int retval = -1;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(nodename);
  assert(!line_num || (line_num && stream));

  if (strlen(nodename) > MAXHOSTNAMELEN) {
    if (line_num > 0) {
      fprintf(stream, "Line %d: hostname too long\n", line_num);
      retval = 1;
    }
    handle->errnum = GENDERS_ERR_PARSE;
    goto cleanup;
  }
    
  if (strchr(nodename, '.')) {
    if (line_num > 0) {
      fprintf(stream, "Line %d: node not a shortened hostname\n", line_num);
      retval = 1;
    }
    handle->errnum = GENDERS_ERR_PARSE;
    goto cleanup;
  }

  retval = 0;
 cleanup:
  return retval;
}

static int
_duplicate_node_attr_check(genders_t handle, 
                           genders_node_t n, 
                           List attrvals,
                           int line_num,
                           FILE *stream)
{
  ListIterator attrvals_itr = NULL;
  genders_attrval_t av = NULL;
  genders_attrval_t av_ret = NULL;
  int retval = -1;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(n && attrvals);
  assert(!line_num || (line_num && stream));

  __list_iterator_create(attrvals_itr, attrvals);
  while ((av = list_next(attrvals_itr))) {
    if (_find_attrval(handle, n, av->attr, NULL, &av_ret) < 0)
      goto cleanup;
    if (av_ret) {
      if (line_num > 0) {
        fprintf(stream, "Line %d: duplicate attribute listed for node \"%s\"\n",
                line_num, n->name);
        retval = 1;
      }
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }
  }

  retval = 0;
 cleanup:
  __list_iterator_destroy(attrvals_itr);
  return retval;
}

/* parse a genders file line
 * - If line_num == 0, parse and store genders data
 * - If line_num > 0, debug genders file
 * - Returns -1 on error, 1 if there was a parse error, 0 if no errors
 */
int 
_parse_line(genders_t handle, List nodeslist, List attrvalslist, 
            char *line, int line_num, FILE *stream)
{
  char *linebuf, *temp, *attr, *nodenames, *node = NULL;
  int retval = -1;
  int max_n_subst_vallen = 0;
  int line_maxnodelen = 0;
  List attrvals = NULL;
  genders_node_t n;
  hostlist_t hl = NULL;
  hostlist_iterator_t hlitr = NULL;

  assert(handle && handle->magic == GENDERS_MAGIC_NUM);
  assert(nodeslist && attrvalslist && line);
  assert((!line_num) || (line_num > 0 && stream));

  /* "remove" comments */
  if ((temp = strchr(line, '#'))) 
    *temp = '\0';

  /* remove trailing white space, including newline */
  temp = line + strlen(line);
  for (--temp; temp >= line; temp--) {
    if (isspace(*temp))
      *temp = '\0';
    else
      break;
  }

  /* empty line */
  if (*line == '\0')
    return 0;

  /* move forward to node name(s) */
  while(isspace(*line))  
    line++;

  /* get node name(s) */
  if (!(nodenames = strsep(&line, " \t\0")))
    return 0;

  /* if strsep() sets line == NULL, line has no attributes */
  if (line) {
    /* move forward to attributes */
    while(isspace(*line))  
      line++;

    /* *line == '\0' means line has no attributes */
    if (*line != '\0') {
      if ((retval = _whitespace_check(handle, line, line_num, stream)) != 0)
        goto cleanup;
      
      __list_create(attrvals, _free_genders_attrval);

      /* parse attributes */
      attr = strtok_r(line,",\0",&linebuf);
      while (attr) {
        char *val = NULL;
      
        /* parse value out of attribute */
        if ((val = strchr(attr,'=')))
          *val++ = '\0'; 

        if ((retval = _duplicate_line_attr_check(handle, attr, attrvals,
                                                 line_num, stream)) != 0)
          goto cleanup;

        if (_insert_attrval(handle, attrvals, attr, val) < 0)
          goto cleanup;
        
        if (!line_num) { 
          if (_insert_attr(handle, attr) < 0)
            goto cleanup;
          
          handle->numattrs++;
          handle->maxattrlen = MAX(strlen(attr), handle->maxattrlen);
          if (val) {
            if (strstr(val, "%n"))
              max_n_subst_vallen = strlen(val);
            else
              handle->maxvallen = MAX(strlen(val), handle->maxvallen);
          }
        }
        
        attr = strtok_r(NULL,",\0",&linebuf);
      }
    }
  }

  __hostlist_create(hl, nodenames);
  __hostlist_iterator_create(hlitr, hl);
  while ((node = hostlist_next(hlitr))) {
    if ((retval = _nodename_check(handle, node, line_num, stream)) != 0)
      goto cleanup;
    
    if (!(n =_insert_node(handle, nodeslist, node)))
      goto cleanup;
    
    if (attrvals) {
      if ((retval = _duplicate_node_attr_check(handle, n, attrvals, 
                                               line_num, stream)) != 0)
        goto cleanup;
      
      if (_insert_ptr(handle, n->attrlist, attrvals) < 0)
        goto cleanup;
      
      n->attrcount += list_count(attrvals);
    }

    if (!line_num) {
      handle->maxattrs = MAX(n->attrcount, handle->maxattrs);
      handle->maxnodelen = MAX(strlen(node), handle->maxnodelen);
      line_maxnodelen = MAX(strlen(node), line_maxnodelen);
    }
    
    free(node);
  }
  node = NULL;

  /* %n substitution found on this line, update maxvallen */
  if (!line_num && max_n_subst_vallen)
    handle->maxvallen = MAX(max_n_subst_vallen - 2 + line_maxnodelen,
                            handle->maxvallen);

  /* Append at the very end, so cleanup area cleaner */
  if (attrvals) {
    if (_insert_ptr(handle, attrvalslist, attrvals) < 0)
      goto cleanup;
    attrvals = NULL;
  }

  retval = 0;
 cleanup:
  __hostlist_iterator_destroy(hlitr);
  __hostlist_destroy(hl);
  __list_destroy(attrvals);
  free(node);
  return retval;
}

int
_index_nodes(genders_t handle)
{
  genders_node_t n;
  ListIterator nodeslist_itr = NULL;

  __hash_create(handle->node_index, 
                handle->numnodes,
                (hash_key_f)hash_key_string, 
                (hash_cmp_f)strcmp, 
                NULL);

  __list_iterator_create(nodeslist_itr, handle->nodeslist);

  while ((n = list_next(nodeslist_itr)))
    __hash_insert(handle->node_index, n->name, n);

  return 0;
 cleanup:
  __list_iterator_destroy(nodeslist_itr);
  __hash_destroy(handle->node_index);
  handle->node_index = NULL;
  return -1;
}

int
_index_attrs(genders_t handle)
{
  ListIterator attrslist_itr = NULL;
  ListIterator nodeslist_itr = NULL;
  List l = NULL;
  genders_node_t n;
  genders_attrval_t av;
  char *attr;

  __hash_create(handle->attr_index, 
                handle->numattrs,
                (hash_key_f)hash_key_string, 
                (hash_cmp_f)strcmp, 
                (hash_del_f)list_destroy);
  __list_iterator_create(attrslist_itr, handle->attrslist);
  __list_iterator_create(nodeslist_itr, handle->nodeslist);

  while ((attr = list_next(attrslist_itr))) {
    __list_create(l, NULL);

    while ((n = list_next(nodeslist_itr))) {
      if (_find_attrval(handle, n, attr, NULL, &av) < 0)
        goto cleanup;
      if (av)
        __list_append(l, n);
    }
    list_iterator_reset(nodeslist_itr);

    __hash_insert(handle->attr_index, attr, l);
    l = NULL;
  }

  __list_iterator_destroy(nodeslist_itr);
  __list_iterator_destroy(attrslist_itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;

 cleanup:
  __list_iterator_destroy(attrslist_itr);
  __list_iterator_destroy(nodeslist_itr);
  __list_destroy(l);
  __hash_destroy(handle->attr_index);
  handle->attr_index = NULL;
  return -1;
}
