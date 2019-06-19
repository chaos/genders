/*****************************************************************************\
 *  $Id: genders_test_query_tests.h,v 1.5 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_TEST_QUERY_TESTS_H
#define _GENDERS_TEST_QUERY_TESTS_H 1

#include "genders.h"
#include "genders_testlib.h"

#define GENDERS_QUERY_BUFLEN   1024
#define GENDERS_QUERY_MAXTESTS 2048
#define GENDERS_QUERY_MAXNODES 100

typedef struct {
  struct {
    char *query;
    char *nodes[GENDERS_QUERY_MAXNODES];
    int nodeslen;
  } tests[GENDERS_QUERY_MAXTESTS];
} genders_query_tests_t;

typedef struct {
  char *filename;
  genders_query_tests_t *tests;
} genders_query_functionality_tests_t;

extern char *genders_query_parse_error_tests[];
extern genders_query_functionality_tests_t *genders_query_functionality_tests[];

#endif /* _GENDERS_TEST_QUERY_TESTS_H */
