/*****************************************************************************\
 *  $Id: genders_parsing.c,v 1.37 2010-02-02 00:04:34 chu11 Exp $
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
#include <ctype.h>
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */

#include "genders.h"
#include "genders_api.h"
#include "genders_constants.h"
#include "genders_util.h"
#include "hash.h"
#include "hostlist.h"
#include "list.h"

/*
 * _insert_node
 *
 * Insert a node into the nodelist and node index.
 *
 * Returns 0 on success, -1 on error
 */
static genders_node_t
_insert_node(genders_t handle,
             List nodelist,
             hash_t *node_index,
             int *node_index_size,
             char *nodename)
{
  genders_node_t n = NULL;

  /* must create node if node doesn't exist */
  if (!(n = hash_find((*node_index), nodename)))
    {
      /* insert into nodelist */
      __xmalloc(n, genders_node_t, sizeof(struct genders_node));
      __xstrdup(n->name, nodename);
      __list_create(n->attrlist, NULL);
      n->attrcount = 0;
      n->attrlist_index_size = GENDERS_ATTRLIST_INDEX_INIT_SIZE;
      __hash_create(n->attrlist_index,
                    n->attrlist_index_size,
                    (hash_key_f)hash_key_string,
                    (hash_cmp_f)strcmp,
                    NULL);

      __list_append(nodelist, n);

      /* insert into node_index */

      if (hash_count((*node_index)) > ((*node_index_size) * 2))
        {
          if (_genders_rehash(handle, node_index, node_index_size) < 0)
            goto cleanup;
        }

      __hash_insert((*node_index), n->name, n);
    }
  return n;

 cleanup:
  if (n)
    {
      __list_destroy(n->attrlist);
      __hash_destroy(n->attrlist_index);
      free(n);
    }
  return NULL;
}

/*
 * _insert_attrval
 *
 * Insert an attrval into the attrvallist.
 *
 * If line_num > 0, returns 1 if a duplicate exists, 0 if not, -1 on error
 *
 * If line_num == 0, returns 0 on success, -1 on error
 */
static int
_insert_attrval(genders_t handle,
                genders_attrvals_container_t avc,
                char *attr,
                char *val,
                int line_num,
                FILE *stream)
{
  genders_attrval_t av = NULL;

  /* Check attribute already listed for this node and on same line */

  if (list_find_first(avc->attrvals, _genders_list_is_attr_in_attrvals, attr))
    {
      int rv = -1;
      if (line_num > 0)
        {
          fprintf(stream, "Line %d: duplicate attribute \"%s\" listed\n",
                  line_num, attr);
          rv = 1;
        }
      handle->errnum = GENDERS_ERR_PARSE;
      return rv;
    }

  __xmalloc(av, genders_attrval_t, sizeof(struct genders_attrval));
  av->val_contains_subst = 0;

  __xstrdup(av->attr, attr);
  if (val)
    {
      __xstrdup(av->val, val);
      if (strstr(av->val, "%n") || strstr(av->val, "%%"))
        av->val_contains_subst = 1;
    }
  else
    av->val = NULL;

  __list_append(avc->attrvals, av);

  return 0;
 cleanup:
  if (av)
    {
      free(av->attr);
      free(av->val);
    }
  free(av);
  return -1;
}

/*
 * _insert_attr
 *
 * Insert an attr into the attrlist and attr_index
 *
 * Returns 0 on success, -1 on error
 */
static int
_insert_attr(genders_t handle,
             List attrslist,
             hash_t *attr_index,
             int *attr_index_size,
             char *attr)
{
  List l = NULL;
  char *attr_new = NULL;

  if (hash_find((*attr_index), attr))
    return 0;

  /* insert into attrlist */
  __xstrdup(attr_new, attr);
  __list_append(attrslist, attr_new);

  /* insert into attr_index */

  if (hash_count((*attr_index)) > ((*attr_index_size) * 2))
    {
      if (_genders_rehash(handle, attr_index, attr_index_size) < 0)
        goto cleanup;
    }

  __list_create(l, NULL);
  __hash_insert((*attr_index), attr_new, l);

  return 1;
 cleanup:
  __list_destroy(l);
  free(attr_new);
  return -1;
}

