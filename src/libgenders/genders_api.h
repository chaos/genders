/*****************************************************************************\
 *  $Id: genders_api.h,v 1.8 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_API_H
#define _GENDERS_API_H 1

#include "genders_constants.h"
#include "list.h"
#include "hash.h"
#include "hostlist.h"

#define GENDERS_MAGIC_NUM                0xdeadbeef

/* Impossible to have a genders value with spaces */
#define GENDERS_NOVALUE                  "  NOVAL  "   

#define GENDERS_NODE_INDEX_INIT_SIZE     2048

#define GENDERS_ATTR_INDEX_INIT_SIZE     128

#define GENDERS_ATTRLIST_INDEX_INIT_SIZE 128

/* 
 * struct genders_node
 *
 * stores node name and a list of pointers to attrval lists containing
 * the attributes and values of this node.  The pointers point to
 * lists stored within the attrvalslist parameter of the genders
 * handle.  The attrlist_index is hash that enables faster lookups
 * into the attrlist.
 */
struct genders_node {
  char *name;
  List attrlist;
  int attrcount;
  hash_t attrlist_index;
  int attrlist_index_size;
};
typedef struct genders_node *genders_node_t;

/* 
 * struct genders_attrval
 *
 * stores attribute name and its value (if no value, val == NULL) 
 */
struct genders_attrval {
  char *attr;
  char *val;
  int val_contains_subst;       /* to limit constant calls to strstr() */
};
typedef struct genders_attrval *genders_attrval_t;

/*
 * struct genders_attrvals_container
 *
 * stores attrvals list and an index indicating its location.
 * Implemented predominantly to make genders_copy() easier to
 * implement.
 */
struct genders_attrvals_container {
  List attrvals;
  unsigned int index;
};
typedef struct genders_attrvals_container *genders_attrvals_container_t;

/* 
 * struct genders
 * 
 * Genders handle, caches all information loaded from a genders
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
 *    node1.attrlist_index = hash table with
 *          KEY(attr1): listptr1
 *          KEY(attr2): listptr1
 *          KEY(attr3): listptr2
 *          KEY(attr4): listptr2
 *    node2.name = nodename2, node2.attrlist = listptr1 -> listptr3 -> \0
 *    node2.attrlist_index = hash table with
 *          KEY(attr1): listptr1
 *          KEY(attr2): listptr1
 *          KEY(attr5): listptr3
 *    node3.name = nodename3, node3.attrlist = listptr4 -> \0
 *    node3.attrlist_index = hash table with
 *          KEY(attr6): listptr4
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
 * node_index = hash table with
 *              KEY(nodename1): node1
 *              KEY(nodename2): node2
 *              KEY(nodename3): node3
 *
 * attr_index = hash table with
 *              KEY(attrname1): node1 -> node2
 *              KEY(attrname2): node1 -> node2
 *              KEY(attrname3): node1
 *              KEY(attrname4): node1
 *              KEY(attrname5): node2
 *              KEY(attrname6): node3 
 */
struct genders {
  int magic;                                /* magic number */ 
  int errnum;                               /* error code */
  int is_loaded;                            /* genders loaded flag */
  unsigned int flags;                       /* flags for alternate behavior */
  int numnodes;                             /* number of nodes */
  int numattrs;                             /* number of attrs */
  int maxattrs;                             /* max attrs for any one node */
  int maxnodelen;                           /* max node name length */
  int maxattrlen;                           /* max attr name length */
  int maxvallen;                            /* max value name length */
  char nodename[GENDERS_MAXHOSTNAMELEN+1];  /* local hostname */
  List nodeslist;                           /* Lists of genders_node */
  List attrvalslist;                        /* Lists of ptrs to Lists of genders_attrvals */
  List attrslist;                           /* List of unique attribute strings */
  char *valbuf;                             /* Buffer for value substitution */
  hash_t node_index;                        /* Index table for quicker node access */
  int node_index_size;                      /* Index size for node_index */
  hash_t attr_index;                        /* Index table for quicker search times */
  int attr_index_size;                      /* Index size for attr_index */
  hash_t attrval_index;                     /* Index table for quicker search times */
  char *attrval_index_attr;                 /* Current indexed attr in attrval_index */
  List attrval_buflist;                     /* List to store val buffers to be free */
};

#endif /* _GENDERS_API_H */
