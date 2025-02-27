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
                hash_t *attrval_index,
                int *attrval_index_size,
                char *attr,
                char *val,
                int line_num,
                FILE *stream)
{
  genders_attrval_t av = NULL;
  int rv = -1;

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

  /* insert into attrs */

  if (hash_count((*attrval_index)) > ((*attrval_index_size) * 2))
    {
      if (_genders_rehash(handle, attrval_index, attrval_index_size) < 0)
        goto cleanup;
    }

  if (!hash_insert((*attrval_index), av->attr, av))
    {
      if (errno == EEXIST)
        {
          if (line_num > 0)
            {
              fprintf(stream,
                      "Line %d: duplicate attribute \"%s\" listed\n",
                      line_num,
                      av->attr);
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

  return 0;
 cleanup:
  if (av)
    {
      free(av->attr);
      free(av->val);
    }
  free(av);
  return rv;
}

/*
 * _insert_attr
 *
 * Insert an attr into the attrs hash
 *
 * Returns 1 if inserted an attr, 0 if not, -1 on error
 */
static int
_insert_attr(genders_t handle,
             hash_t *attrs,
             int *attrs_size,
             char *attr)
{
  char *attr_new = NULL;

  if (hash_find((*attrs), attr))
    return 0;

  /* insert into attrlist */
  __xstrdup(attr_new, attr);

  /* insert into attrs */
  if (hash_count((*attrs)) > ((*attrs_size) * 2))
    {
      if (_genders_rehash(handle, attrs, attrs_size) < 0)
        goto cleanup;
    }

  __hash_insert((*attrs), attr_new, attr_new);

  return 1;
 cleanup:
  free(attr_new);
  return -1;
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
 * _hostlist_intersect
 *
 * Returns 1 if hostlists intersect, 0 if not, -1 on error
 */
static int
_hostlist_intersect(genders_t handle, hostlist_t hl1, hostlist_t hl2)
{
  hostlist_iterator_t hlitr;
  hostlist_t hlfind;
  hostlist_t hl;
  char *node = NULL;
  int rv = -1;

  if (hostlist_count(hl1) > hostlist_count(hl2))
    {
      hl = hl2;
      hlfind = hl1;
    }
  else
    {
      hl = hl1;
      hlfind = hl2;
    }

  __hostlist_iterator_create(hlitr, hl);
  while ((node = hostlist_next(hlitr)))
    {
      if (hostlist_find(hlfind, node) >= 0)
        {
          rv = 1;
          goto cleanup;
        }
      free(node);
    }
  node = NULL;
  rv = 0;
 cleanup:
  __hostlist_iterator_destroy(hlitr);
  free(node);
  return rv;
}

struct attr_lookup
{
  hash_t attrval_index_lookup;
  int line_num;
  FILE *stream;
};

static int
_attr_lookup(void *data, const void *key, void *arg)
{
  struct attr_lookup *al = arg;

  if (hash_find(al->attrval_index_lookup, key))
    {
      if (al->line_num > 0)
        fprintf(al->stream,
                "Line %d: duplicate attribute \"%s\" listed\n",
                al->line_num,
                (char *)key);
      return 1;
    }
  return 0;
}

/*
 * _attrval_intersect
 *
 * Returns 1 if attrval has duplicate, 0 if not, -1 on error
 */
static int
_attrval_intersect(genders_t handle,
                   hash_t attrval_index1,
                   hash_t attrval_index2,
                   int line_num,
                   FILE *stream)
{
  struct attr_lookup al;
  hash_t attrval_index;
  hash_t attrval_index_lookup;
  int ret;

  if (hash_count(attrval_index1) > hash_count(attrval_index2))
    {
      attrval_index = attrval_index2;
      attrval_index_lookup = attrval_index1;
    }
  else
    {
      attrval_index = attrval_index1;
      attrval_index_lookup = attrval_index2;
    }

  al.attrval_index_lookup = attrval_index_lookup;
  al.line_num = line_num;
  al.stream = stream;
  if ((ret = hash_for_each(attrval_index,
                           _attr_lookup,
                           &al)) < 0)
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      return -1;
    }

  if (ret > 0)
    {
      handle->errnum = GENDERS_ERR_PARSE;
      return 1;
    }
  return 0;
}

/*
 * _duplicate_attr_checks
 *
 * Check if any prior lines have duplicate attributes
 * for a node.
 *
 * If line_num > 0, returns 1 if a duplicate exists, 0 if not, -1 on error
 *
 * If line_num == 0, returns 0 on success, -1 on error
 */
static int
_duplicate_attr_checks(genders_t handle,
                       List genders_hosts,
                       hostlist_t hl,
                       hash_t attrval_index,
                       int line_num,
                       FILE *stream)
{
  ListIterator itr = NULL;
  genders_hosts_t hosts = NULL;
  int rv = -1;

  __list_iterator_create(itr, genders_hosts);
  while ((hosts = list_next(itr)))
    {
      int ret;
      if ((ret = _hostlist_intersect(handle, hosts->hl, hl)) < 0)
          goto cleanup;

      /* hostlists intersect, gotta check attrs */
      if (ret)
        {
          if ((ret = _attrval_intersect(handle,
                                        hosts->attrval_index,
                                        attrval_index,
                                        line_num,
                                        stream)) < 0)
            goto cleanup;

          if (ret > 0)
            {
              if (line_num > 0)
                rv = 1;
              goto cleanup;
            }
        }
    }

  rv = 0;
 cleanup:
  __list_iterator_destroy(itr);
  return rv;
}

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
            int *maxnodelen,
            int *maxattrlen,
            int *maxvallen,
            List genders_hosts,
            hostlist_t hostnames,
            hash_t *attrs,
            int *attrs_size,
            char *line,
            int line_num,
            FILE *stream)
{
  char *temp, *nodenames, *node = NULL;
  int max_n_subst_vallen = 0, line_maxnodelen = 0, rv = -1;
  genders_hosts_t hosts = NULL;
  hash_t attrval_index = NULL;
  int attrval_index_size = GENDERS_ATTR_INDEX_INIT_SIZE;
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

  __hash_create(attrval_index,
                attrval_index_size,
                (hash_key_f)hash_key_string,
                (hash_cmp_f)strcmp,
                (hash_del_f)_genders_hash_free_genders_attrval);

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

              if ((ret = _insert_attrval(handle,
                                         &attrval_index,
                                         &attrval_index_size,
                                         attr,
                                         val,
                                         line_num,
                                         stream)) != 0)
                {
                  rv = ret;
                  goto cleanup;
                }

              if ((insert_count = _insert_attr(handle,
                                               attrs,
                                               attrs_size,
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

  if ((ret = _duplicate_attr_checks(handle,
                                    genders_hosts,
                                    hl,
                                    attrval_index,
                                    line_num,
                                    stream)) != 0)
    {
      rv = ret;
      goto cleanup;
    }

  __hostlist_iterator_create(hlitr, hl);

  while ((node = hostlist_next(hlitr)))
    {
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

      if (!line_num)
        {
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

  if (!hostlist_push(hostnames, nodenames))
    {
      /* assume internal error if above hostlist_push() call worked */
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  __xmalloc(hosts, genders_hosts_t, sizeof(struct genders_hosts));
  hosts->hl = hl;
  hosts->attrval_index = attrval_index;
  hosts->attrval_index_size = attrval_index_size;
  __list_append(genders_hosts, hosts);
  hl = NULL;
  attrval_index = NULL;
  hosts = NULL;

  rv = 0;
 cleanup:
  __hostlist_iterator_destroy(hlitr);
  __hostlist_destroy(hl);
  __hash_destroy(attrval_index);
  free(hosts);
  free(node);
  return rv;
}

int
_genders_open_and_parse(genders_t handle,
                        const char *filename,
                        int *numattrs,
                        int *maxnodelen,
                        int *maxattrlen,
                        int *maxvallen,
                        List genders_hosts,
                        hostlist_t hostnames,
                        hash_t *attrs,
                        int *attrs_size,
                        int debug,
                        FILE *stream)
{
  int len = 0, errcount = 0, rv = -1, line_count = 1;
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
                                   maxnodelen,
                                   maxattrlen,
                                   maxvallen,
                                   genders_hosts,
                                   hostnames,
                                   attrs,
                                   attrs_size,
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
