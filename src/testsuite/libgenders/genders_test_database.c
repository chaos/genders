/*****************************************************************************\
 *  $Id: genders_test_database.c,v 1.9 2010-02-02 00:04:34 chu11 Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include "genders_test_database.h"

genders_parse_error_database_t genders_parse_error_databases[] = 
  {
#if 0
    /* No longer a parse error */
    {"testdatabases/genders.parse_error_empty_file", 1},
    /* No longer a parse error */
    {"testdatabases/genders.parse_error_comments_only", 1},
    /* we are leaving this as a "feature" */
    {"testdatabases/genders.parse_error_equal_sign_in_value_1", 1},
    /* we are leaving this as a "feature" */
    {"testdatabases/genders.parse_error_equal_sign_in_value_2", 1},
#endif
    {"testdatabases/genders.parse_error_hostname_not_shortened", 1},
    {"testdatabases/genders.parse_error_hostname_not_shortened_comma", 1},
    {"testdatabases/genders.parse_error_hostname_not_shortened_hostrange", 1},
    {"testdatabases/genders.parse_error_hostname_too_long", 1},
    {"testdatabases/genders.parse_error_hostname_too_long_comma", 1},
    {"testdatabases/genders.parse_error_hostname_too_long_hostrange", 1},
    {"testdatabases/genders.parse_error_whitespace_in_list", 2},
    {"testdatabases/genders.parse_error_whitespace_in_list_comma", 2},
    {"testdatabases/genders.parse_error_whitespace_in_list_hostrange", 2},
    {"testdatabases/genders.parse_error_whitespace_in_attr", 2},
    {"testdatabases/genders.parse_error_whitespace_in_attr_comma", 2},
    {"testdatabases/genders.parse_error_whitespace_in_attr_hostrange", 2},
    {"testdatabases/genders.parse_error_whitespace_in_val", 2},
    {"testdatabases/genders.parse_error_whitespace_in_val_comma", 2},
    {"testdatabases/genders.parse_error_whitespace_in_val_hostrange", 2},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_and_val", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_and_val_comma", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_and_val_hostrange", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_without_val", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_without_val_comma", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_without_val_hostrange", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_with_val", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_with_val_comma", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_different_line_attr_with_val_hostrange", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_and_val", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_and_val_comma", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_and_val_hostrange", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_without_val", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_without_val_comma", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_without_val_hostrange", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_with_val", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_with_val_comma", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_same_line_attr_with_val_hostrange", 1},
    {"testdatabases/genders.parse_error_duplicate_attr_not_last_in_line", 1},
    {"testdatabases/genders.parse_error_no_attr_before_equal_sign", 6},
    {"testdatabases/genders.parse_error_no_value_after_equal_sign", 6},
    {"testdatabases/genders.parse_error_errant_commas", 5},
    {"testdatabases/genders.parse_error_all", 8},
    {"testdatabases/genders.parse_error_hostrange_unbalanced", 4},
    {NULL, -1},
  };

genders_database_data_t genders_database_data_not_exist = 
  {
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    0,
    0,
    0,
    0,
    0,
    {NULL},
    0,
    {NULL},
    0,
    {NULL},
    0,
    {
      {
	{NULL},
	{NULL},
	{NULL}
      }
    },
    0,
    {
      {
	NULL,
	NULL,
	{NULL},
	0,
      },
    },
    0,
  };

genders_database_t genders_database_not_exist =
  {
    "/etc/fake1/fake2/fake3/fake4",
    &genders_database_data_not_exist,
  };

