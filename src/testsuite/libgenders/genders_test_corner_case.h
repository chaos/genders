/*****************************************************************************\
 *  $Id: genders_test_corner_case.h,v 1.8 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_TEST_CORNER_CASE_H
#define _GENDERS_TEST_CORNER_CASE_H 1

#include "genders.h"
#include "genders_testlib.h"

typedef struct {
  int num;
  genders_handle_type_t param1;
  int expected_return_value;
  int expected_errnum;
} genders_handle_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_pointer_type_t param2;
  int expected_return_value;
  int expected_errnum;
} genders_handle_pointer_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_string_type_t param2;
  int expected_return_value;
  int expected_errnum;
} genders_handle_string_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_pointer_type_t param2;
  genders_length_type_t param3;
  int expected_return_value;
  int expected_errnum;
} genders_handle_pointer_length_corner_case_t;

typedef genders_handle_corner_case_t genders_handle_destroy_corner_case_t; 

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_filename_type_t param2;
  int expected_return_value;
  int expected_errnum;
} genders_load_data_corner_case_t; 

typedef struct {
  int num;
  genders_handle_type_t param1;
  int expected_return_value;
} genders_errnum_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  int expected_errnum_string;
} genders_errstring_corner_case_t;

typedef genders_errstring_corner_case_t genders_strerror_corner_case_t;
typedef genders_errstring_corner_case_t genders_errormsg_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  char *param2;
  int expected_errnum;
} genders_perror_corner_case_t;

typedef genders_handle_pointer_corner_case_t genders_get_flags_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_flag_type_t param2;
  int expected_return_value;
  int expected_errnum;
} genders_set_flags_corner_case_t; 

typedef genders_handle_corner_case_t genders_getnumnodes_corner_case_t;
typedef genders_handle_corner_case_t genders_getnumattrs_corner_case_t;
typedef genders_handle_corner_case_t genders_getmaxattrs_corner_case_t;
typedef genders_handle_corner_case_t genders_getmaxnodelen_corner_case_t;
typedef genders_handle_corner_case_t genders_getmaxattrlen_corner_case_t;
typedef genders_handle_corner_case_t genders_getmaxvallen_corner_case_t;

typedef genders_handle_pointer_corner_case_t genders_nodelist_create_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_nodelist_clear_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_nodelist_destroy_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_attrlist_create_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_attrlist_clear_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_attrlist_destroy_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_vallist_create_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_vallist_clear_corner_case_t;
typedef genders_handle_pointer_corner_case_t genders_vallist_destroy_corner_case_t;

typedef genders_handle_pointer_length_corner_case_t genders_getnodename_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_pointer_type_t param2;
  genders_length_type_t param3;
  genders_string_type_t param4;
  genders_string_type_t param5;
  int expected_return_value;
  int expected_errnum;
} genders_getnodes_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_pointer_type_t param2;
  genders_pointer_type_t param3;
  genders_length_type_t param4;
  genders_string_type_t param5;
  int expected_return_value;
  int expected_errnum;
} genders_getattr_corner_case_t;

typedef genders_handle_pointer_length_corner_case_t genders_getattr_all_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_string_type_t param2;
  genders_string_type_t param3;
  genders_pointer_type_t param4;
  genders_length_type_t param5;
  int expected_return_value;
  int expected_errnum;
} genders_testattr_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_string_type_t param2;
  genders_string_type_t param3;
  genders_string_type_t param4;
  int expected_return_value;
  int expected_errnum;
} genders_testattrval_corner_case_t;

typedef genders_handle_string_corner_case_t genders_isnode_corner_case_t;
typedef genders_handle_string_corner_case_t genders_isattr_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_string_type_t param2;
  genders_string_type_t param3;
  int expected_return_value;
  int expected_errnum;
} genders_isattrval_corner_case_t;

typedef genders_handle_string_corner_case_t genders_index_attrvals_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_pointer_type_t param2;
  genders_length_type_t param3;
  genders_string_type_t param4;
  int expected_return_value;
  int expected_errnum;
} genders_query_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_string_type_t param2;
  genders_string_type_t param3;
  int expected_return_value;
  int expected_errnum;
} genders_testquery_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_filename_type_t param2;
  genders_pointer_type_t param3;
  int expected_return_value;
  int expected_errnum;
} genders_parse_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  int param2;
  int expected_errnum;
} genders_set_errnum_corner_case_t;

typedef struct {
  int num;
  genders_handle_type_t param1;
  genders_pointer_type_t expected_return_value;
  int expected_errnum;
} genders_copy_corner_case_t;

extern genders_handle_destroy_corner_case_t genders_handle_destroy_corner_case_tests[];
extern genders_load_data_corner_case_t genders_load_data_corner_case_tests[];
extern genders_errnum_corner_case_t genders_errnum_corner_case_tests[];
extern genders_strerror_corner_case_t genders_strerror_corner_case_tests[];
extern genders_errormsg_corner_case_t genders_errormsg_corner_case_tests[];
extern genders_perror_corner_case_t genders_perror_corner_case_tests[];
extern genders_get_flags_corner_case_t genders_get_flags_corner_case_tests[];
extern genders_set_flags_corner_case_t genders_set_flags_corner_case_tests[];
extern genders_getnumnodes_corner_case_t genders_getnumnodes_corner_case_tests[];
extern genders_getnumattrs_corner_case_t genders_getnumattrs_corner_case_tests[];
extern genders_getmaxattrs_corner_case_t genders_getmaxattrs_corner_case_tests[];
extern genders_getmaxnodelen_corner_case_t genders_getmaxnodelen_corner_case_tests[];
extern genders_getmaxattrlen_corner_case_t genders_getmaxattrlen_corner_case_tests[];
extern genders_getmaxvallen_corner_case_t genders_getmaxvallen_corner_case_tests[];
extern genders_nodelist_create_corner_case_t genders_nodelist_create_corner_case_tests[];
extern genders_nodelist_clear_corner_case_t genders_nodelist_clear_corner_case_tests[];
extern genders_nodelist_destroy_corner_case_t genders_nodelist_destroy_corner_case_tests[];
extern genders_attrlist_create_corner_case_t genders_attrlist_create_corner_case_tests[];
extern genders_attrlist_clear_corner_case_t genders_attrlist_clear_corner_case_tests[];
extern genders_attrlist_destroy_corner_case_t genders_attrlist_destroy_corner_case_tests[];
extern genders_vallist_create_corner_case_t genders_vallist_create_corner_case_tests[];
extern genders_vallist_clear_corner_case_t genders_vallist_clear_corner_case_tests[];
extern genders_vallist_destroy_corner_case_t genders_vallist_destroy_corner_case_tests[];
extern genders_getnodename_corner_case_t genders_getnodename_corner_case_tests[];
extern genders_getnodes_corner_case_t genders_getnodes_corner_case_tests[];
extern genders_getattr_corner_case_t genders_getattr_corner_case_tests[];
extern genders_getattr_all_corner_case_t genders_getattr_all_corner_case_tests[];
extern genders_testattr_corner_case_t genders_testattr_corner_case_tests[];
extern genders_testattrval_corner_case_t genders_testattrval_corner_case_tests[];
extern genders_isnode_corner_case_t genders_isnode_corner_case_tests[];
extern genders_isattr_corner_case_t genders_isattr_corner_case_tests[];
extern genders_isattrval_corner_case_t genders_isattrval_corner_case_tests[];
extern genders_index_attrvals_corner_case_t genders_index_attrvals_corner_case_tests[];
extern genders_query_corner_case_t genders_query_corner_case_tests[];
extern genders_testquery_corner_case_t genders_testquery_corner_case_tests[];
extern genders_parse_corner_case_t genders_parse_corner_case_tests[];
extern genders_set_errnum_corner_case_t genders_set_errnum_corner_case_tests[];
extern genders_copy_corner_case_t genders_copy_corner_case_tests[];

typedef int (*GendersCornerCaseFunc)(int);

int genders_handle_create_corner_case(int verbose);
int genders_handle_destroy_corner_case(int verbose);
int genders_load_data_corner_case(int verbose);
int genders_errnum_corner_case(int verbose);
int genders_strerror_corner_case(int verbose);
int genders_errormsg_corner_case(int verbose);
int genders_perror_corner_case(int verbose);
int genders_get_flags_corner_case(int verbose);
int genders_set_flags_corner_case(int verbose);
int genders_getnumnodes_corner_case(int verbose);
int genders_getnumattrs_corner_case(int verbose);
int genders_getmaxattrs_corner_case(int verbose);
int genders_getmaxnodelen_corner_case(int verbose);
int genders_getmaxattrlen_corner_case(int verbose);
int genders_getmaxvallen_corner_case(int verbose);
int genders_nodelist_create_corner_case(int verbose);
int genders_nodelist_clear_corner_case(int verbose);
int genders_nodelist_destroy_corner_case(int verbose);
int genders_attrlist_create_corner_case(int verbose);
int genders_attrlist_clear_corner_case(int verbose);
int genders_attrlist_destroy_corner_case(int verbose);
int genders_vallist_create_corner_case(int verbose);
int genders_vallist_clear_corner_case(int verbose);
int genders_vallist_destroy_corner_case(int verbose);
int genders_getnodename_corner_case(int verbose);
int genders_getnodes_corner_case(int verbose);
int genders_getattr_corner_case(int verbose);
int genders_getattr_all_corner_case(int verbose);
int genders_testattr_corner_case(int verbose);
int genders_testattrval_corner_case(int verbose);
int genders_isnode_corner_case(int verbose);
int genders_isattr_corner_case(int verbose);
int genders_isattrval_corner_case(int verbose);
int genders_index_attrvals_corner_case(int verbose);
int genders_query_corner_case(int verbose);
int genders_testquery_corner_case(int verbose);
int genders_parse_corner_case(int verbose);
int genders_set_errnum_corner_case(int verbose);
int genders_copy_corner_case(int verbose);

#endif /* _GENDERS_TEST_CORNER_CASE_H */
