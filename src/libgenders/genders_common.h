/*****************************************************************************\
 *  $Id: genders_common.h,v 1.1 2004-06-07 20:14:37 achu Exp $
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

#define GENDERS_MAGIC_NUM         0xdeadbeef

#define GENDERS_READLINE_BUFLEN   65536

/* Impossible to have a genders value with spaces */
#define GENDERS_NOVALUE           "  NOVAL  "   

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN            64
#endif

#ifndef MAX
#define MAX(x,y)                  ((x > y) ? x : y)
#endif

/* stores node name and a list of pointers to attrval lists containing
 * the attributes and values of this node.  The pointers point to
 * lists stored within the attrvalslist parameter of the genders
 * handle.
 */
struct genders_node {
  char *name;
  List attrlist;
  int attrcount;
};
typedef struct genders_node *genders_node_t;

/* stores attribute name and its value (if no value, val == NULL) */
struct genders_attrval {
  char *attr;
  char *val;
};
typedef struct genders_attrval *genders_attrval_t;

/* Genders handle, caches all information loaded from a genders
 * database.  Consider the following genders database
 *
 * nodename[1-2]  attrname1=val1,attrname2=val2
 * nodename1      attrname3=val3,attrname4
 * nodename2      attrname5   
 * nodename3      attrname6
 *
 * After the genders database has been loaded using genders_load_data,
 * the lists and data in the handle can be viewed like the following:
 *
 * magic = GENDERS_MAGIC_NUM
 * errnum = current error code
 * is_loaded = 1
 * numnodes = 3
 * numattrs = 6 
 * maxattrs = 4
 * maxnodelen = 9
 * maxattrlen = 5
 * maxvallen = 4
 * nodename = localhost
 * nodeslist = node1 -> node2 -> node3 -> \0
 *    node1.name = nodename1, node1.attrlist = listptr1 -> listptr2 -> \0
 *    node2.name = nodename2, node2.attrlist = listptr1 -> listptr3 -> \0
 *    node3.name = nodename3, node3.attrlist = listptr4 -> \0
 * attrvalslist = listptr1 -> listptr2 -> listptr3 -> listptr4 -> \0
 *    listptr1 = attr1 -> attr2 -> \0
 *    listptr2 = attr3 -> attr4 -> \0
 *    listptr3 = attr5 -> \0
 *    listptr4 = attr6 -> \0
 *      attr1.attr = attrname1, attr1.val = val1
 *      attr2.attr = attrname2, attr2.val = val2
 *      attr3.attr = attrname3, attr3.val = val3
 *      attr4.attr = attrname4, attr4.val = NULL
 *      attr5.attr = attrname5, attr5.val = NULL
 *      attr6.attr = attrname6, attr6.val = NULL
 * attrslist = attrname1 -> attrname2 -> attrname3 -> attrname4 -> 
 *             attrname5 -> attrname6 -> \0
 * valbuf -> buffer of length 5 (maxvallen + 1) 
 *
 * If genders_index_nodes is called:
 * node_index = hash table with
 *              KEY(nodename1): node1
 *              KEY(nodename2): node2
 *              KEY(nodename3): node3
 *
 * If genders_index_attrs is called:
 * attr_index = hash table with
 *              KEY(attrname1): node1 -> node2
 *              KEY(attrname2): node1 -> node2
 *              KEY(attrname3): node1
 *              KEY(attrname4): node1
 *              KEY(attrname5): node2
 *              KEY(attrname6): node3 
 */
struct genders {
  int magic;                        /* magic number */ 
  int errnum;                       /* error code */
  int is_loaded;                    /* genders data loaded? */
  int numnodes;                     /* number of nodes in database */
  int numattrs;                     /* number of attrs in database */
  int maxattrs;                     /* max attrs any one node has */
  int maxnodelen;                   /* max node name length */
  int maxattrlen;                   /* max attr name length */
  int maxvallen;                    /* max value name length */
  char nodename[MAXHOSTNAMELEN+1];  /* local hostname */
  List nodeslist;                   /* Lists of genders_node */
  List attrvalslist;                /* Lists of ptrs to Lists of genders_attrvals */
  List attrslist;                   /* List of unique attribute strings */
  char *valbuf;                     /* Buffer for value substitution */
  hash_t node_index;                /* Index table for quicker node access */
  hash_t attr_index;                /* Index table for quicker search times */
  hash_t attrval_index;             /* Index table for quicker search times */
  char *attrval_index_attr;         /* Current indexed attr in attrval_index */ 
};

/* Error messages */
static char * errmsg[] = {
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
  "node not found",
  "out of memory",
  "genders handle magic number incorrect, improper handle passed in",
  "unknown internal error"
  "error number out of range",
};

int _handle_error_check(genders_t handle);
int _unloaded_handle_error_check(genders_t handle);
int _loaded_handle_error_check(genders_t handle);
int _put_in_list(genders_t handle, char *str, char **list, int index, int len);

#endif
