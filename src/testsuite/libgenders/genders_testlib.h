/*****************************************************************************\
 *  $Id: genders_testlib.h,v 1.5 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_TESTLIB_H
#define _GENDERS_TESTLIB_H 1

#include "genders.h"

#define GENDERS_ERR_BUFLEN  4096

typedef enum {
  GENDERS_HANDLE_NULL = 0,
  GENDERS_HANDLE_UNLOADED = 1,
  GENDERS_HANDLE_UNLOADED_DESTROYED = 2,
  GENDERS_HANDLE_LOADED = 3,
  GENDERS_HANDLE_LOADED_DESTROYED = 4 
} genders_handle_type_t;

typedef enum {
  GENDERS_FILENAME_NULL = 0,
  GENDERS_FILENAME_NON_NULL_EMPTY = 1,
  GENDERS_FILENAME_NON_NULL_EXIST = 2,
  GENDERS_FILENAME_NON_NULL_NOT_EXIST = 3,
} genders_filename_type_t;

typedef enum {
  GENDERS_POINTER_NULL = 0,
  GENDERS_POINTER_NON_NULL = 1,
} genders_pointer_type_t;

typedef enum {
  GENDERS_STRING_NULL = 0,
  GENDERS_STRING_NON_NULL_EMPTY = 1,
  GENDERS_STRING_NON_NULL_FULL = 2,
} genders_string_type_t;

typedef enum {
  GENDERS_LENGTH_NEGATIVE = -1,
  GENDERS_LENGTH_ZERO = 0,
  GENDERS_LENGTH_POSITIVE_SMALL = 1,
  GENDERS_LENGTH_POSITIVE_LARGE = 100,
} genders_length_type_t;

typedef enum {
  GENDERS_TEST_FLAG_DEFAULT = GENDERS_FLAG_DEFAULT,
  GENDERS_TEST_FLAG_MIN = 0x00000001,
  GENDERS_TEST_FLAG_MAX = 0x80000000,
} genders_flag_type_t;

typedef enum {
  GENDERS_COMPARISON_MATCH = 0,
  GENDERS_COMPARISON_CLEAR = 1,
} genders_comparison_type_t;

typedef int (*GendersGetFunc)(genders_t);
typedef int (*GendersListCreateFunc)(genders_t, char ***);
typedef int (*GendersListClearFunc)(genders_t, char **);
typedef int (*GendersListDestroyFunc)(genders_t, char **);

void genders_err_init(char *progname);
void genders_err_exit(char *fmt, ...);

void genders_pass_output(char *funcname,
			 int num,
			 int err,
			 char *msg,
			 int verbose);

int genders_val_check(char *funcname,
		      int num,
		      int expected_val,
		      char *expected_val_str,
		      int val,
		      char *val_str,
		      char *msg,
		      int verbose);

int genders_pointer_check(char *funcname,
			  int num,
			  void *expected_pointer,
			  char *expected_pointer_str,
			  void *pointer,
			  char *pointer_str,
			  char *msg,
			  int verbose);

int genders_pointer_exist_check(char *funcname,
				int num,
				void *pointer,
				char *pointer_str,
				genders_pointer_type_t expected_null,
				char *msg,
				int verbose);

int genders_string_check(char *funcname,
			 int num,
			 char *expected_string,
			 char *expected_string_str,
			 char *string,
			 char *string_str,
			 char *msg,
			 int verbose);

int genders_return_value_check(char *funcname,
			       int num, 
			       int expected_return_value,
			       int ret,
			       char *msg,
			       int verbose);

int genders_errnum_check(char *funcname,
			 int num, 
			 int expected_errnum,
			 int errnum,
			 char *msg,
			 int verbose);

int genders_return_value_errnum_check(char *funcname,
				      int num, 
				      int expected_return_value,
				      int expected_errnum,
				      int return_value,
				      int errnum,
				      char *msg,
				      int verbose);

int genders_return_value_pointer_errnum_check(char *funcname,
					      int num, 
					      int expected_return_value,
					      int expected_errnum,
					      void *return_value,
					      int errnum,
					      char *msg,
					      int verbose);

int genders_return_value_errnum_string_check(char *funcname,
					     int num, 
					     int expected_return_value,
					     int expected_errnum,
					     char *expected_string,
					     int return_value,
					     int errnum,
					     char *string,
					     char *msg,
					     int verbose);

int genders_return_value_errnum_list_check(char *funcname,
					   int num, 
					   int expected_return_value,
					   int expected_errnum,
					   char *expected_list[],
					   int expected_list_len,
					   int return_value,
					   int errnum,
					   char *list[],
					   int list_len,
					   genders_comparison_type_t comp_type,
					   char *msg,
					   int verbose);

int genders_return_value_errnum_attrval_list_check(char *funcname,
						   int num, 
						   int expected_return_value,
						   int expected_errnum,
						   char *expected_attrlist[],
						   char *expected_vallist[],
						   int expected_list_len,
						   int return_value,
						   int errnum,
						   char *attrlist[],
						   char *vallist[],
						   int list_len,
						   char *msg,
						   int verbose);

int genders_flag_check(char *funcname,
		       int num,
		       unsigned int expected_flags,
		       unsigned int return_flags,
		       char *msg,
		       int verbose);


#endif /* _GENDERS_TESTLIB_H */