/*
 * _attr_node_processing
 *
 * Determine if an attr in the attrvallist already exists for the
 * node.  If not insert into the attr_index and node attrlist_index.
 *
 * If line_num > 0, returns 1 if a duplicate exists, 0 if not, -1 on error
 *
 * If line_num == 0, returns 0 on success, -1 on error
 */
static int
_attr_node_processing(genders_t handle,
                      genders_node_t n,
                      genders_attrvals_container_t avc,
                      hash_t *attr_index,
                      int line_num,
                      FILE *stream)
{
  ListIterator attrvals_itr = NULL;
  genders_attrval_t av = NULL;
  int rv = -1;

  __list_iterator_create(attrvals_itr, avc->attrvals);
  while ((av = list_next(attrvals_itr)))
    {
      List l = NULL;

      /* add node to attr_index */
      /* attribute should already be in the hash */
      if (!(l = hash_find((*attr_index), av->attr)))
        {
          handle->errnum = GENDERS_ERR_INTERNAL;
          goto cleanup;
        }
      __list_append(l, n);

      /* add attr to attrlist_index */
      if (hash_count(n->attrlist_index) > (n->attrlist_index_size * 2))
        {
          if (_genders_rehash(handle, &(n->attrlist_index), &(n->attrlist_index_size)) < 0)
            goto cleanup;
        }

      if (!hash_insert(n->attrlist_index, av->attr, avc))
        {
          if (errno == EEXIST)
            {
              if (line_num > 0)
                {
                  fprintf(stream,
                          "Line %d: duplicate attribute \"%s\" listed for node \"%s\"\n",
                          line_num,
                          av->attr,
                          n->name);
                  rv = 1;
                }
              handle->errnum = GENDERS_ERR_PARSE;
              goto cleanup;
            }
          else if (errno == ENOMEM)
            handle->errnum = GENDERS_ERR_OUTMEM;
          else
            handle->errnum = GENDERS_ERR_INTERNAL;
          goto cleanup;
        }
    }

  rv = 0;
 cleanup:
  __list_iterator_destroy(attrvals_itr);
  return rv;
}

#ifndef HAVE_STRSEP
/*
 * strsep for those systems that do not define it.
 *
 * Taken straight from glibc
 */
static char *
strsep (char **stringp, const char *delim)
{
  char *begin, *end;

  begin = *stringp;
  if (begin == NULL)
    return NULL;

  /* A frequent case is when the delimiter string contains only one
     character.  Here we don't need to call the expensive `strpbrk'
     function and instead work using `strchr'.  */
  if (delim[0] == '\0' || delim[1] == '\0')
    {
      char ch = delim[0];

      if (ch == '\0')
        end = NULL;
      else
        {
          if (*begin == ch)
            end = begin;
          else if (*begin == '\0')
            end = NULL;
          else
            end = strchr (begin + 1, ch);
        }
    }
  else
    /* Find the end of the token.  */
    end = strpbrk (begin, delim);

  if (end)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = '\0';
      *stringp = end;
    }
  else
    /* No more delimiters; this is the last token.  */
    *stringp = NULL;

  return begin;
}
#endif /* HAVE_STRSEP */

/*
 * _parse_line
 *
 * parse a genders file line
 * - If line_num == 0, parse and store genders data
 * - If line_num > 0, debug genders file
 *
 * Returns -1 on error, 1 if there was a parse error, 0 if no errors
 */