genders_database_data_t genders_database_data_corner_case = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    2,				/* GENDERS_DATABASE_CORNER_CASE_NUMNODES */
    2,				/* GENDERS_DATABASE_CORNER_CASE_NUMATTRS */
    2,				/* GENDERS_DATABASE_CORNER_CASE_MAXATTRS */
    5,				/* GENDERS_DATABASE_CORNER_CASE_MAXNODELEN */
    5,				/* GENDERS_DATABASE_CORNER_CASE_MAXATTRLEN */
    4,				/* GENDERS_DATABASE_CORNER_CASE_MAXVALLEN */
    {"node1", "node2", NULL},
    2,				
    {"attr1", "attr2", NULL},
    2,
    {NULL,    "val2", NULL},
    2,
    {
      {
	{"attr1", "attr2", NULL},
	{"", "val2", NULL}, 
	{NULL, "val2", NULL}, 
	2,
      },
      {
	{"attr1", "attr2", NULL},
	{"", "val2", NULL}, 
	{NULL, "val2", NULL}, 
	2,
      },
    },
    2,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	"val2",
	{"node1", "node2", NULL},
	2,
      },
    },
    4,
  };

genders_database_t genders_database_corner_case = 
  {
    "testdatabases/genders.corner_case",
    &genders_database_data_corner_case,
  };

genders_database_data_t genders_database_data_base = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    2,
    2,
    2,
    5,
    5,
    4,
    {"node1", "node2", NULL},
    2,				
    {"attr1", "attr2", NULL},
    2,
    {NULL,    "val2", NULL},
    2,
    {
      {
	{"attr1", "attr2", NULL},
	{"", "val2", NULL}, 
	{NULL, "val2", NULL}, 
	2,
      },
      {
	{"attr1", "attr2", NULL},
	{"", "val2", NULL}, 
	{NULL, "val2", NULL}, 
	2,
      },
    },
    2,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"",
	"",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	"val2",
	{"node1", "node2", NULL},
	2,
      },
    },
    5,
  };

