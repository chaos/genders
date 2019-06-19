/*****************************************************************************\
 *  $Id: genders_test_database.h,v 1.7 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_TEST_DATABASE_H
#define _GENDERS_TEST_DATABASE_H 1

#define GENDERS_DATABASE_MAXNODES 100
#define GENDERS_DATABASE_MAXATTRS 100
#define GENDERS_DATABASE_MAXVALS  GENDERS_DATABASE_MAXATTRS

#define GENDERS_DATABASE_BUFLEN   1024

/* Need constants for initializers */
#define GENDERS_DATABASE_CORNER_CASE_NUMNODES   2
#define GENDERS_DATABASE_CORNER_CASE_NUMATTRS   2
#define GENDERS_DATABASE_CORNER_CASE_MAXATTRS   2
#define GENDERS_DATABASE_CORNER_CASE_MAXNODELEN 5
#define GENDERS_DATABASE_CORNER_CASE_MAXATTRLEN 5
#define GENDERS_DATABASE_CORNER_CASE_MAXVALLEN  4

/* Fake data */
#define GENDERS_DATABASE_INVALID_NODE    "fakenode"
#define GENDERS_DATABASE_INVALID_ATTR    "fakeattr"
#define GENDERS_DATABASE_INVALID_VAL     "fakeval"

typedef struct {
  char *filename;
  int expected_parse_errors;
} genders_parse_error_database_t;

typedef struct {
  char *node;
  char *attr_without_val;
  char *attr_with_val;
  char *val;
  /* Expected values from genders_getnumnodes, genders_getnumattrs,
   * genders_getmaxattrs, genders_maxnodelen, genders_maxattrlen, and
   *  genders_maxvallen respectively
   */
  int numnodes;
  int numattrs;
  int maxattrs;
  int maxnodelen;
  int maxattrlen;
  int maxvallen;
  /* Contains the nodes, attributes, and values in the test database.
   * - Used for various testing and comparisons.
   */
  char *nodes[GENDERS_DATABASE_MAXNODES];
  int nodeslen;
  char *attrs[GENDERS_DATABASE_MAXATTRS];
  int attrslen;
  char *vals[GENDERS_DATABASE_MAXVALS];
  int valslen;
  /* node_attrvals
   * - Lists the attributes and values for each node in the nodes array above
   * - vals_string - for string comparisons (genders_getattr, genders_testattr)
   * - vals_input - for input to functions (genders_testattrval)
   */
  struct {
    char *attrs[GENDERS_DATABASE_MAXATTRS];
    char *vals_string[GENDERS_DATABASE_MAXVALS];
    char *vals_input[GENDERS_DATABASE_MAXVALS];
    int attrslen;
  } node_attrvals[GENDERS_DATABASE_MAXNODES];
  int node_attrvals_len;
  /* attrval_nodes
   * - Lists nodes based on attr or attr=val combinations. 
   * - For use with genders_getnodes, genders_query, and genders_testquery
   */
  struct {
    char *attr;
    char *val;
    char *nodes[GENDERS_DATABASE_MAXNODES];
    int nodeslen;
  } attrval_nodes[GENDERS_DATABASE_MAXATTRS];
  int attrval_nodes_len;
} genders_database_data_t;

typedef struct {
  char *filename;
  genders_database_data_t *data;
} genders_database_t;

extern genders_database_t genders_database_not_exist;
extern genders_database_t genders_database_corner_case;
extern genders_database_t genders_database_base;

extern genders_parse_error_database_t genders_parse_error_databases[];

extern genders_database_t *genders_functionality_databases[];

extern genders_database_t genders_database_flag_test_raw_values;

#endif /* _GENDERS_TEST_DATABASE_H */