static int
_parse_line(genders_t handle,
            int *numattrs,
            int *maxattrs,
            int *maxnodelen,
            int *maxattrlen,
            int *maxvallen,
            List nodeslist,
            List attrvalslist,
            List attrslist,
            hash_t *node_index,
            int *node_index_size,
            hash_t *attr_index,
            int *attr_index_size,
            char *line,
            int line_num,
            FILE *stream)
{
  char *temp, *nodenames, *node = NULL;
  int max_n_subst_vallen = 0, line_maxnodelen = 0, rv = -1;
  genders_attrvals_container_t avc = NULL;
  hostlist_t hl = NULL;
  hostlist_iterator_t hlitr = NULL;
  int ret;

  /* "remove" comments */
  if ((temp = strchr(line, '#')))
    *temp = '\0';

  /* remove trailing white space, including newline */
  temp = line + strlen(line);
  for (--temp; temp >= line; temp--)
    {
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
  if (line)
    {
      char *attr;

      /* move forward to attributes */
      while(isspace(*line))
        line++;

      /* *line == '\0' means line has no attributes */
      if (*line != '\0')
        {
          int insert_count;

          if (strchr(line,' ') || strchr(line,'\t'))
            {
              if (line_num > 0)
                {
                  fprintf(stream, "Line %d: white space in attribute list\n", line_num);
                  rv = 1;
                }
              handle->errnum = GENDERS_ERR_PARSE;
              goto cleanup;
            }

          __xmalloc(avc,
                    genders_attrvals_container_t,
                    sizeof(struct genders_attrvals_container));
          __list_create(avc->attrvals, _genders_list_free_genders_attrval);
          avc->index = list_count(handle->attrvalslist);

          /* parse attributes */
          attr = strsep(&line, ",");
          while (attr)
            {
              char *val = NULL;

              /* parse value out of attribute */
              if ((val = strchr(attr,'=')))
                *val++ = '\0';

#if 0
              /* Remove this check, we will leave this as a "feature" */
              if (val && strchr(val,'='))
                {
                  if (line_num > 0)
                    {
                      fprintf(stream, "Line %d: value contains equal sign\n", line_num);
                      rv = 1;
                    }
                  handle->errnum = GENDERS_ERR_PARSE;
                  goto cleanup;
                }
#endif

              if (!strlen(attr))
                {
                  if (line_num > 0)
                    {
                      fprintf(stream, "Line %d: empty string attribute listed\n", line_num);
                      rv = 1;
                    }
                  handle->errnum = GENDERS_ERR_PARSE;
                  goto cleanup;
                }

              if (val && !strlen(val))
                {
                  if (line_num > 0)
                    {
                      fprintf(stream, "Line %d: no value specified for attribute \"%s\"\n",
                              line_num, attr);
                      rv = 1;
                    }
                  handle->errnum = GENDERS_ERR_PARSE;
                  goto cleanup;
                }

              if ((ret = _insert_attrval(handle, avc, attr, val, line_num, stream)) != 0)
                {
                  rv = ret;
                  goto cleanup;
                }

              if ((insert_count = _insert_attr(handle,
                                               attrslist,
                                               attr_index,
                                               attr_index_size,
                                               attr)) < 0)
                goto cleanup;

              if (!line_num)
                {
                  (*numattrs) += insert_count;
                  (*maxattrlen) = GENDERS_MAX(strlen(attr), (*maxattrlen));

                  if (val)
                    {
                      if (strstr(val, "%n") && !strstr(val, "%%n"))
                        max_n_subst_vallen = strlen(val);
                      else
                        (*maxvallen) = GENDERS_MAX(strlen(val), (*maxvallen));
                    }
                }

              attr = strsep(&line, ",");
            }
        }
    }

#ifndef WITH_NON_SHORTENED_HOSTNAMES
  if (strchr(nodenames, '.'))
    {
      if (line_num > 0)
        {
          fprintf(stream, "Line %d: node not a shortened hostname\n", line_num);
          rv = 1;
        }
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }
#endif /* !WITH_NON_SHORTENED_HOSTNAMES */

  __hostlist_create(hl, NULL);

  if (!hostlist_push(hl, nodenames))
    {
      if (line_num > 0)
        {
          fprintf(stream, "Line %d: incorrectly specified nodename(s)\n", line_num);
          rv = 1;
        }
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }

  __hostlist_iterator_create(hlitr, hl);

  while ((node = hostlist_next(hlitr)))
    {
      genders_node_t n;

      if (strlen(node) > GENDERS_MAXHOSTNAMELEN)
        {
          if (line_num > 0)
            {
              fprintf(stream, "Line %d: hostname too long\n", line_num);
              rv = 1;
            }
          handle->errnum = GENDERS_ERR_PARSE;
          goto cleanup;
        }

      if (!(n = _insert_node(handle,
                             nodeslist,
                             node_index,
                             node_index_size,
                             node)))
        goto cleanup;

      if (avc)
        {
          if ((ret = _attr_node_processing(handle,
                                           n,
                                           avc,
                                           attr_index,
                                           line_num,
                                           stream)) != 0)
            {
              rv = ret;
              goto cleanup;
            }

          __list_append(n->attrlist, avc);
          n->attrcount += list_count(avc->attrvals);
        }

      if (!line_num)
        {
          (*maxattrs) = GENDERS_MAX(n->attrcount, (*maxattrs));
          (*maxnodelen) = GENDERS_MAX(strlen(node), (*maxnodelen));
          line_maxnodelen = GENDERS_MAX(strlen(node), line_maxnodelen);
        }

      free(node);
    }
  node = NULL;

  /* %n substitution found on this line, update maxvallen */
  if (!line_num && max_n_subst_vallen)
    (*maxvallen) = GENDERS_MAX(max_n_subst_vallen - 2 + line_maxnodelen,
                               (*maxvallen));

  /* Append at the very end, so cleanup area cleaner */
  if (avc)
    {
      __list_append(attrvalslist, avc);
      avc = NULL;
    }

  rv = 0;
 cleanup:
  __hostlist_iterator_destroy(hlitr);
  __hostlist_destroy(hl);
  if (avc)
    {
      __list_destroy(avc->attrvals);
      free(avc);
    }
  free(node);
  return rv;
}

int
_genders_open_and_parse(genders_t handle,
                        const char *filename,
                        int *numattrs,
                        int *maxattrs,
                        int *maxnodelen,
                        int *maxattrlen,
                        int *maxvallen,
                        List nodeslist,
                        List attrvalslist,
                        List attrslist,
                        hash_t *node_index,
                        int *node_index_size,
                        hash_t *attr_index,
                        int *attr_index_size,
                        int debug,
                        FILE *stream)
{
  int len, errcount = 0, rv = -1, line_count = 1;
  char buf[GENDERS_BUFLEN];

  if (!filename || !strlen(filename))
    filename = GENDERS_DEFAULT_FILE;

  FILE *f = fopen(filename, "r");
  if (!f)
    {
      handle->errnum = GENDERS_ERR_OPEN;
      goto cleanup;
    }

  /* parse line by line */
  while (fgets(buf, GENDERS_BUFLEN, f))
    {
      int bug_count;

      len = strlen(buf);

      /* -1 for NUL char */
      if (len >= (GENDERS_BUFLEN - 1))
        {
          len = -1;
          handle->errnum = GENDERS_ERR_PARSE;
          break;
        }

      if ((bug_count = _parse_line(handle,
                                   numattrs,
                                   maxattrs,
                                   maxnodelen,
                                   maxattrlen,
                                   maxvallen,
                                   nodeslist,
                                   attrvalslist,
                                   attrslist,
                                   node_index,
                                   node_index_size,
                                   attr_index,
                                   attr_index_size,
                                   buf,
                                   (debug) ? line_count : 0,
                                   stream)) < 0)
        goto cleanup;

      if (debug)
        {
          if (bug_count)
            errcount++;
          line_count++;
        }
    }

  if (!feof(f))
    {
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }

  if (len < 0)
    {
      if (debug && handle->errnum == GENDERS_ERR_OVERFLOW)
        {
          fprintf(stream, "Line %d: exceeds maximum allowed length\n", line_count);
          rv = ++errcount;
          handle->errnum = GENDERS_ERR_PARSE;
        }
      goto cleanup;
    }

  rv = (debug) ? errcount : 0;
 cleanup:
  /* ignore potential error, just return results */
  if (f)
    fclose(f);

  return rv;
}
