%{
/*****************************************************************************\
 *  $Id: genders_query.y,v 1.1 2004-06-07 22:11:30 achu Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "genders.h"
#include "genders_common.h"

struct genders_treenode {
  char *str;
  struct genders_treenode *left;
  struct genders_treenode *right;
};

int genders_query_err; 
struct genders_treenode *genders_treeroot;

#define HOSTLIST_BUFLEN 32768

struct genders_treenode *
genders_makenode(char *str, void *left, void *right)
{
  struct genders_treenode *t; 

  if (genders_query_err)
    return NULL;

  if (!str)
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      return NULL;
    }

  if (!(t = (struct genders_treenode *)malloc(sizeof(struct genders_treenode))))
    {
      genders_query_err = GENDERS_ERR_OUTMEM;
      return NULL;
    }

  if (!(t->str = (char *)strdup(str)))
    {
      genders_query_err = GENDERS_ERR_OUTMEM;
      return NULL;
    }

  t->left = left;
  t->right = right;
  return t;
} 

void
genders_printtree(struct genders_treenode *t)
{
  if (!t)
    return;
  genders_printtree(t->left);
  printf("%s\n", t->str);
  genders_printtree(t->right);
}

void
genders_freetree(struct genders_treenode *t)
{
  if (!t)
    return;
  genders_freetree(t->left);
  genders_freetree(t->right);
  free(t->str);
  free(t);
  return;
}

void yyerror(const char *str) 
{
  if (!genders_query_err)
    genders_query_err = GENDERS_ERR_PARSE;
}
  
int yywrap()
{
  return 1;
}

int 
_parse_query(genders_t handle, char *query)
{
  extern FILE *yyin, *yyout; 
  int fds[2];

  genders_query_err = 0;
  genders_treeroot = NULL;

  /* yyparse() only works with FILE streams, not fds, so we gotta
   * do some funky hacking here to make it work.
   */

  if (pipe(fds) < 0)
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  if (write(fds[1], query, strlen(query)) < 0)
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }
  close(fds[1]);

  if (!(yyin = fdopen(fds[0], "r")))
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  /* disable yacc output to stdout */
  if (!(yyout = fopen("/dev/null", "r+")))
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  yyparse();

  /* This can happen if the user passes in all whitespace for
   * example 
   */
  if (!genders_treeroot && !genders_query_err)
    genders_query_err = GENDERS_ERR_PARSE;

 cleanup:
  /* no need to close fds[0], fclose is enough */
  fclose(yyin);
  fclose(yyout);
  
  if (genders_query_err)
    {
      handle->errnum = genders_query_err;
      return -1;
    }
  return 0;
}

int
_check_attrs(genders_t handle, struct genders_treenode *t)
{
  if (!t->left && !t->right)
    {
      int rv;
      if ((rv = genders_isattr(handle, t->str)) < 0)
        return -1;
      if (rv == 0)
        {
          handle->errnum = GENDERS_ERR_NOTFOUND;
          return -1;
        }
    }
  else
    {
      if (t->left)
        {
          if (_check_attrs(handle, t->left) < 0)
            return -1;
        }
      if (t->right)
        {
          if (_check_attrs(handle, t->right) < 0)
            return -1;
        }
    }
  return 0;
}

