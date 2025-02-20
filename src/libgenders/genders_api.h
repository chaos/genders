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

#ifndef _GENDERS_API_H
#define _GENDERS_API_H 1

#include "genders_constants.h"
#include "list.h"
#include "hash.h"
#include "hostlist.h"

#define GENDERS_MAGIC_NUM                0xdeadbeef

/* Impossible to have a genders value with spaces */
#define GENDERS_NOVALUE                  "  NOVAL  "

#define GENDERS_ATTR_INDEX_INIT_SIZE     512

/*
 * struct genders_hosts
 *
 * stores a hostlist and a set of attrvals that belong to that hostlist
 */
struct genders_hosts {
  hostlist_t hl;
  hash_t attrval_index;
  int attrval_index_size;
};
typedef struct genders_hosts *genders_hosts_t;

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
 * struct genders
 *
 * Genders handle, caches all information loaded from a genders
 * database.  Consider the following genders database
 *
 * nodename[1-3]  attrname1=val1,attrname2=val2
 * nodename[2-3]  attrname3=val3,attrname4
 * nodename4      attrname5
 *
 * After the genders database has been loaded using genders_load_data,
 * the lists and data in the handle can be viewed like the following:
 *
 * magic = GENDERS_MAGIC_NUM
 * errnum = current error code
 * is_loaded = 1
 * numnodes = 3
 * numattrs = 6
 * maxnodelen = 9
 * maxattrlen = 5
 * maxvallen = 4
 * nodename = localhost
 *
 * hosts = hosts1 -> hosts2 -> hosts3 -> \0
 *    hosts1.hl = "nodename[1-3]"
 *    hosts1.attr_index = hash w/
 *      hosts1.attr_index["attrname1"] = val1
 *      hosts1.attr_index["attrname2"] = val2
 *
 *    hosts2.hl = "nodename[2-3]"
 *    hosts2.attr_index = hash w/
 *      hosts2.attr_index["attrname3"] = val3
 *      hosts2.attr_index["attrname3"] = val4
 *
 *    hosts3.hl = "nodename4"
 *    hosts3.attr_index = hash w/
 *      hosts3.attr_index["attrname5"] = NULL
 *
 * hostnames - "nodename[1-4]"
 *
 * attrs - hash w/
 *   attrs["attrname1"] = "attrname1"
 *   attrs["attrname2"] = "attrname2"
 *   attrs["attrname3"] = "attrname3"
 *   attrs["attrname4"] = "attrname4"
 *   attrs["attrname5"] = "attrname5"
 *
 * valbuf -> buffer of length 5 (maxvallen + 1)
 */
struct genders {
  int magic;                                /* magic number */
  int errnum;                               /* error code */
  int is_loaded;                            /* genders loaded flag */
  unsigned int flags;                       /* flags for alternate behavior */
  int numnodes;                             /* number of nodes */
  int numattrs;                             /* number of attrs */
  int maxnodelen;                           /* max node name length */
  int maxattrlen;                           /* max attr name length */
  int maxvallen;                            /* max value name length */
  char nodename[GENDERS_MAXHOSTNAMELEN+1];  /* local hostname */
  List genders_hosts;                       /* List of genders_hosts */
  hostlist_t hostnames;                     /* hostlist w/ all hostnames */
  hash_t attrs;                             /* Hash of unique attribute strings */
  int attrs_size;
  char *valbuf;                             /* Buffer for value substitution */
  hash_t attrval_index;                     /* Index table for quicker search times */
  char *attrval_index_attr;                 /* Current indexed attr in attrval_index */
  List attrval_buflist;                     /* List to store val buffers to be freed */
};

#endif /* _GENDERS_API_H */
