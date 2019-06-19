%{
/*****************************************************************************\
 *  $Id: genders_query.y,v 1.34 2009-06-02 18:05:21 chu11 Exp $
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
#if HAVE_PATHS_H
#include <paths.h>
#endif /* HAVE_PATHS_H */

#include "genders.h"
#include "genders_api.h"
#include "genders_constants.h"
#include "genders_util.h"

/* 
 * struct genders_treenode
 *
 * stores query parse information
 */
struct genders_treenode {
  char *str;
  struct genders_treenode *left;
  struct genders_treenode *right;
  int complement;
};

/* 
 * genders_query_err
 *
 * Used to store errnum until handle errnum can be set
 */ 
static int genders_query_err = 0; 

/* 
 * genders_treeroot
 *
 * To store the parse tree
 */ 
static struct genders_treenode *genders_treeroot = NULL;

#ifndef _PATH_DEVNULL
#define _PATH_DEVNULL "/dev/null"
#endif /* _PATH_DEVNULL */

extern int yylex();
extern int yyparse(void);

/* 
 * _genders_makenode
 *
 * Make a genders treenode
 *
 * Returns pointer to new node on success, NULL on error
 */ 
static struct genders_treenode *
_genders_makenode(char *str, void *left, void *right)
{
  struct genders_treenode *t; 

  if (genders_query_err != GENDERS_ERR_SUCCESS)
    return NULL;

  if (!str || !((!left && !right) || (left && right)))
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      return NULL;
    }

  if (!(t = (struct genders_treenode *)malloc(sizeof(struct genders_treenode)))) 
    {
      genders_query_err = GENDERS_ERR_OUTMEM;
      return NULL;
    }

  /* No wrapper, no handle->errnum */
  if (!(t->str = (char *)strdup(str))) 
    {
      genders_query_err = GENDERS_ERR_OUTMEM;
      free(t);
      return NULL;
    }

  t->left = left;
  t->right = right;
  t->complement = 0;
  return t;
} 

/* 
 * _genders_set_complement_flag
 *
 * set the complement flag on a node
 */ 
static void
_genders_set_complement_flag(void *node)
{
  struct genders_treenode *t;

  if (genders_query_err != GENDERS_ERR_SUCCESS)
    return;

  if (!node)
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      return;
    }

  t = (struct genders_treenode *)node; 

  /* do ! instead of ++ so double negation is allowed */
  t->complement = !(t->complement); 
}

/* 
 * _genders_free_treenode
 *
 * Free a genders_treenode
 */
static void
_genders_free_treenode(struct genders_treenode *t)
{
  if (!t)
    return;

  _genders_free_treenode(t->left);
  _genders_free_treenode(t->right);
  free(t->str);
  free(t);

  return;
}

/* 
 * yyerror
 *
 * Override yacc default, set genders query error to syntax error
 */
void 
yyerror(const char *str) 
{
  if (!genders_query_err)
    genders_query_err = GENDERS_ERR_SYNTAX;
}
  
/* 
 * yywrap
 *
 * Override yacc default, Returns 1 to inform yacc we're done parsing.
 */
int 
yywrap()
{
  return 1;
}

/* 
 * _parse_query
 *
 * Parse the genders query.  Sets up pipes appropriately to work with
 * yacc.
 * 
 * Returns 0 on success, -1 on error
 */
static int 
_parse_query(genders_t handle, const char *query)
{
  extern FILE *yyin, *yyout; 
  int fds[2];

  genders_query_err = GENDERS_ERR_SUCCESS;
  genders_treeroot = NULL;

  /* yyparse() only works with FILE streams, not file descriptors or
   * strings.  So we gotta do some funky hacking here to make yyparse()
   * work with our input query.
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
  if (close(fds[1]) < 0)
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  if (!(yyin = fdopen(fds[0], "r"))) 
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  /* disable yacc output to stdout */
  if (!(yyout = fopen(_PATH_DEVNULL, "r+"))) 
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }

  yyparse();

  /* For example, this can happen if the user passes in all whitespace */
  if (!genders_treeroot && genders_query_err == GENDERS_ERR_SUCCESS)
    genders_query_err = GENDERS_ERR_SYNTAX;

 cleanup:
  /* no need to close fds[0] and fds[1], fclose is enough */
  fclose(yyin);
  fclose(yyout);
  
  if (genders_query_err != GENDERS_ERR_SUCCESS) 
    {
      handle->errnum = genders_query_err;
      return -1;
    }
  return 0;
}

/* 
 * _calc_attrval_nodes
 *
 * Determines the nodes containing this treenode's attr and
 * value.
 *
 * Returns hostlist on success, NULL on error
 */