genders_database_t genders_database_base = 
  {
    "testdatabases/genders.base",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comma =
  {
    "testdatabases/genders.base_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_hostrange =
  {
    "testdatabases/genders.base_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_hostrange_single =
  {
    "testdatabases/genders.base_hostrange_single",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_beginning_of_line =
  {
    "testdatabases/genders.base_comments_beginning_of_line",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_beginning_of_line_comma =
  {
    "testdatabases/genders.base_comments_beginning_of_line_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_beginning_of_line_hostrange =
  {
    "testdatabases/genders.base_comments_beginning_of_line_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_end_of_line =
  {
    "testdatabases/genders.base_comments_end_of_line",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_end_of_line_comma =
  {
    "testdatabases/genders.base_comments_end_of_line_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_end_of_line_hostrange =
  {
    "testdatabases/genders.base_comments_end_of_line_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_end_of_line_with_whitespace =
  {
    "testdatabases/genders.base_comments_end_of_line_with_whitespace",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_end_of_line_with_whitespace_comma =
  {
    "testdatabases/genders.base_comments_end_of_line_with_whitespace_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_end_of_line_with_whitespace_hostrange =
  {
    "testdatabases/genders.base_comments_end_of_line_with_whitespace_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_middle_of_line =
  {
    "testdatabases/genders.base_comments_middle_of_line",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_middle_of_line_comma =
  {
    "testdatabases/genders.base_comments_middle_of_line_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_comments_middle_of_line_hostrange =
  {
    "testdatabases/genders.base_comments_middle_of_line_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_before_nodes =
  {
    "testdatabases/genders.base_whitespace_before_nodes",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_before_nodes_comma =
  {
    "testdatabases/genders.base_whitespace_before_nodes_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_before_nodes_hostrange =
  {
    "testdatabases/genders.base_whitespace_before_nodes_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_after_nodes =
  {
    "testdatabases/genders.base_whitespace_after_nodes",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_after_nodes_comma =
  {
    "testdatabases/genders.base_whitespace_after_nodes_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_after_nodes_hostrange =
  {
    "testdatabases/genders.base_whitespace_after_nodes_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_after_attrs =
  {
    "testdatabases/genders.base_whitespace_after_attrs",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_after_attrs_comma =
  {
    "testdatabases/genders.base_whitespace_after_attrs_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_after_attrs_hostrange =
  {
    "testdatabases/genders.base_whitespace_after_attrs_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_before_and_after_nodes =
  {
    "testdatabases/genders.base_whitespace_before_and_after_nodes",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_before_and_after_nodes_comma =
  {
    "testdatabases/genders.base_whitespace_before_and_after_nodes_comma",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_before_and_after_nodes_hostrange =
  {
    "testdatabases/genders.base_whitespace_before_and_after_nodes_hostrange",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_between_nodes =
  {
    "testdatabases/genders.base_whitespace_between_nodes",
    &genders_database_data_base,
  };

genders_database_t genders_database_base_whitespace_between_nodes_and_attrs =
  {
    "testdatabases/genders.base_whitespace_between_nodes_and_attrs",
    &genders_database_data_base,
  };

genders_database_data_t genders_database_data_test_1 = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    4,
    4,
    4,
    5,
    5,
    4,
    {"node1", "node2", "node3", "node4", NULL},
    4,				
    {"attr1", "attr2", "attr3", "attr4", NULL},
    4,
    {NULL,    "val2",  NULL,    "val4",  NULL},
    4,
    {
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
    },
    4,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"",
	"",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2",
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2",
	"val2",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr3",
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr4",
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr4",
	"val4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
    },
    8,
  };

genders_database_t genders_database_test_1 =
  {
    "testdatabases/genders.test_1",
    &genders_database_data_test_1,
  };

genders_database_t genders_database_test_1_comma =
  {
    "testdatabases/genders.test_1_comma",
    &genders_database_data_test_1,
  };

genders_database_t genders_database_test_1_hostrange =
  {
    "testdatabases/genders.test_1_hostrange",
    &genders_database_data_test_1,
  };

genders_database_data_t genders_database_data_test_2 = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    4,
    8,
    4,
    5,
    5,
    4,
    {"node1", "node2", "node3", "node4", NULL},
    4,				
    {"attr1", "attr2", "attr3", "attr4", 
     "attr5", "attr6", "attr7", "attr8",
     NULL},
    8,
    {NULL,    "val2",  NULL,    "val4",  
     NULL,    "val6",  NULL,    "val8",  
     NULL},
    8,
    {
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
      {
	{"attr5", "attr6", "attr7", "attr8", NULL},
	{"", "val6", "", "val8", NULL}, 
	{NULL, "val6", NULL, "val8", NULL}, 
	4,
      },
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
      {
	{"attr5", "attr6", "attr7", "attr8", NULL},
	{"", "val6", "", "val8", NULL}, 
	{NULL, "val6", NULL, "val8", NULL}, 
	4,
      },
    },
    4,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"",
	"",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1",
	NULL,
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr2",
	NULL,
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr2",
	"val2",
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr3",
	NULL,
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr4",
	NULL,
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr4",
	"val4",
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr5",
	NULL,
	{"node2", "node4", NULL},
	2,
      },
      {
	"attr6",
	NULL,
	{"node2", "node4", NULL},
	2,
      },
      {
	"attr6",
	"val6",
	{"node2", "node4", NULL},
	2,
      },
      {
	"attr7",
	NULL,
	{"node2", "node4", NULL},
	2,
      },
      {
	"attr8",
	NULL,
	{"node2", "node4", NULL},
	2,
      },
      {
	"attr8",
	"val8",
	{"node2", "node4", NULL},
	2,
      },
    },
    14,
  };

genders_database_t genders_database_test_2 =
  {
    "testdatabases/genders.test_2",
    &genders_database_data_test_2,
  };

genders_database_t genders_database_test_2_comma =
  {
    "testdatabases/genders.test_2_comma",
    &genders_database_data_test_2,
  };

genders_database_t genders_database_test_2_hostrange =
  {
    "testdatabases/genders.test_2_hostrange",
    &genders_database_data_test_2,
  };

genders_database_data_t genders_database_data_test_3 = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    4,
    16,
    4,
    5,
    6,
    5,
    {"node1", "node2", "node3", "node4", NULL},
    4,				
    {"attr1",  "attr2",  "attr3",  "attr4", 
     "attr5",  "attr6",  "attr7",  "attr8",
     "attr9",  "attr10", "attr11", "attr12", 
     "attr13", "attr14", "attr15", "attr16",
     NULL},
    16,
    {NULL,    "val2",  NULL,    "val4",  
     NULL,    "val6",  NULL,    "val8",  
     NULL,    "val10",  NULL,    "val12",  
     NULL,    "val14",  NULL,    "val16",  
     NULL},
    16,
    {
      {
	{"attr1", "attr2", "attr3", "attr4", NULL},
	{"", "val2", "", "val4", NULL}, 
	{NULL, "val2", NULL, "val4", NULL}, 
	4,
      },
      {
	{"attr5", "attr6", "attr7", "attr8", NULL},
	{"", "val6", "", "val8", NULL}, 
	{NULL, "val6", NULL, "val8", NULL}, 
	4,
      },
      {
	{"attr9", "attr10", "attr11", "attr12", NULL},
	{"", "val10", "", "val12", NULL}, 
	{NULL, "val10", NULL, "val12", NULL}, 
	4,
      },
      {
	{"attr13", "attr14", "attr15", "attr16", NULL},
	{"", "val14", "", "val16", NULL}, 
	{NULL, "val14", NULL, "val16", NULL}, 
	4,
      },
    },
    4,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"",
	"",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"attr2",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"attr2",
	"val2",
	{"node1", NULL},
	1,
      },
      {
	"attr3",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"attr4",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"attr4",
	"val4",
	{"node1", NULL},
	1,
      },
      {
	"attr5",
	NULL,
	{"node2", NULL},
	1,
      },
      {
	"attr6",
	NULL,
	{"node2", NULL},
	1,
      },
      {
	"attr6",
	"val6",
	{"node2", NULL},
	1,
      },
      {
	"attr7",
	NULL,
	{"node2", NULL},
	1,
      },
      {
	"attr8",
	NULL,
	{"node2", NULL},
	1,
      },
      {
	"attr8",
	"val8",
	{"node2", NULL},
	1,
      },
      {
	"attr9",
	NULL,
	{"node3", NULL},
	1,
      },
      {
	"attr10",
	NULL,
	{"node3", NULL},
	1,
      },
      {
	"attr10",
	"val10",
	{"node3", NULL},
	1,
      },
      {
	"attr11",
	NULL,
	{"node3", NULL},
	1,
      },
      {
	"attr12",
	NULL,
	{"node3", NULL},
	1,
      },
      {
	"attr12",
	"val12",
	{"node3", NULL},
	1,
      },
      {
	"attr13",
	NULL,
	{"node4", NULL},
	1,
      },
      {
	"attr14",
	NULL,
	{"node4", NULL},
	1,
      },
      {
	"attr14",
	"val14",
	{"node4", NULL},
	1,
      },
      {
	"attr15",
	NULL,
	{"node4", NULL},
	1,
      },
      {
	"attr16",
	NULL,
	{"node4", NULL},
	1,
      },
      {
	"attr16",
	"val16",
	{"node4", NULL},
	1,
      },
    },
    26,
  };

genders_database_t genders_database_test_3 =
  {
    "testdatabases/genders.test_3",
    &genders_database_data_test_3,
  };

genders_database_data_t genders_database_data_empty = 
  {
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    0,
    0,
    0,
    0,
    0,
    {NULL},
    0,
    {NULL},
    0,
    {NULL},
    0,
    {
      {
	{NULL},
	{NULL},
	{NULL}
      }
    },
    0,
    {
      {
	NULL,
	NULL,
	{NULL},
	0,
      },
    },
    0,
  };

genders_database_t genders_database_empty =
  {
    "testdatabases/genders.empty_file",
    &genders_database_data_empty,
  };

genders_database_data_t genders_database_data_equal_sign_in_value = 
  {
    "node1",
    "attr2",
    "attr1",
    "foo=bar",
    2,
    2,
    2,
    5,
    5,
    7,
    {"node1", "node2"},
    2,
    {"attr1", "attr2"},
    2,
    {"foo=bar", NULL},
    2,
    {
      {
	{"attr1", NULL},
	{"foo=bar", NULL}, 
	{NULL, NULL}, 
	1,
      },
      {
	{"attr1", "attr2", NULL},
	{"foo=baz", "", NULL}, 
	{"foo=baz", NULL, NULL}, 
	2,
      },
    },
    2,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"",
	"",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	"foo=bar",
	{"node1", NULL},
	1,
      },

      {
	"attr1",
	"foo=baz",
	{"node2", NULL},
	1,
      },
      {
	"attr2",
	NULL,
	{"node2", NULL},
	1,
      },
    },
    6,
  };

genders_database_t genders_database_equal_sign_in_value =
  {
    "testdatabases/genders.equal_sign_in_value",
    &genders_database_data_equal_sign_in_value,
  };

genders_database_data_t genders_database_data_nodes_only_one = 
  {
    "node1",
    NULL,
    NULL,
    NULL,
    1,
    0,
    0,
    5,
    0,
    0,
    {"node1", NULL},
    1,				
    {NULL},
    0,
    {NULL},
    0,
    {
      {
	{NULL},
	{NULL}, 
	{NULL}, 
	0,
      },
    },
    0,
    {
      {
	NULL,
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"",
	"",
	{"node1", NULL},
	1,
      },
    },
    2,
  };

genders_database_t genders_database_nodes_only_one = 
  {
    "testdatabases/genders.nodes_only_one",
    &genders_database_data_nodes_only_one,
  };

genders_database_data_t genders_database_data_nodes_only_many = 
  {
    "node1",
    NULL,
    NULL,
    NULL,
    5,
    0,
    0,
    5,
    0,
    0,
    {"node1", "node2", "node3", "node4", "node5", NULL},
    5,				
    {NULL},
    0,
    {NULL},
    0,
    {
      {
	{NULL},
	{NULL}, 
	{NULL}, 
	0,
      },
    },
    0,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", "node3", "node4", "node5", NULL},
	5,
      },
      {
	"",
	"",
	{"node1", "node2", "node3", "node4", "node5", NULL},
	5,
      },
    },
    2,
  };

genders_database_t genders_database_nodes_only_many = 
  {
    "testdatabases/genders.nodes_only_many",
    &genders_database_data_nodes_only_many,
  };

genders_database_data_t genders_database_data_nodes_and_attrs_only = 
  {
    "node1",
    "attr1",
    NULL,
    NULL,
    2,
    2,
    2,
    5,
    5,
    0,
    {"node1", "node2", NULL},
    2,				
    {"attr1", "attr2", NULL},
    2,
    {NULL},
    0,
    {
      {
	{"attr1", "attr2", NULL},
	{"", "", NULL}, 
	{NULL, NULL, NULL}, 
	2,
      },
      {
	{"attr1", "attr2", NULL},
	{"", "", NULL}, 
	{NULL, NULL, NULL}, 
	2,
      },
    },
    2,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"",
	"",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
    },
    4,
  };

genders_database_t genders_database_nodes_and_attrs_only = 
  {
    "testdatabases/genders.nodes_and_attrs_only",
    &genders_database_data_nodes_and_attrs_only,
  };

genders_database_t genders_database_nodes_and_attrs_only_comma = 
  {
    "testdatabases/genders.nodes_and_attrs_only_comma",
    &genders_database_data_nodes_and_attrs_only,
  };

genders_database_t genders_database_nodes_and_attrs_only_hostrange = 
  {
    "testdatabases/genders.nodes_and_attrs_only_hostrange",
    &genders_database_data_nodes_and_attrs_only,
  };

genders_database_data_t genders_database_data_subst_escape_char = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    2,
    5,
    5,
    5,
    7,
    4,
    {"node1", "node2", NULL},
    2,				
    {"attr1", "attr2", "escape1", "escape2", "escape3", NULL},
    5,
    {NULL,    "val2", "%t", "%t", "%n", NULL},
    5,
    {
      {
	{"attr1", "attr2", "escape1", "escape2", "escape3", NULL},
	{"", "val2", "%t", "%t", "%n", NULL}, 
	{NULL, "val2", "%t", "%t", "%n", NULL}, 
	5,
      },
      {
	{"attr1", "attr2", "escape1", "escape2", "escape3", NULL},
	{"", "val2", "%t", "%t", "%n", NULL}, 
	{NULL, "val2", "%t", "%t", "%n", NULL}, 
	5,
      },
    },
    2,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"",
	"",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	"val2",
	{"node1", "node2", NULL},
	2,
      },
      {
	"escape1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"escape1",
	"%t",
	{"node1", "node2", NULL},
	2,
      },
      {
	"escape2",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"escape2",
	"%t",
	{"node1", "node2", NULL},
	2,
      },
      {
	"escape3",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"escape3",
	"%n",
	{"node1", "node2", NULL},
	2,
      },
    },
    11,
  };

genders_database_t genders_database_subst_escape_char = 
  {
    "testdatabases/genders.subst_escape_char",
    &genders_database_data_subst_escape_char,
  };

genders_database_data_t genders_database_data_subst_nodename = 
  {
    "node1",
    "attr1",
    "attr2",
    "val2",
    2,
    3,
    3,
    5,
    5,
    5,
    {"node1", "node2", NULL},
    2,				
    {"attr1", "attr2", "attr3", NULL},
    3,
    /* Can't test "%n", so make NULL so test is not run */
    {NULL,    "val2",  NULL,    NULL}, 
    3,
    {
      {
	{"attr1", "attr2", "attr3", NULL},
	{"", "val2", "node1", NULL}, 
	{NULL, "val2", "node1", NULL}, 
	3,
      },
      {
	{"attr1", "attr2", "attr3", NULL},
	{"", "val2", "node2", NULL}, 
	{NULL, "val2", "node2", NULL}, 
	3,
      },
    },
    2,
    {
      {
	NULL,
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"",
	"",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2",
	"val2",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr3",
	NULL,
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr3",
	"node1",
	{"node1", NULL},
	1,
      },
      {
	"attr3",
	"node2",
	{"node2", NULL},
	1,
      },
    },
    8,
  };

genders_database_t genders_database_subst_nodename = 
  {
    "testdatabases/genders.subst_nodename",
    &genders_database_data_subst_nodename,
  };

genders_database_t genders_database_subst_nodename_comma = 
  {
    "testdatabases/genders.subst_nodename_comma",
    &genders_database_data_subst_nodename,
  };

genders_database_t genders_database_subst_nodename_hostrange = 
  {
    "testdatabases/genders.subst_nodename_hostrange",
    &genders_database_data_subst_nodename,
  };

genders_database_data_t genders_database_data_large_attr = 
  {
    "node1",
    "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
    NULL,
    NULL,
    1,
    1,
    1,
    5,
    78,
    0,
    {"node1", NULL},
    1,				
    {"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL},
    1,
    {NULL, NULL},
    1,
    {
      {
	{"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL},
	{"", NULL}, 
	{NULL, NULL}, 
	1,
      },
    },
    1,
    {
      {
	NULL,
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"",
	"",
	{"node1", NULL},
	1,
      },
      {
	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
	NULL,
	{"node1", NULL},
	1,
      },
    },
    3,
  };

genders_database_t genders_database_large_attr = 
  {
    "testdatabases/genders.large_attr",
    &genders_database_data_large_attr,
  };

genders_database_data_t genders_database_data_large_val = 
  {
    "node1",
    NULL,
    "attr1",
    "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
    1,
    1,
    1,
    5,
    5,
    78,
    {"node1", NULL},
    1,				
    {"attr1", NULL},
    1,
    {"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL},
    1,
    {
      {
	{"attr1", NULL}, 
	{"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL},
	{"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL},
	1,
      },
    },
    1,
    {
      {
	NULL,
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"",
	"",
	{"node1", NULL},
	1,
      },
      {
	"attr1",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"attr1",
	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
	{"node1", NULL},
	1,
      },
    },
    4,
  };

genders_database_t genders_database_large_val = 
  {
    "testdatabases/genders.large_val",
    &genders_database_data_large_val,
  };

genders_database_t *genders_functionality_databases[] = 
  {
    &genders_database_base,
    &genders_database_base_comma,
    &genders_database_base_hostrange,
    &genders_database_base_hostrange_single,
    &genders_database_base_comments_beginning_of_line,
    &genders_database_base_comments_beginning_of_line_comma,
    &genders_database_base_comments_beginning_of_line_hostrange,
    &genders_database_base_comments_end_of_line,
    &genders_database_base_comments_end_of_line_comma,
    &genders_database_base_comments_end_of_line_hostrange,
    &genders_database_base_comments_end_of_line_with_whitespace,
    &genders_database_base_comments_end_of_line_with_whitespace_comma,
    &genders_database_base_comments_end_of_line_with_whitespace_hostrange,
    &genders_database_base_comments_middle_of_line,
    &genders_database_base_comments_middle_of_line_comma,
    &genders_database_base_comments_middle_of_line_hostrange,
    &genders_database_base_whitespace_before_nodes,
    &genders_database_base_whitespace_before_nodes_comma,
    &genders_database_base_whitespace_before_nodes_hostrange,
    &genders_database_base_whitespace_after_nodes,
    &genders_database_base_whitespace_after_nodes_comma,
    &genders_database_base_whitespace_after_nodes_hostrange,
    &genders_database_base_whitespace_after_attrs,
    &genders_database_base_whitespace_after_attrs_comma,
    &genders_database_base_whitespace_after_attrs_hostrange,
    &genders_database_base_whitespace_before_and_after_nodes,
    &genders_database_base_whitespace_before_and_after_nodes_comma,
    &genders_database_base_whitespace_before_and_after_nodes_hostrange,
    &genders_database_base_whitespace_between_nodes,
    &genders_database_base_whitespace_between_nodes_and_attrs,
    &genders_database_test_1,
    &genders_database_test_1_comma,
    &genders_database_test_1_hostrange,
    &genders_database_test_2,
    &genders_database_test_2_comma,
    &genders_database_test_2_hostrange,
    &genders_database_test_3,
    &genders_database_empty,
    &genders_database_equal_sign_in_value,
    &genders_database_nodes_only_one,
    &genders_database_nodes_only_many,
    &genders_database_nodes_and_attrs_only,
    &genders_database_nodes_and_attrs_only_comma,
    &genders_database_nodes_and_attrs_only_hostrange,
    &genders_database_subst_escape_char,
    &genders_database_subst_nodename,
    &genders_database_subst_nodename_comma,
    &genders_database_subst_nodename_hostrange,
    &genders_database_large_attr,
    &genders_database_large_val,
    NULL,
  };

genders_database_data_t genders_database_data_flag_test_raw_values = 
  {
    "node1",
    NULL,			
    "escape1",			
    "%t",			
    1,
    4,
    4,
    5,
    7,
    5,
    {"node1", NULL},
    1,				
    {"escape1", "escape2", "escape3", "escape4", NULL},
    4,
    {"%t", "%t", "%n", "node1", NULL},
    4,
    {
      {
	{"escape1", "escape2", "escape3", "escape4", NULL},
	{"%t", "%t", "%n", "node1", NULL}, /* cheat, these are the substituted values */
	{"%t", "%%t", "%%n", "%n", NULL}, /* cheat, these are "raw" values */
	4,
      },
    },
    1,
    {
      {
	NULL,
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"",
	"",
	{"node1", NULL},
	1,
      },
      {
	"escape1",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"escape1",
	"%t",
	{"node1", NULL},
	1,
      },
      {
	"escape2",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"escape2",
	"%t",
	{"node1", NULL},
	1,
      },
      {
	"escape3",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"escape3",
	"%n",
	{"node1", NULL},
	1,
      },
      {
	"escape4",
	NULL,
	{"node1", NULL},
	1,
      },
      {
	"escape4",
	"node1",
	{"node1", NULL},
	1,
      },
    },
    10,
  };

genders_database_t genders_database_flag_test_raw_values = 
  {
    "testdatabases/genders.flag_test_raw_values",
    &genders_database_data_flag_test_raw_values,
  };