hostlist_t
_calc_query(genders_t handle, struct genders_treenode *t)
{
  if (!t->left && !t->right)
    {
      char **nodes;
      hostlist_t h;
      int i, len, num;

      if ((len = genders_nodelist_create(handle, &nodes)) < 0)
        return NULL;

      if ((num = genders_getnodes(handle, nodes, len, t->str, NULL)) < 0)
        return NULL;

      if (!(h = hostlist_create(NULL)))
        {
          handle->errnum = GENDERS_ERR_OUTMEM;
          genders_nodelist_destroy(handle, nodes);
          return NULL;
        }

      for (i = 0; i < num; i++)
        {
          if (hostlist_push(h, nodes[i]) == 0)
            {
              handle->errnum = GENDERS_ERR_INTERNAL;
              genders_nodelist_destroy(handle, nodes);
              hostlist_destroy(h);
              return NULL;
            }
        }

      genders_nodelist_destroy(handle, nodes);
      hostlist_sort(h);
      return h;
    }
  else
    {
      hostlist_t h, l, r;
      char buf[HOSTLIST_BUFLEN];
      int i, rv, len, num;

      h = l = r = NULL;

      if (t->left)
        {
          if (!(l = _calc_query(handle, t->left)))
            return NULL;
        }
      if (t->right)
        {
          if (!(r = _calc_query(handle, t->right)))
            {
              hostlist_destroy(l);
              return NULL;
            }
        }

      if (!(h = hostlist_create(NULL)))
        {
          handle->errnum = GENDERS_ERR_OUTMEM;
          hostlist_destroy(l);
          hostlist_destroy(r);
          return NULL;
        }
      
      if (strcmp(t->str, "+") == 0)
        {
          memset(buf, '\0', HOSTLIST_BUFLEN);
          if ((rv = hostlist_ranged_string(l, HOSTLIST_BUFLEN, buf)) < 0)
            {
              handle->errnum = GENDERS_ERR_INTERNAL;
              hostlist_destroy(l);
              hostlist_destroy(r);
              hostlist_destroy(h);
              return NULL;
            }

          if (rv > 0)
            hostlist_push(h, buf);

          memset(buf, '\0', HOSTLIST_BUFLEN);
          if ((rv = hostlist_ranged_string(r, HOSTLIST_BUFLEN, buf)) < 0)
            {
              handle->errnum = GENDERS_ERR_INTERNAL;
              hostlist_destroy(l);
              hostlist_destroy(r);
              hostlist_destroy(h);
              return NULL;
            }
          
          if (rv > 0)
            hostlist_push(h, buf);

          hostlist_sort(h);
          hostlist_uniq(h);
          hostlist_destroy(l);
          hostlist_destroy(r);
          return h;
        }
      else if (strcmp(t->str, ",") == 0)
        {
          hostlist_iterator_t itr;
          char *node;

          if (!(itr = hostlist_iterator_create(l)))
            {
              hostlist_destroy(l);
              hostlist_destroy(r);
              hostlist_destroy(h);
              return NULL;
            }
          
          while ((node = hostlist_next(itr))) 
            {
              if (hostlist_find(r, node) >= 0)
                {
                  if (hostlist_push_host(h, node) <= 0)
                    {
                      handle->errnum = GENDERS_ERR_INTERNAL;
                      hostlist_destroy(l);
                      hostlist_destroy(r);
                      hostlist_destroy(h);
                      free(node);
                      return NULL;
                    }
                }
              free(node);
            }
          
          hostlist_sort(h);
          hostlist_uniq(h);
          hostlist_iterator_destroy(itr);
          hostlist_destroy(l);
          hostlist_destroy(r);
          return h;
        }
      else if (strcmp(t->str, "-") == 0)
        {
          hostlist_iterator_t itr;
          char *node;

          if (!(itr = hostlist_iterator_create(l)))
            {
              hostlist_destroy(l);
              hostlist_destroy(r);
              hostlist_destroy(h);
              return NULL;
            }
          
          while ((node = hostlist_next(itr))) 
            {
              if (hostlist_find(r, node) < 0)
                {
                  if (hostlist_push_host(h, node) <= 0)
                    {
                      handle->errnum = GENDERS_ERR_INTERNAL;
                      hostlist_destroy(l);
                      hostlist_destroy(r);
                      hostlist_destroy(h);
                      free(node);
                      return NULL;
                    }
                }
              free(node);
            }
          
          hostlist_sort(h);
          hostlist_uniq(h);
          hostlist_iterator_destroy(itr);
          hostlist_destroy(l);
          hostlist_destroy(r);
          return h;
        }
      else
        {
          handle->errnum = GENDERS_ERR_INTERNAL;
          hostlist_destroy(l);
          hostlist_destroy(r);
          hostlist_destroy(h);
          return NULL;
        }
    }

}

int
genders_query(genders_t handle, char *query, char *nodes[], int len)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node;
  int retval = -1;
  int index = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (query == NULL || nodes == NULL || len <= 0)
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  if (_parse_query(handle, query) < 0)
    goto cleanup;

  if (_check_attrs(handle, genders_treeroot) < 0)
    goto cleanup;

  if (!(h = _calc_query(handle, genders_treeroot)))
    goto cleanup;

  if (!(itr = hostlist_iterator_create(h)))
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  while ((node = hostlist_next(itr)))
    {
      if (_put_in_list(handle, node, nodes, index++, len) < 0)
        {
          free(node);
          goto cleanup;
        }
      free(node);
    }

  /* genders_printtree(genders_treeroot); */
  handle->errnum = GENDERS_ERR_SUCCESS;
  retval = index;
 cleanup:
  hostlist_iterator_destroy(itr);
  hostlist_destroy(h);
  if (genders_treeroot)
    genders_freetree(genders_treeroot);
  return retval;
}
%}

%start input
%token ATTRTOK LPARENTOK RPARENTOK PLUSTOK MINUSTOK COMMATOK

%union {
  char *attr;
  struct genders_treenode *tree;
}
%token <attr> ATTRTOK
%type <tree> term query
%%

input: query 
           {
             genders_treeroot = $1;
           }
       ;

query: term {$$ = $1;}
       | query PLUSTOK term 
           {
             $$ = genders_makenode("+", $1, $3);
           }
       | query MINUSTOK term 
           {
             $$ = genders_makenode("-", $1, $3);
           }
       | query COMMATOK term 
           {
             $$ = genders_makenode(",", $1, $3);
           }
       ;

term: ATTRTOK 
           {
             $$ = genders_makenode($1, NULL, NULL);
           }
      | LPARENTOK query RPARENTOK 
           {
             $$ = $2;
           }
%%