%{
/*****************************************************************************\
 *  $Id: genders_query.y,v 1.17 2004-12-22 00:01:13 achu Exp $
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
#include <paths.h>
#include "genders.h"
#include "genders_common.h"

struct genders_treenode {
  char *str;
  struct genders_treenode *left;
  struct genders_treenode *right;
  int complement;
};

int genders_query_err; 
struct genders_treenode *genders_treeroot;

#define HOSTLIST_BUFLEN 32768

extern int yylex();

static struct genders_treenode *
genders_makenode(char *str, void *left, void *right)
{
  struct genders_treenode *t; 

  if (genders_query_err)
    return NULL;

  if (!(t = (struct genders_treenode *)malloc(sizeof(struct genders_treenode)))) {
    genders_query_err = GENDERS_ERR_OUTMEM;
    return NULL;
  }

  /* No wrapper, no handle->errnum */
  if (!(t->str = (char *)strdup(str))) {
    genders_query_err = GENDERS_ERR_OUTMEM;
    return NULL;
  }

  t->left = left;
  t->right = right;
  t->complement = 0;
  return t;
} 

static void
genders_set_complement_flag(void *node)
{
  struct genders_treenode *t = (struct genders_treenode *)node; 
  t->complement++;
}

static void
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

void 
yyerror(const char *str) 
{
  if (!genders_query_err)
    genders_query_err = GENDERS_ERR_SYNTAX;
}
  
int 
yywrap()
{
  return 1;
}

static int 
_parse_query(genders_t handle, char *query)
{
  extern FILE *yyin, *yyout; 
  int fds[2];

  genders_query_err = GENDERS_ERR_SUCCESS;
  genders_treeroot = NULL;

  /* yyparse() only works with FILE streams, not file descriptors or
   * strings.  So we gotta do some funky hacking here to make yyparse()
   * work with our input query.
   */

  if (pipe(fds) < 0) {
    genders_query_err = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }

  if (write(fds[1], query, strlen(query)) < 0) {
    genders_query_err = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }
  close(fds[1]);

  if (!(yyin = fdopen(fds[0], "r"))) {
    genders_query_err = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }

  /* disable yacc output to stdout */
  if (!(yyout = fopen(_PATH_DEVNULL, "r+"))) {
    genders_query_err = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }

  yyparse();

  /* For example, this can happen if the user passes in all whitespace */
  if (!genders_treeroot && genders_query_err == GENDERS_ERR_SUCCESS)
    genders_query_err = GENDERS_ERR_SYNTAX;

 cleanup:
  /* no need to close fds[0], fclose is enough */
  fclose(yyin);
  fclose(yyout);
  
  if (genders_query_err != GENDERS_ERR_SUCCESS) {
    handle->errnum = genders_query_err;
    return -1;
  }
  return 0;
}

static hostlist_t
_calc_attrval(genders_t handle, struct genders_treenode *t)
{
  hostlist_t h = NULL;
  char **nodes = NULL;
  int i, len, num;
  char *attr, *val;
    
  attr = t->str; 
  if ((val = strchr(attr, '=')))
    *val++ = '\0';
    
  if ((len = genders_nodelist_create(handle, &nodes)) < 0)
    return NULL;

  if ((num = genders_getnodes(handle, nodes, len, attr, val)) < 0)
    return NULL;

  __hostlist_create(h, NULL);
  for (i = 0; i < num; i++) {
    if (hostlist_push(h, nodes[i]) == 0) {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }
  }

  genders_nodelist_destroy(handle, nodes);
  hostlist_uniq(h);
  return h;
 cleanup:
  genders_nodelist_destroy(handle, nodes);
  __hostlist_destroy(h);
  return NULL;
}

static hostlist_t
_calc_union(genders_t handle, hostlist_t l, hostlist_t r)
{
  hostlist_t h = NULL;
  char buf[HOSTLIST_BUFLEN];
  int rv;
  
  __hostlist_create(h, NULL);
  memset(buf, '\0', HOSTLIST_BUFLEN);
  if ((rv = hostlist_ranged_string(l, HOSTLIST_BUFLEN, buf)) < 0) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }
  
  if (rv > 0)
    hostlist_push(h, buf);
  
  memset(buf, '\0', HOSTLIST_BUFLEN);
  if ((rv = hostlist_ranged_string(r, HOSTLIST_BUFLEN, buf)) < 0) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }
  
  if (rv > 0)
    hostlist_push(h, buf);
  
  hostlist_uniq(h);
  return h;
 cleanup:
  __hostlist_destroy(h);
  return NULL;
}
            