static hostlist_t
_calc_attrval_nodes(genders_t handle, struct genders_treenode *t)
{
  hostlist_t h = NULL;
  char **nodes = NULL;
  int i, len, num;
  char *attr, *val;
  int errnum_save;
    
  attr = t->str; 
  if ((val = strchr(attr, '=')))
    *val++ = '\0';
    
  if ((len = genders_nodelist_create(handle, &nodes)) < 0)
    return NULL;

  if ((num = genders_getnodes(handle, nodes, len, attr, val)) < 0)
    goto cleanup;

  __hostlist_create(h, NULL);
  for (i = 0; i < num; i++) 
    {
      if (!hostlist_push(h, nodes[i])) 
	{
	  handle->errnum = GENDERS_ERR_INTERNAL;
	  goto cleanup;
	}
    }

  genders_nodelist_destroy(handle, nodes);
  hostlist_uniq(h);
  return h;

 cleanup:
  errnum_save = handle->errnum;
  genders_nodelist_destroy(handle, nodes);
  handle->errnum = errnum_save;
  __hostlist_destroy(h);
  return NULL;
}

/* 
 * _calc_union
 *
 * Determine the union of two hostlists.
 *
 * Returns resulting hostlist on success, NULL on error
 */
static hostlist_t
_calc_union(genders_t handle, hostlist_t l, hostlist_t r)
{
  hostlist_t h = NULL;
  char buf[GENDERS_BUFLEN];
  int rv;
  
  __hostlist_create(h, NULL);
  memset(buf, '\0', GENDERS_BUFLEN);
  if ((rv = hostlist_ranged_string(l, GENDERS_BUFLEN, buf)) < 0) 
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }
  
  if (rv > 0)
    hostlist_push(h, buf);
  
  memset(buf, '\0', GENDERS_BUFLEN);
  if ((rv = hostlist_ranged_string(r, GENDERS_BUFLEN, buf)) < 0) 
    {
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
            
/* 
 * _calc_intersection
 *
 * Determine the intersection of two hostlists.
 *
 * Returns resulting hostlist on success, NULL on error
 */
static hostlist_t
_calc_intersection(genders_t handle, hostlist_t l, hostlist_t r)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node = NULL;
  
  __hostlist_create(h, NULL);
  __hostlist_iterator_create(itr, l);
  while ((node = hostlist_next(itr))) 
    {
      if (hostlist_find(r, node) >= 0) 
	{
	  if (hostlist_push_host(h, node) <= 0) 
	    {
	      handle->errnum = GENDERS_ERR_INTERNAL;
	      goto cleanup;
	    }
	}
      free(node);
    }
  node = NULL;

  hostlist_uniq(h);
  __hostlist_iterator_destroy(itr);
  return h;
 cleanup:
  __hostlist_iterator_destroy(itr);
  __hostlist_destroy(h);
  free(node);
  return NULL;
}

/* 
 * _calc_set_difference
 *
 * Determine the set difference between two hostlists.
 *
 * Returns resulting hostlist on success, NULL on error
 */
static hostlist_t
_calc_set_difference(genders_t handle, hostlist_t l, hostlist_t r)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node = NULL;
    
  __hostlist_create(h, NULL);
  __hostlist_iterator_create(itr, l);
      
  while ((node = hostlist_next(itr))) 
    {
      if (hostlist_find(r, node) < 0) 
	{
	  if (hostlist_push_host(h, node) <= 0) 
	    {
	      handle->errnum = GENDERS_ERR_INTERNAL;
	      goto cleanup;
	    }
	}
      free(node);
    }
  node = NULL;

  hostlist_uniq(h);
  __hostlist_iterator_destroy(itr);
  return h;
 cleanup:
  __hostlist_iterator_destroy(itr);
  __hostlist_destroy(h);
  free(node);
  return NULL;

}

/* 
 * _calc_complement
 *
 * Determine the complement of a hostlist.
 *
 * Returns resulting hostlist on success, NULL on error
 */
static hostlist_t
_calc_complement(genders_t handle, hostlist_t h)
{
  hostlist_t ch = NULL;
  char **nodes = NULL;
  char *node = NULL;
  int i, len, num;
  int errnum_save;
    
  if ((len = genders_nodelist_create(handle, &nodes)) < 0)
    return NULL;

  if ((num = genders_getnodes(handle, nodes, len, NULL, NULL)) < 0) 
    goto cleanup;
  
  __hostlist_create(ch, NULL);
  for (i = 0; i < num; i++) 
    {
      if (hostlist_find(h, nodes[i]) < 0) 
	{
	  if (hostlist_push_host(ch, nodes[i]) <= 0) 
	    {
	      handle->errnum = GENDERS_ERR_INTERNAL;
	      goto cleanup;
	    }
	}
      free(node);
    }
  node = NULL;

  genders_nodelist_destroy(handle, nodes);
  hostlist_uniq(ch);
  return ch;

 cleanup:
  errnum_save = handle->errnum;
  genders_nodelist_destroy(handle, nodes);
  handle->errnum = errnum_save;
  __hostlist_destroy(ch);
  free(node);
  return NULL;
}

