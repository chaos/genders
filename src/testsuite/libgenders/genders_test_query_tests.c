/*****************************************************************************\
 *  $Id: genders_test_query_tests.c,v 1.9 2010-03-24 21:26:05 chu11 Exp $
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

#include "genders_test_query_tests.h"

char *genders_query_parse_error_tests[] = 
  {
    "~",
    "(",
    ")",
    "attr1 &&",
    "&& attr1",
    "attr1 ||",
    "|| attr1",
    "attr1 --",
    "-- attr1",
    "(attr1",
    "attr1)",
    "((attr1)",
    "(attr1))",
    "      ",
    NULL,
  };

genders_query_tests_t genders_query_functionality_tests_query_1_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"attr1",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr2=val2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr3",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr4=val4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr5",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr6=val6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr7",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr8=val8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr9",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr10=val10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      /* 
       * Empty sets
       */
      {
	"fakeattr",
	{NULL},
	0,
      },
      {
	"attr1=fakeval",
	{NULL},
	0,
      },
      {
	"attr2=fakeval",
	{NULL},
	0,
      },
      {
	"attr3&&attr5",
	{NULL},
	0,
      },
      {
	"attr4&&attr6",
	{NULL},
	0,
      },
      {
	"attr4=val4&&attr6=val6",
	{NULL},
	0,
      },
      {
	"attr7&&attr9",
	{NULL},
	0,
      },
      {
	"attr8&&attr10",
	{NULL},
	0,
      },
      {
	"attr8=val8&&attr10=val10",
	{NULL},
	0,
      },
      {
	"(attr1&&attr3)--(attr1&&attr4=val4)",
	{NULL},
	0,
      },
      {
	"(attr2&&attr3)--(attr2&&attr4=val4)",
	{NULL},
	0,
      },
      {
	"(attr2=val2&&attr3)--(attr2=val2&&attr4=val4)",
	{NULL},
	0,
      },
      {
	"(attr1&&attr5)--(attr1&&attr6=val6)",
	{NULL},
	0,
      },
      {
	"(attr2&&attr5)--(attr2&&attr6=val6)",
	{NULL},
	0,
      },
      {
	"(attr2=val2&&attr5)--(attr2=val2&&attr6=val6)",
	{NULL},
	0,
      },
      {
	"(attr1&&attr7)--(attr1&&attr8=val8)",
	{NULL},
	0,
      },
      {
	"(attr2&&attr7)--(attr2&&attr8=val8)",
	{NULL},
	0,
      },
      {
	"(attr2=val2&&attr7)--(attr2=val2&&attr8=val8)",
	{NULL},
	0,
      },
      {
	"(attr1&&attr9)--(attr1&&attr10=val10)",
	{NULL},
	0,
      },
      {
	"(attr2&&attr9)--(attr2&&attr10=val10)",
	{NULL},
	0,
      },
      {
	"(attr2=val2&&attr9)--(attr2=val2&&attr10=val10)",
	{NULL},
	0,
      },
      {
	"~attr1",
	{NULL},
	0,
      },
      {
	"~attr2",
	{NULL},
	0,
      },
      {
	"~attr2=val2",
	{NULL},
	0,
      },

      /* 
       * Complex tests - all nodes 
       */
      /* binary operations */
      {
	"attr1||attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2=val2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr3||attr5",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr4||attr6",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr4=val4||attr6=val6",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr7||attr9",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr8||attr10",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr8=val8||attr10=val10",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2=val2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      /* binary operations and complements */
      {
	"~(attr3&&attr5)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr4&&attr6)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr4=val4&&attr6=val6)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr7&&attr9)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr8&&attr10)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr8=val8&&attr10=val10)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      /* mixed operations */
      {
	"(attr1&&attr3)||(attr1&&attr5)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr1&&attr4=val4)||(attr1&&attr6=val6)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr1&&attr7)||(attr1&&attr9)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr1&&attr8=val8)||(attr1&&attr10=val10)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2&&attr3)||(attr2&&attr5)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2&&attr4=val4)||(attr2&&attr6=val6)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2&&attr7)||(attr2&&attr9)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2&&attr8=val8)||(attr2&&attr10=val10)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2=val2&&attr3)||(attr2=val2&&attr5)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2=val2&&attr4=val4)||(attr2=val2&&attr6=val6)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2=val2&&attr7)||(attr2=val2&&attr9)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"(attr2=val2&&attr8=val8)||(attr2=val2&&attr10=val10)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      /* 
       * Complex tests - half nodes 
       */
      {
	"attr1&&attr3",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2&&attr3",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=val2&&attr3",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&attr4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2&&attr4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=val2&&attr4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&attr4=val4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2&&attr4=val4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=val2&&attr4=val4",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1--attr5",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1--attr6",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1--attr6=val6",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2--attr5",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2--attr6",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2--attr6=val6",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=val2--attr5",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=val2--attr6",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=val2--attr6=val6",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&attr5",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2&&attr5",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2=val2&&attr5",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&attr6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2&&attr6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2=val2&&attr6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&attr6=val6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2&&attr6=val6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2=val2&&attr6=val6",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--attr3",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--attr4",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--attr4=val4",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2--attr3",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2--attr4",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2--attr4=val4",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2=val2--attr3",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2=val2--attr4",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr2=val2--attr4=val4",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&attr7",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2&&attr7",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2=val2&&attr7",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1&&attr8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2&&attr8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2=val2&&attr8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1&&attr8=val8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2&&attr8=val8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2=val2&&attr8=val8",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1--attr9",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1--attr10",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1--attr10=val10",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2--attr9",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2--attr10",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2--attr10=val10",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2=val2--attr9",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2=val2--attr10",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr2=val2--attr10=val10",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1&&attr9",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2&&attr9",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2=val2&&attr9",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1&&attr10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2&&attr10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2=val2&&attr10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1&&attr10=val10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2&&attr10=val10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2=val2&&attr10=val10",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      }, 
      {
	"attr1--attr7",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1--attr8",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1--attr8=val8",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2--attr7",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2--attr8",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2--attr8=val8",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2=val2--attr7",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2=val2--attr8",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr2=val2--attr8=val8",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      /* complementing empty sets */
      {
	"~fakeattr",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~attr1=fakeval",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~attr2=fakeval",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr1)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr2)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr2=val2)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--((attr1&&attr3)||(attr1&&attr5)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--((attr1&&attr4=val4)||(attr1&&attr6=val6)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--((attr1&&attr7)||(attr1&&attr9)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--((attr1&&attr8=val8)||(attr1&&attr10=val10)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--((attr2&&attr3)||(attr2&&attr5)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--((attr2&&attr4=val4)||(attr2&&attr6=val6)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--((attr2&&attr7)||(attr2&&attr9)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--((attr2&&attr8=val8)||(attr2&&attr10=val10)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2=val2--((attr2=val2&&attr3)||(attr2=val2&&attr5)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2=val2--((attr2=val2&&attr4=val4)||(attr2=val2&&attr6=val6)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2=val2--((attr2=val2&&attr7)||(attr2=val2&&attr9)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2=val2--((attr2=val2&&attr8=val8)||(attr2=val2&&attr10=val10)))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      /* double negation */
      {
	"~(~(attr1))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~(attr2))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~(attr2=val2))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_1 = 
  {
    "testdatabases/genders.query_1",
    &genders_query_functionality_tests_query_1_tests,
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_1_comma = 
  {
    "testdatabases/genders.query_1_comma",
    &genders_query_functionality_tests_query_1_tests,
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_1_hostrange = 
  {
    "testdatabases/genders.query_1_hostrange",
    &genders_query_functionality_tests_query_1_tests,
  };

genders_query_tests_t genders_query_functionality_tests_query_2_tests =
  {
    { 
      /* 
       * Simple tests 
       */
      {
	"attr1",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr2=valB",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr2=valC",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr3=valD",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr3=valE",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr3=valF",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr3=valG",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr4=valH",
	{"node1", NULL},
	1,
      },
      {
	"attr4=valI",
	{"node2", NULL},
	1,
      },
      {
	"attr4=valJ",
	{"node3", NULL},
	1,
      },
      {
	"attr4=valK",
	{"node4", NULL},
	1,
      },
      {
	"attr4=valL",
	{"node5", NULL},
	1,
      },
      {
	"attr4=valM",
	{"node6", NULL},
	1,
      },
      {
	"attr4=valN",
	{"node7", NULL},
	1,
      },
      {
	"attr4=valO",
	{"node8", NULL},
	1,
      },
      /* 
       * Empty sets
       */
      {
	"fakeattr",
	{NULL},
	0,
      },
      {
	"attr1=fakeval",
	{NULL},
	0,
      },
      {
	"attr2=valB&&attr2=valC",
	{NULL},
	0,
      },
      {
	"attr3=valD&&attr3=valE&&attr3=valF&&attr3=valG",
	{NULL},
	0,
      },
      {
	"attr4=valH&&attr4=valI&&attr4=valJ&&attr4=valK&&attr4=valL&&attr4=valM&&attr4=valN&&attr4=valO",
	{NULL},
	0,
      },
      {
	"attr1--attr1",
	{NULL},
	0,
      },
      {
	"attr1--attr2",
	{NULL},
	0,
      },
      {
	"attr1--attr3",
	{NULL},
	0,
      },
      {
	"attr1--attr4",
	{NULL},
	0,
      },
      {
	"attr1=valA--attr2",
	{NULL},
	0,
      },
      {
	"attr1=valA--attr3",
	{NULL},
	0,
      },
      {
	"attr1=valA--attr4",
	{NULL},
	0,
      },
      {
	"attr1--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"attr1--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"attr1--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"attr1=valA--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"attr1=valA--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"attr1=valA--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"attr2--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"attr2--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"attr2--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"(attr2=valB||attr2=valC)--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"(attr2=valB||attr2=valC)--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"(attr2=valB||attr2=valC)--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"attr3--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"attr3--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"attr3--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"(attr3=valD||attr3=valE||attr3=valF||attr3=valG)--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"(attr3=valD||attr3=valE||attr3=valF||attr3=valG)--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"(attr3=valD||attr3=valE||attr3=valF||attr3=valG)--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"attr4--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"attr4--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"attr4--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)--(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)--(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      {
	"~attr1",
	{NULL},
	0,
      },
      {
	"~attr1=valA",
	{NULL},
	0,
      },
      {
	"~attr2",
	{NULL},
	0,
      },
      {
	"~(attr2=valB||attr2=valC)",
	{NULL},
	0,
      },
      {
	"~attr3",
	{NULL},
	0,
      },
      {
	"~(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{NULL},
	0,
      },
      {
	"~attr4",
	{NULL},
	0,
      },
      {
	"~(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{NULL},
	0,
      },
      /* 
       * Complex tests - all nodes 
       */
      {
	"attr2=valB||attr2=valC",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr3=valD||attr3=valE||attr3=valF||attr3=valG",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2&&attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2&&attr3&&attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2||attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2||attr3||attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA&&attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA&&attr2&&attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA&&attr2&&attr3&&attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA||attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA||attr2||attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1=valA||attr2||attr3||attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&(attr2=valB||attr2=valC)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&(attr2=valB||attr2=valC)&&attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&(attr2=valB||attr2=valC)&&attr3&&attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||(attr2=valB||attr2=valC)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||(attr2=valB||attr2=valC)||attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||(attr2=valB||attr2=valC)||attr3||attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2&&(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2&&(attr3=valD||attr3=valE||attr3=valF||attr3=valG)&&attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2||(attr3=valD||attr3=valE||attr3=valF||attr3=valG)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2||(attr3=valD||attr3=valE||attr3=valF||attr3=valG)||attr4",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2&&attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1&&attr2&&attr3&&(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2||attr3",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"attr1||attr2||attr3||(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      /* all nodes - negations */
      {
	"~(fakeattr)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=fakeval)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2=valB&&attr2=valC)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr3=valD&&attr3=valE&&attr3=valF&&attr3=valG)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr4=valH&&attr4=valI&&attr4=valJ&&attr4=valK&&attr4=valL&&attr4=valM&&attr4=valN&&attr4=valO)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr1)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr2)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr3)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--attr4)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=valA--attr2)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=valA--attr3)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=valA--attr4)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=valA--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=valA--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr1=valA--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr2--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr2=valB||attr2=valC)--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr2=valB||attr2=valC)--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr2=valB||attr2=valC)--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr3--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr3--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr3--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr3=valD||attr3=valE||attr3=valF||attr3=valG)--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr3=valD||attr3=valE||attr3=valF||attr3=valG)--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr3=valD||attr3=valE||attr3=valF||attr3=valG)--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr4--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr4--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(attr4--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)--(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)--(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~((attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO)--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~attr1)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~attr1=valA)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~attr2)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~(attr2=valB||attr2=valC))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~attr3)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~(attr3=valD||attr3=valE||attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~attr4)",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      {
	"~(~(attr4=valH||attr4=valI||attr4=valJ||attr4=valK||attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", NULL},
	8,
      },
      /* 
       * Complex tests - half nodes 
       */
      {
	"attr3=valD||attr3=valE",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr4=valH||attr4=valI||attr4=valJ||attr4=valK",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"~(attr2=valC)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"~(attr3=valF||attr3=valG)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"~(attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&attr2=valB",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&(attr3=valD||attr3=valE)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&(attr4=valH||attr4=valI||attr4=valJ||attr4=valK)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA&&attr2=valB",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA&&(attr3=valD||attr3=valE)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA&&(attr4=valH||attr4=valI||attr4=valJ||attr4=valK)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&(~(attr2=valC))",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&(~(attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1&&(~(attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr2=valC))",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr3=valF||attr3=valG))",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr4=valL||attr4=valM||attr4=valN||attr4=valO))",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1--(attr2=valC)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1--(attr3=valF||attr3=valG)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1--(attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA--(attr2=valC)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA--(attr3=valF||attr3=valG)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr1=valA--(attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node1", "node2", "node3", "node4", NULL},
	4,
      },
      {
	"attr3=valF||attr3=valG",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr4=valL||attr4=valM||attr4=valN||attr4=valO",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"~(attr2=valB)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"~(attr3=valD||attr3=valE)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"~(attr4=valH||attr4=valI||attr4=valJ||attr4=valK)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&attr2=valC",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(attr3=valF||attr3=valG)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&attr2=valC",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(attr3=valF||attr3=valG)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(attr4=valL||attr4=valM||attr4=valN||attr4=valO)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(~(attr2=valB))",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(~(attr3=valD||attr3=valE))",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(~(attr4=valH||attr4=valI||attr4=valJ||attr4=valK))",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr2=valB))",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr3=valD||attr3=valE))",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr4=valH||attr4=valI||attr4=valJ||attr4=valK))",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--(attr2=valB)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--(attr3=valD||attr3=valE)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA--(attr2=valB)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA--(attr3=valD||attr3=valE)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA--(attr4=valH||attr4=valI||attr4=valJ||attr4=valK)",
	{"node5", "node6", "node7", "node8", NULL},
	4,
      },
      {
	"attr3=valE||attr3=valF",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr4=valJ||attr4=valK||attr4=valL||attr4=valM",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"~(attr3=valD||attr3=valG)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"~(attr4=valH||attr4=valI||attr4=valN||attr4=valO)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1&&(attr3=valE||attr3=valF)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1&&(attr4=valJ||attr4=valK||attr4=valL||attr4=valM)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1=valA&&(attr3=valE||attr3=valF)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1=valA&&(attr4=valJ||attr4=valK||attr4=valL||attr4=valM)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1&&(~(attr3=valD||attr3=valG))",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1&&(~(attr4=valH||attr4=valI||attr4=valN||attr4=valO))",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr3=valD||attr3=valG))",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr4=valH||attr4=valI||attr4=valN||attr4=valO))",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1--(attr3=valD||attr3=valG)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1--(attr4=valH||attr4=valI||attr4=valN||attr4=valO)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1=valA--(attr3=valD||attr3=valG)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr1=valA--(attr4=valH||attr4=valI||attr4=valN||attr4=valO)",
	{"node3", "node4", "node5", "node6", NULL},
	4,
      },
      {
	"attr3=valD||attr3=valG",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr4=valH||attr4=valI||attr4=valN||attr4=valO",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"~(attr3=valE||attr3=valF)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"~(attr4=valJ||attr4=valK||attr4=valL||attr4=valM)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(attr3=valD||attr3=valG)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(attr4=valH||attr4=valI||attr4=valN||attr4=valO)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(attr3=valD||attr3=valG)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(attr4=valH||attr4=valI||attr4=valN||attr4=valO)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(~(attr3=valE||attr3=valF))",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1&&(~(attr4=valJ||attr4=valK||attr4=valL||attr4=valM))",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr3=valE||attr3=valF))",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr4=valJ||attr4=valK||attr4=valL||attr4=valM))",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--(attr3=valE||attr3=valF)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1--(attr4=valJ||attr4=valK||attr4=valL||attr4=valM)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA--(attr3=valE||attr3=valF)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr1=valA--(attr4=valJ||attr4=valK||attr4=valL||attr4=valM)",
	{"node1", "node2", "node7", "node8", NULL},
	4,
      },
      {
	"attr4=valH||attr4=valJ||attr4=valL||attr4=valN",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"~(attr4=valI||attr4=valK||attr4=valM||attr4=valO)",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1&&(attr4=valH||attr4=valJ||attr4=valL||attr4=valN)",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1=valA&&(attr4=valH||attr4=valJ||attr4=valL||attr4=valN)",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1&&(~(attr4=valI||attr4=valK||attr4=valM||attr4=valO))",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr4=valI||attr4=valK||attr4=valM||attr4=valO))",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1--(attr4=valI||attr4=valK||attr4=valM||attr4=valO)",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr1=valA--(attr4=valI||attr4=valK||attr4=valM||attr4=valO)",
	{"node1", "node3", "node5", "node7", NULL},
	4,
      },
      {
	"attr4=valI||attr4=valK||attr4=valM||attr4=valO",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"~(attr4=valH||attr4=valJ||attr4=valL||attr4=valN)",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1&&(attr4=valI||attr4=valK||attr4=valM||attr4=valO)",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(attr4=valI||attr4=valK||attr4=valM||attr4=valO)",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1&&(~(attr4=valH||attr4=valJ||attr4=valL||attr4=valN))",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1=valA&&(~(attr4=valH||attr4=valJ||attr4=valL||attr4=valN))",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1--(attr4=valH||attr4=valJ||attr4=valL||attr4=valN)",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      {
	"attr1=valA--(attr4=valH||attr4=valJ||attr4=valL||attr4=valN)",
	{"node2", "node4", "node6", "node8", NULL},
	4,
      },
      /* 
       * Complex tests - quarter nodes 
       */
      {
	"attr1&&attr3=valD",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1=valA&&attr3=valD",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2&&attr3=valD",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2=valB&&attr3=valD",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1&&(attr4=valH||attr4=valI)",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1=valA&&(attr4=valH||attr4=valI)",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2&&(attr4=valH||attr4=valI)",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2=valB&&(attr4=valH||attr4=valI)",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2=valB--attr3=valE",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr2=valB--(attr4=valJ||attr4=valK)",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr1&&attr3=valE",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr1=valA&&attr3=valE",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr2&&attr3=valE",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr2=valB&&attr3=valE",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr1&&(attr4=valJ||attr4=valK)",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr1=valA&&(attr4=valJ||attr4=valK)",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr2&&(attr4=valJ||attr4=valK)",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr2=valB&&(attr4=valJ||attr4=valK)",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr2=valB--attr3=valD",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr2=valB--(attr4=valH||attr4=valI)",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr1&&attr3=valF",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr1=valA&&attr3=valF",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr2&&attr3=valF",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr2=valC&&attr3=valF",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr1&&(attr4=valL||attr4=valM)",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr1=valA&&(attr4=valL||attr4=valM)",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr2&&(attr4=valL||attr4=valM)",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr2=valC&&(attr4=valL||attr4=valM)",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr2=valC--attr3=valG",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr2=valC--(attr4=valN||attr4=valO)",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr1&&attr3=valG",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr1=valA&&attr3=valG",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr2&&attr3=valG",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr2=valC&&attr3=valG",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr1&&(attr4=valN||attr4=valO)",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr1=valA&&(attr4=valN||attr4=valO)",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr2&&(attr4=valN||attr4=valO)",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr2=valC&&(attr4=valN||attr4=valO)",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr2=valC--attr3=valF",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr2=valC--(attr4=valL||attr4=valM)",
	{"node7", "node8", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valI",
	{"node1", "node2", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valJ",
	{"node1", "node3", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valK",
	{"node1", "node4", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valL",
	{"node1", "node5", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valM",
	{"node1", "node6", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valN",
	{"node1", "node7", NULL},
	2,
      },
      {
	"attr4=valH||attr4=valO",
	{"node1", "node8", NULL},
	2,
      },
      {
	"attr4=valI||attr4=valJ",
	{"node2", "node3", NULL},
	2,
      },
      {
	"attr4=valI||attr4=valK",
	{"node2", "node4", NULL},
	2,
      },
      {
	"attr4=valI||attr4=valL",
	{"node2", "node5", NULL},
	2,
      },
      {
	"attr4=valI||attr4=valM",
	{"node2", "node6", NULL},
	2,
      },
      {
	"attr4=valI||attr4=valN",
	{"node2", "node7", NULL},
	2,
      },
      {
	"attr4=valI||attr4=valO",
	{"node2", "node8", NULL},
	2,
      },
      {
	"attr4=valJ||attr4=valK",
	{"node3", "node4", NULL},
	2,
      },
      {
	"attr4=valJ||attr4=valL",
	{"node3", "node5", NULL},
	2,
      },
      {
	"attr4=valJ||attr4=valM",
	{"node3", "node6", NULL},
	2,
      },
      {
	"attr4=valJ||attr4=valN",
	{"node3", "node7", NULL},
	2,
      },
      {
	"attr4=valJ||attr4=valO",
	{"node3", "node8", NULL},
	2,
      },
      {
	"attr4=valK||attr4=valL",
	{"node4", "node5", NULL},
	2,
      },
      {
	"attr4=valK||attr4=valM",
	{"node4", "node6", NULL},
	2,
      },
      {
	"attr4=valK||attr4=valN",
	{"node4", "node7", NULL},
	2,
      },
      {
	"attr4=valK||attr4=valO",
	{"node4", "node8", NULL},
	2,
      },
      {
	"attr4=valL||attr4=valM",
	{"node5", "node6", NULL},
	2,
      },
      {
	"attr4=valL||attr4=valN",
	{"node5", "node7", NULL},
	2,
      },
      {
	"attr4=valL||attr4=valO",
	{"node5", "node8", NULL},
	2,
      },
      {
	"attr4=valM||attr4=valN",
	{"node6", "node7", NULL},
	2,
      },
      {
	"attr4=valM||attr4=valO",
	{"node6", "node8", NULL},
	2,
      },
      {
	"attr4=valN||attr4=valO",
	{"node7", "node8", NULL},
	2,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_2 = 
  {
    "testdatabases/genders.query_2",
    &genders_query_functionality_tests_query_2_tests,
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_2_comma = 
  {
    "testdatabases/genders.query_2_comma",
    &genders_query_functionality_tests_query_2_tests,
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_2_hostrange = 
  {
    "testdatabases/genders.query_2_hostrange",
    &genders_query_functionality_tests_query_2_tests,
  };

genders_query_tests_t genders_query_functionality_tests_query_special_chars_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"attr%percent",
	{"node1", NULL},
	1,
      },
      {
	"attr|pipe",
	{"node1", NULL},
	1,
      },
      {
	"attr&ampersand",
	{"node1", NULL},
	1,
      },
      {
	"attr-minus",
	{"node1", NULL},
	1,
      },
      {
	"attr:colon",
	{"node1", NULL},
	1,
      },
      {
	"attr\\backslash",
	{"node1", NULL},
	1,
      },
      {
	"attr/forwardslash",
	{"node1", NULL},
	1,
      },
      {
	"attr%foo%percent",
	{"node2", NULL},
	1,
      },
      {
	"attr|foo|pipe",
	{"node2", NULL},
	1,
      },
      {
	"attr&foo&ampersand",
	{"node2", NULL},
	1,
      },
      {
	"attr-foo-minus",
	{"node2", NULL},
	1,
      },
      {
	"attr:foo:colon",
	{"node2", NULL},
	1,
      },
      {
	"attr\\foo\\backslash",
	{"node2", NULL},
	1,
      },
      {
	"attr/foo/forwardslash",
	{"node2", NULL},
	1,
      },
      {
	"attr1=attr1%percent",
	{"node3", NULL},
	1,
      },
      {
	"attr2=attr2|pipe",
	{"node3", NULL},
	1,
      },
      {
	"attr3=attr3&ampersand",
	{"node3", NULL},
	1,
      },
      {
	"attr4=attr4-minus",
	{"node3", NULL},
	1,
      },
      {
	"attr5=attr5:colon",
	{"node3", NULL},
	1,
      },
      {
	"attr6=attr6\\backslash",
	{"node3", NULL},
	1,
      },
      {
	"attr7=attr7/forwardslash",
	{"node3", NULL},
	1,
      },
      {
	"attr1+plus",
	{"node4", NULL},
	1,
      },
      {
	"attr2+foo+plus",
	{"node4", NULL},
	1,
      },
      {
	"attr3=val3+plus",
	{"node4", NULL},
	1,
      },
      {
	"attr4=val4+foo+plus",
	{"node4", NULL},
	1,
      },

      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_query_special_chars = 
  {
    "testdatabases/genders.query_special_chars",
    &genders_query_functionality_tests_query_special_chars_tests,
  };

genders_query_tests_t genders_query_functionality_tests_bugzilla414_1_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"a&&b",
	{"tst", NULL},
	1,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_bugzilla414_1 =
  {
    "testdatabases/genders.bugzilla414_1",
    &genders_query_functionality_tests_bugzilla414_1_tests,
  };

genders_query_tests_t genders_query_functionality_tests_bugzilla414_2_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"~b",
	{"tst0", NULL},
	1,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_bugzilla414_2 =
  {
    "testdatabases/genders.bugzilla414_2",
    &genders_query_functionality_tests_bugzilla414_2_tests,
  };

genders_query_tests_t genders_query_functionality_tests_bugzilla414_3_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"~b",
	{"tst0", NULL},
	1,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_bugzilla414_3 =
  {
    "testdatabases/genders.bugzilla414_3",
    &genders_query_functionality_tests_bugzilla414_3_tests,
  };

genders_query_tests_t genders_query_functionality_tests_bugzilla414_4_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"a&&b",
	{"tst", NULL},
	1,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_bugzilla414_4 =
  {
    "testdatabases/genders.bugzilla414_4",
    &genders_query_functionality_tests_bugzilla414_4_tests,
  };

genders_query_tests_t genders_query_functionality_tests_bugzilla414_5_tests =
  {
    {
      /* 
       * Simple tests 
       */
      {
	"a&&b",
	{"tst", NULL},
	1,
      },
      {
	NULL,
	{NULL},
	0
      },
    }
  };

genders_query_functionality_tests_t genders_query_functionality_tests_bugzilla414_5 =
  {
    "testdatabases/genders.bugzilla414_5",
    &genders_query_functionality_tests_bugzilla414_5_tests,
  };

genders_query_functionality_tests_t *genders_query_functionality_tests[] =
  {
    &genders_query_functionality_tests_query_1,
    &genders_query_functionality_tests_query_1_comma,
    &genders_query_functionality_tests_query_1_hostrange,
    &genders_query_functionality_tests_query_2,
    &genders_query_functionality_tests_query_2_comma,
    &genders_query_functionality_tests_query_2_hostrange,
    &genders_query_functionality_tests_query_special_chars,
    &genders_query_functionality_tests_bugzilla414_1,
    &genders_query_functionality_tests_bugzilla414_2,
    &genders_query_functionality_tests_bugzilla414_3,
    &genders_query_functionality_tests_bugzilla414_4,
    &genders_query_functionality_tests_bugzilla414_5,
    NULL
  };