static hostlist_t
_calc_intersection(genders_t handle, hostlist_t l, hostlist_t r)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node;
  
  __hostlist_create(h, NULL);
  __hostlist_iterator_create(itr, l);
  while ((node = hostlist_next(itr))) {
    if (hostlist_find(r, node) >= 0) {
      if (hostlist_push_host(h, node) <= 0) {
        free(node);
        handle->errnum = GENDERS_ERR_INTERNAL;
        goto cleanup;
      }
    }
    free(node);
  }
  
  hostlist_uniq(h);
  __hostlist_iterator_destroy(itr);
  return h;
 cleanup:
  __hostlist_iterator_destroy(itr);
  __hostlist_destroy(h);
  return NULL;
}

static hostlist_t
_calc_difference(genders_t handle, hostlist_t l, hostlist_t r)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node;
    
  __hostlist_create(h, NULL);
  __hostlist_iterator_create(itr, l);
      
  while ((node = hostlist_next(itr))) {
    if (hostlist_find(r, node) < 0) {
      if (hostlist_push_host(h, node) <= 0) {
        free(node);
        handle->errnum = GENDERS_ERR_INTERNAL;
        goto cleanup;
      }
    }
    free(node);
  }
          
  hostlist_uniq(h);
  __hostlist_iterator_destroy(itr);
  return h;
 cleanup:
  __hostlist_iterator_destroy(itr);
  __hostlist_destroy(h);
  return NULL;

}

static hostlist_t
_calc_complement(genders_t handle, hostlist_t h)
{
  hostlist_t ch = NULL;
  char **nodes = NULL;
  char *node = NULL;
  int i, len, num;
    
  if ((len = genders_nodelist_create(handle, &nodes)) < 0)
    return NULL;

  if ((num = genders_getnodes(handle, nodes, len, NULL, NULL)) < 0)
    return NULL;
  
  __hostlist_create(ch, NULL);
  for (i = 0; i < num; i++) {
    if (hostlist_find(h, nodes[i]) < 0) {
      if (hostlist_push_host(ch, nodes[i]) <= 0) {
        free(node);
        handle->errnum = GENDERS_ERR_INTERNAL;
        goto cleanup;
      }
    }
    free(node);
  }
 
  genders_nodelist_destroy(handle, nodes);
  hostlist_uniq(ch);
  return ch;
 cleanup:
  genders_nodelist_destroy(handle, nodes);
  __hostlist_destroy(ch);
  return NULL;
}

static hostlist_t
_calc_query(genders_t handle, struct genders_treenode *t)
{
  hostlist_t h = NULL;

  if (!t->left && !t->right)
    h = _calc_attrval(handle, t);
  else {
    hostlist_t l = NULL;
    hostlist_t r = NULL;

    if (!(l = _calc_query(handle, t->left)))
      goto cleanup_calc;
    if (!(r = _calc_query(handle, t->right)))
      goto cleanup_calc;
    
    /* | is Union
     * & is Intersection
     * - is Set Difference
     */
    
    if (strcmp(t->str, "|") == 0)
      h = _calc_union(handle, l, r);
    else if (strcmp(t->str, "&") == 0) 
      h = _calc_intersection(handle, l, r);
    else if (strcmp(t->str, "-") == 0) 
      h = _calc_difference(handle, l, r);
    else {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup_calc;
    }

    if (!h) {
    cleanup_calc:
      __hostlist_destroy(l);
      __hostlist_destroy(r);
      return NULL;
    }
  }

  if (t->complement) {
    hostlist_t temp;
    temp = _calc_complement(handle, h);
    __hostlist_destroy(h);
    h = temp;
  }
  return h;
}

int
genders_query(genders_t handle, char *nodes[], int len, char *query)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node;
  int retval = -1;
  int index = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!nodes || len <= 0 || !query) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    goto cleanup;
  }

  /* Special case for NULL query */
  if (!query)
    return genders_getnodes(handle, nodes, len, NULL, NULL);

  if (_parse_query(handle, query) < 0)
    goto cleanup;
  
  if (!(h = _calc_query(handle, genders_treeroot)))
    goto cleanup;

  __hostlist_iterator_create(itr, h);
  while ((node = hostlist_next(itr))) {
    if (_put_in_array(handle, node, nodes, index++, len) < 0) {
      free(node);
      goto cleanup;
    }
    free(node);
  }
  
  /* genders_printtree(genders_treeroot); */
  handle->errnum = GENDERS_ERR_SUCCESS;
  retval = index;
 cleanup:
  __hostlist_iterator_destroy(itr);
  __hostlist_destroy(h);
  if (genders_treeroot)
    genders_freetree(genders_treeroot);
  return retval;
}
%}

%start input
%token ATTRTOK LPARENTOK RPARENTOK PIPETOK AMPERSANDTOK MINUSTOK TILDETOK

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
       | query PIPETOK term 
           {
             $$ = genders_makenode("|", $1, $3);
           }
       | query AMPERSANDTOK term 
           {
             $$ = genders_makenode("&", $1, $3);
           }
       | query MINUSTOK term 
           {
             $$ = genders_makenode("-", $1, $3);
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
       | TILDETOK term
           {
             genders_set_complement_flag($2);
             $$ = $2;
           }
%%
