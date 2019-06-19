/*****************************************************************************\
 *  $Id: genders_testlib.c,v 1.6 2010-02-02 00:04:34 chu11 Exp $
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
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "genders.h"
#include "genders_testlib.h"
#include "genders_test_database.h"

static char *err_progname = NULL;

void
genders_err_init(char *progname)
{
  char *ptr = strrchr(progname, '/');
  err_progname = (ptr == NULL) ? progname : ptr + 1;
}

void
genders_err_exit(char *fmt, ...)
{
  char buffer[GENDERS_ERR_BUFLEN];
  va_list ap;

  assert (fmt != NULL && err_progname != NULL);

  va_start(ap, fmt);
  snprintf(buffer, GENDERS_ERR_BUFLEN, "%s: %s\n", err_progname, fmt);
  vfprintf(stderr, buffer, ap);
  va_end(ap);
  exit(1);
}

void 
genders_pass_output(char *funcname,
		    int num,
		    int err,
		    char *msg,
		    int verbose)
{
  assert(funcname);
  
  if (err)
    return;

  if (verbose > 1)
    {
      if (msg)
	fprintf(stderr, 
		"%s(%d): %s: PASS\n",
		funcname,
		num,
		msg);
      else
      fprintf(stderr, 
	      "%s(%d): PASS\n",
	      funcname,
	      num);
    }
}

int
genders_val_check(char *funcname,
		  int num,
		  int expected_val,
		  char *expected_val_str,
		  int val,
		  char *val_str,
		  char *msg,
		  int verbose)
{
  int err = 0;
  
  assert(funcname && expected_val_str && val_str);
  
  if (val != expected_val)
    {
      if (verbose)
        {
          if (msg)
            fprintf(stderr,
                    "%s(%d): %s: FAIL: %s=%d, %s=%d\n",
                    funcname,
                    num,
                    msg,
                    val_str,
                    val,
                    expected_val_str,
                    expected_val);
          else
            fprintf(stderr,
                    "%s(%d): FAIL: %s=%d, %s=%d\n",
                    funcname,
                    num,
                    val_str,
                    val,
                    expected_val_str,
                    expected_val);
        }
      err++;
    }
  return err;
}

static int 
_genders_val_check(char *funcname,
		   int num,
		   int expected_val,
		   char *expected_val_str,
		   int val,
		   char *val_str,
		   char *msg,
		   int verbose)
{
  int err = 0;

  assert(funcname && expected_val_str && val_str);

  if (val != expected_val)
    {
      if (verbose)
	{
	  if (msg)
	    fprintf(stderr, 
		    "%s(%d): %s: FAIL: %s=%d, %s=%d\n",
		    funcname,
		    num,
		    msg,
		    val_str,
		    val,
		    expected_val_str,
		    expected_val);
	  else
	    fprintf(stderr, 
		    "%s(%d): FAIL: %s=%d, %s=%d\n",
		    funcname,
		    num,
		    val_str,
		    val,
		    expected_val_str,
		    expected_val);
	}
      err++;
    }
  return err;
}

static int 
_genders_pointer_check(char *funcname,
		       int num,
		       int expected_val,
		       char *expected_val_str,
		       void *val,
		       char *val_str,
		       char *msg,
		       int verbose)
{
  int err = 0;

  assert(funcname && expected_val_str && val_str);
  
  if (!((expected_val == GENDERS_POINTER_NULL
	 && val == NULL)
	|| (expected_val == GENDERS_POINTER_NON_NULL
	    && val != NULL)))
    {
      if (verbose)
	{
	  if (msg)
	    fprintf(stderr, 
		    "%s(%d): %s: FAIL: %s=%p, %s=%s\n",
		    funcname,
		    num,
		    msg,
		    val_str,
		    val,
		    expected_val_str,
		    (expected_val == GENDERS_POINTER_NULL) ? "NULL" : "Non-NULL");
	  else
	    fprintf(stderr, 
		    "%s(%d): FAIL: %s=%p, %s=%s\n",
		    funcname,
		    num,
		    val_str,
		    val,
		    expected_val_str,
		    (expected_val == GENDERS_POINTER_NULL) ? "NULL" : "Non-NULL");
	}
      err++;
    }
  return err;
}

static int 
_genders_string_check(char *funcname,
		      int num,
		      char *expected_string,
		      char *expected_string_str,
		      char *string,
		      char *string_str,
		      char *msg,
		      int verbose)
{
  int err = 0;

  assert(funcname && expected_string_str && string_str);

  if ((expected_string && string && strcmp(expected_string, string) != 0)
      || (expected_string && !string)
      || (!expected_string && string))
    {
      if (verbose)
	{
	  if (msg)
	    fprintf(stderr, 
		    "%s(%d): %s: FAIL: %s=%s, %s=%s\n",
		    funcname,
		    num,
		    msg,
		    string_str,
		    (string) ? string : "NULL",
		    expected_string_str,
		    (expected_string) ? expected_string : "NULL");
	  else
	    fprintf(stderr, 
		    "%s(%d): FAIL: %s=%s, %s=%s\n",
		    funcname,
		    num,
		    string_str,
		    (string) ? string : "NULL",
		    expected_string_str,
		    (expected_string) ? expected_string : "NULL");
	}
      err++;
    }
  return err;
}

int 
genders_pointer_check(char *funcname,
		      int num,
		      void *expected_pointer,
		      char *expected_pointer_str,
		      void *pointer,
		      char *pointer_str,
		      char *msg,
		      int verbose)
{
  int err = 0;

  assert(funcname && expected_pointer_str && pointer_str);

  if (pointer != expected_pointer)
    {
      if (verbose)
	{
	  if (msg)
	    fprintf(stderr, 
		    "%s(%d): %s: FAIL: %s=%p, %s=%p\n",
		    funcname,
		    num,
		    msg,
		    pointer_str,
		    pointer,
		    expected_pointer_str,
		    expected_pointer);
	  else
	    fprintf(stderr, 
		    "%s(%d): FAIL: %s=%p, %s=%p\n",
		    funcname,
		    num,
		    pointer_str,
		    pointer,
		    expected_pointer_str,
		    expected_pointer);
	}
      err++;
    }
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int 
genders_pointer_exist_check(char *funcname,
			    int num,
			    void *pointer,
			    char *pointer_str,
			    genders_pointer_type_t expected_null,
			    char *msg,
			    int verbose)
{
  int err = 0;

  assert(funcname && pointer_str);

  if ((expected_null == GENDERS_POINTER_NULL && pointer != NULL)
      || (!expected_null == GENDERS_POINTER_NON_NULL && pointer == NULL))
    {
      if (verbose)
	{
	  if (msg)
	    fprintf(stderr, 
		    "%s(%d): %s: FAIL: %s=%p, expected=%s\n",
		    funcname,
		    num,
		    msg,
		    pointer_str,
		    pointer,
		    (expected_null) ? "NULL" : "Non-NULL");
	  else
	    fprintf(stderr, 
		    "%s(%d): FAIL: %s=%p, expected=%s\n",
		    funcname,
		    num,
		    pointer_str,
		    pointer,
		    (expected_null) ? "NULL" : "Non-NULL");
	}
      err++;
    }
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int genders_string_check(char *funcname,
			 int num,
			 char *expected_string,
			 char *expected_string_str,
			 char *string,
			 char *string_str,
			 char *msg,
			 int verbose)
{
  int err = 0;

  assert(funcname && expected_string_str && string_str);

  err = _genders_string_check(funcname,
			      num,
			      expected_string,
			      expected_string_str,
			      string,
			      string_str,
			      msg,
			      verbose);
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int
genders_return_value_check(char *funcname,
			   int num, 
			   int expected_return_value,
			   int return_value,
			   char *msg,
			   int verbose)
{
  int err = 0;

  assert(funcname);

  err += _genders_val_check(funcname,
			    num,
			    expected_return_value,
			    "expected_return_value",
			    return_value,
			    "return_value",
			    msg,
			    verbose);
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int
genders_errnum_check(char *funcname,
		     int num, 
		     int expected_errnum,
		     int errnum,
		     char *msg,
		     int verbose)
{
  int err = 0;

  assert(funcname);

  err += _genders_val_check(funcname,
			    num,
			    expected_errnum,
			    "expected_errnum",
			    errnum,
			    "errnum",
			    msg,
			    verbose);
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int
genders_return_value_errnum_check(char *funcname,
				  int num, 
				  int expected_return_value,
				  int expected_errnum,
				  int return_value,
				  int errnum,
				  char *msg,
				  int verbose)
{
  int err = 0;

  assert(funcname);
  
  err += _genders_val_check(funcname,
			    num,
			    expected_return_value,
			    "expected_return_value",
			    return_value,
			    "return_value",
			    msg,
			    verbose);
  err += _genders_val_check(funcname,
			    num,
			    expected_errnum,
			    "expected_errnum",
			    errnum,
			    "errnum",
			    msg,
			    verbose);
  genders_pass_output(funcname, num, err, msg, verbose);

  return err;
}

int
genders_return_value_pointer_errnum_check(char *funcname,
					  int num, 
					  int expected_return_value,
					  int expected_errnum,
					  void *return_value,
					  int errnum,
					  char *msg,
					  int verbose)
{
  int err = 0;

  assert(funcname);
  
  err += _genders_pointer_check(funcname,
				num,
				expected_return_value,
				"expected_return_value",
				return_value,
				"return_value",
				msg,
				verbose);
  err += _genders_val_check(funcname,
			    num,
			    expected_errnum,
			    "expected_errnum",
			    errnum,
			    "errnum",
			    msg,
			    verbose);
  genders_pass_output(funcname, num, err, msg, verbose);

  return err;
}

int
genders_return_value_errnum_string_check(char *funcname,
					 int num, 
					 int expected_return_value,
					 int expected_errnum,
					 char *expected_string,
					 int return_value,
					 int errnum,
					 char *string,
					 char *msg,
					 int verbose)
{
  int err = 0;

  assert(funcname);
  
  err += _genders_val_check(funcname,
			    num,
			    expected_return_value,
			    "expected_return_value",
			    return_value,
			    "return_value",
			    msg,
			    verbose);
  err += _genders_val_check(funcname,
			    num,
			    expected_errnum,
			    "expected_errnum",
			    errnum,
			    "errnum",
			    msg,
			    verbose);
  err += _genders_string_check(funcname,
			       num,
			       expected_string,
			       "expected_string",
			       string,
			       "string",
			       msg,
			       verbose);
  genders_pass_output(funcname, num, err, msg, verbose);

  return err;
}

int
genders_return_value_errnum_list_check(char *funcname,
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
				       int verbose)
{
  int err = 0;

  assert(funcname
	 && (comp_type == GENDERS_COMPARISON_MATCH 
	     || comp_type == GENDERS_COMPARISON_CLEAR)
	 && (!(comp_type == GENDERS_COMPARISON_MATCH) ||
	     (comp_type == GENDERS_COMPARISON_MATCH && expected_list && list))
	 && (!(comp_type == GENDERS_COMPARISON_CLEAR) ||
	     (comp_type == GENDERS_COMPARISON_CLEAR && !expected_list && list)));
  
  err += _genders_val_check(funcname,
			    num,
			    expected_return_value,
			    "expected_return_value",
			    return_value,
			    "return_value",
			    msg,
			    verbose);
  err += _genders_val_check(funcname,
			    num,
			    expected_errnum,
			    "expected_errnum",
			    errnum,
			    "errnum",
			    msg,
			    verbose);
	
  if (err == 0)
    {
      if (comp_type == GENDERS_COMPARISON_MATCH)
	{
	  int i;
	  for (i = 0; i < expected_list_len; i++)
	    {
	      int j, found_flag = 0;
	      for (j = 0; j < list_len; j++)
		{
		  if (strcmp(expected_list[i], list[j]) == 0)
		    {
		      found_flag++;
		      break;
		    }
		}
	      err += _genders_val_check(funcname,
					num,
					1,
					"expected_found_flag",
					found_flag,
					"found_flag",
					msg,
					verbose);
	    }
	}
      else if (comp_type == GENDERS_COMPARISON_CLEAR)
	{
	  int i, not_empty_flag = 0;
	  for (i = 0; i < list_len; i++)
	    {
	      if (strcmp(list[i], "") != 0)
		{
		  not_empty_flag++;
		  break;
		}
	    }
	  err += genders_val_check(funcname,
				   num,
				   0,
				   "expected_not_empty_flag",
				   not_empty_flag,
				   "not_empty_flag",
				   msg,
				   verbose);
	}
    }
  
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int
genders_return_value_errnum_attrval_list_check(char *funcname,
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
					       int verbose)
{
  int err = 0;

  assert(funcname);
  
  err += _genders_val_check(funcname,
			    num,
			    expected_return_value,
			    "expected_return_value",
			    return_value,
			    "return_value",
			    msg,
			    verbose);
  err += _genders_val_check(funcname,
			    num,
			    expected_errnum,
			    "expected_errnum",
			    errnum,
			    "errnum",
			    msg,
			    verbose);
	
  /* Step 1: Make sure every attr we're expected is in the attrlist */
  if (err == 0)
    {
      int i;

      for (i = 0; i < expected_list_len; i++)
	{
	  int j, found_flag = 0;
	  for (j = 0; j < list_len; j++)
	    {
	      if (strcmp(expected_attrlist[i], attrlist[j]) == 0)
		{
		  found_flag++;
		  break;
		}
	    }
	  err += _genders_val_check(funcname,
				    num,
				    1,
				    "expected_found_flag",
				    found_flag,
				    "found_flag",
				    msg,
				    verbose);
	}
    }

  /* Step 2: For each expected attr, find the index of the attr in the
   * attrlist and compare it to the approprite val.
   */
  if (err == 0)
    {
      int i, j, val_index, match_flag;
      for (i = 0; i < expected_list_len; i++)
	{
	  val_index = -1;
	  match_flag = 0;
	  for (j = 0; j < list_len; j++)
	    {
	      if (strcmp(expected_attrlist[i], attrlist[j]) == 0)
		{
		  val_index = j;
		  break;
		}
	    }
	  if (val_index == -1)
	    genders_err_exit("genders_return_value_errnum_attrval_list_check: fatal error");
	  
	  if (strcmp(expected_vallist[i], vallist[j]) == 0)
	    match_flag++;

	  err += _genders_val_check(funcname,
				    num,
				    1,
				    "expected_match_flag",
				    match_flag,
				    "match_flag",
				    msg,
				    verbose);
	}
    }
  
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}

int
genders_flag_check(char *funcname,
		   int num,
		   unsigned int expected_flags,
		   unsigned int return_flags,
		   char *msg,
		   int verbose)
{
  int err = 0;
  
  assert(funcname);
  
  err += _genders_val_check(funcname,
                            num,
                            expected_flags,
                            "expected_flags",
                            return_flags,
                            "return_flags",
                            msg,
                            verbose);
  genders_pass_output(funcname, num, err, msg, verbose);
  return err;
}