/* 
 * _calc_query
 *
 * Determine the nodes for the query rooted at 't'.
 *
 * Returns resulting hostlist on success, NULL on error
 */
static hostlist_t
_calc_query(genders_t handle, struct genders_treenode *t)
{
  hostlist_t h = NULL;

  if (!t)
    {
      handle->errnum = GENDERS_ERR_INTERNAL;
      return NULL;;
    }

  if (!t || !((!t->left && !t->right) || (t->left && t->right)))
    {
      genders_query_err = GENDERS_ERR_INTERNAL;
      return NULL;
    }

  if (!t->left && !t->right)
    h = _calc_attrval_nodes(handle, t);
  else {
    hostlist_t l = NULL;
    hostlist_t r = NULL;

    if (!(l = _calc_query(handle, t->left)))
      goto cleanup_calc;
    if (!(r = _calc_query(handle, t->right)))
      goto cleanup_calc;
    
    /* || is Union
     * && is Intersection
     * -- is Set Difference
     */
    
    if (!strcmp(t->str, "||"))
      h = _calc_union(handle, l, r);
    else if (!strcmp(t->str, "&&")) 
      h = _calc_intersection(handle, l, r);
    else if (!strcmp(t->str, "--")) 
      h = _calc_set_difference(handle, l, r);
    else {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup_calc;
    }

    if (!h) 
      {
      cleanup_calc:
	__hostlist_destroy(l);
	__hostlist_destroy(r);
	return NULL;
      }
  }

  if (t->complement) 
    {
      hostlist_t temp;
      if (!(temp = _calc_complement(handle, h)))
        {
          __hostlist_destroy(h);
          return NULL;
        }
      __hostlist_destroy(h);
      h = temp;
    }
  return h;
}

int
genders_query(genders_t handle, char *nodes[], int len, const char *query)
{
  hostlist_t h = NULL;
  hostlist_iterator_t itr = NULL;
  char *node = NULL;
  int index = 0, rv = -1;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((!nodes && len > 0) || len < 0) 
    {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      goto cleanup;
    }

  /* Special case for NULL or empty string query */
  if (!query || !strlen(query))
    return genders_getnodes(handle, nodes, len, NULL, NULL);

  if (_parse_query(handle, query) < 0)
    goto cleanup;
  
  if (!(h = _calc_query(handle, genders_treeroot)))
    goto cleanup;

  __hostlist_iterator_create(itr, h);
  while ((node = hostlist_next(itr))) 
    {
      if (_genders_put_in_array(handle, node, nodes, index++, len) < 0)
	goto cleanup;
      free(node);
    }
  node = NULL;

  rv = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  __hostlist_iterator_destroy(itr);
  __hostlist_destroy(h);
  if (genders_treeroot)
    _genders_free_treenode(genders_treeroot);
  free(node);
  /* reset */
  genders_treeroot = NULL;
  genders_query_err = 0;
  return rv;
}

int
genders_testquery(genders_t handle, 
                  const char *node,
                  const char *query)
{
  genders_node_t n;
  char **nodelist = NULL;
  int i, len, num;
  int found = 0;
  int rv = -1;
  int errnum_save;

  if (_genders_loaded_handle_error_check(handle) < 0)
    return -1;

  if (!query || !strlen(query))
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

  if ((len = genders_nodelist_create(handle, &nodelist)) < 0)
    return -1;

  if ((num = genders_query(handle, nodelist, len, query)) < 0)
    goto cleanup;
  
  for (i = 0; i < num; i++)
    {
      if (!strcmp(nodelist[i], node))
        {
          found++;
          break;
        }
    }
  
  if (found)
    rv = 1;
  else
    rv = 0;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  errnum_save = handle->errnum;
  genders_nodelist_destroy(handle, nodelist);
  handle->errnum = errnum_save;
  return rv;
}
%}

%start input
%token LPARENTOK RPARENTOK UNIONTOK INTERSECTIONTOK DIFFERENCETOK COMPLEMENTTOK

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
       | query UNIONTOK term 
           {
             $$ = _genders_makenode("||", $1, $3);
           }
       | query INTERSECTIONTOK term 
           {
             $$ = _genders_makenode("&&", $1, $3);
           }
       | query DIFFERENCETOK term 
           {
             $$ = _genders_makenode("--", $1, $3);
           }
       ;

term: ATTRTOK 
           {
             $$ = _genders_makenode($1, NULL, NULL);
           }
       | LPARENTOK query RPARENTOK 
           {
             $$ = $2;
           }
       | COMPLEMENTTOK term
           {
             _genders_set_complement_flag($2);
             $$ = $2;
           }
%%
