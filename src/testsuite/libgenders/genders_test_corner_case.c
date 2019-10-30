/*****************************************************************************\
 *  $Id: genders_test_corner_case.c,v 1.14 2010-02-02 00:04:34 chu11 Exp $
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
#include <unistd.h>		/* gethostname */
#include <sys/param.h>		/* MAXHOSTNAMELEN */
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>       	/* stat() */
#include <errno.h>
#include <assert.h>
#include <fcntl.h>       	/* O_APPEND */
#if HAVE_PATHS_H
#include <paths.h>		/* _PATH_DEVNULL */
#endif /* HAVE_PATHS_H */

#include "genders.h"
#include "genders_testlib.h"
#include "genders_test_corner_case.h"
#include "genders_test_database.h"

#ifndef _PATH_DEVNULL
#define _PATH_DEVNULL "/dev/null"
#endif /* _PATH_DEVNULL */

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN    64
#endif /* MAXHOSTNAMELEN */

static genders_t 
genders_unloaded_handle(void) 
{
  genders_t handle = genders_handle_create();
  if (!handle)
    genders_err_exit("genders_handle_create");
  return handle;
}

static genders_t 
genders_destroyed_unloaded_handle(void)
{
  genders_t handle = genders_handle_create();
  if (!handle)
    genders_err_exit("genders_handle_create");
  if (genders_handle_destroy(handle) < 0) {
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(handle));
  }
  return handle;
}

static genders_t 
genders_loaded_handle(void) 
{
  genders_t handle = genders_handle_create();
  if (!handle)
    genders_err_exit("genders_handle_create");
  if (genders_load_data(handle, genders_database_corner_case.filename) < 0)
    genders_err_exit("genders_load_data: %s", genders_errormsg(handle));
  return handle;
}

static genders_t 
genders_destroyed_loaded_handle(void)
{
  genders_t handle = genders_handle_create();
  if (!handle)
    genders_err_exit("genders_handle_create");
  if (genders_load_data(handle, genders_database_corner_case.filename) < 0)
    genders_err_exit("genders_load_data: %s", genders_errormsg(handle));
  if (genders_handle_destroy(handle) < 0)
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(handle));
  return handle;
}

static genders_t
genders_handle_get(genders_handle_type_t type)
{
  genders_t handle = NULL;

  switch (type)
    {
    case GENDERS_HANDLE_NULL:
      handle = NULL;
      break;
    case GENDERS_HANDLE_UNLOADED:
      handle = genders_unloaded_handle();
      break;
    case GENDERS_HANDLE_UNLOADED_DESTROYED:
      handle = genders_destroyed_unloaded_handle();
      break;
    case GENDERS_HANDLE_LOADED:
      handle = genders_loaded_handle();
      break;
    case GENDERS_HANDLE_LOADED_DESTROYED:
      handle = genders_destroyed_loaded_handle();
      break;
    default:
      genders_err_exit("genders_handle_get: invalid type: %d\n", type);
    }

  return handle;
}

static void
genders_handle_cleanup(genders_t handle)
{
  /* Who knows what was done during the test, make a best effort
   * to cleanup and return.
   */
  genders_handle_destroy(handle);
  return;
}

static char *
genders_filename_get(genders_filename_type_t type)
{
  char *filename = NULL;

  switch (type)
    {
    case GENDERS_FILENAME_NULL:
      filename = NULL;
      break;
    case GENDERS_FILENAME_NON_NULL_EMPTY:
      filename = "";
      break;
    case GENDERS_FILENAME_NON_NULL_EXIST:
      filename = genders_database_corner_case.filename;
      break;
    case GENDERS_FILENAME_NON_NULL_NOT_EXIST:
      filename = genders_database_not_exist.filename;
      break;
    default:
      genders_err_exit("genders_handle_get: invalid type: %d\n", type);
    }

  return filename;
}

int
genders_handle_create_corner_case(int verbose)
{
  /* No corner case tests */
  return 0;
}

int
genders_handle_destroy_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_handle_destroy_corner_case_t *tests = &genders_handle_destroy_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);
      return_value = genders_handle_destroy(handle);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_handle_destroy",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_load_data_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_load_data_corner_case_t *tests = &genders_load_data_corner_case_tests[0];

  /* Special case tests.  It depends on if the default genders file
   * exists on the system and the tester has access to it.
   */
  {
    struct stat buf;

    if (stat(GENDERS_DEFAULT_FILE, &buf) < 0)
      {
	tests[4].expected_return_value = -1;
	tests[4].expected_errnum = GENDERS_ERR_OPEN;
	tests[5].expected_return_value = -1;
	tests[5].expected_errnum = GENDERS_ERR_OPEN;
      }
  }

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *filename;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);
      filename = genders_filename_get(tests[i].param2);
      return_value = genders_load_data(handle, filename);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_load_data",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_errnum_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_errnum_corner_case_t *tests = &genders_errnum_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int err, return_value;

      handle = genders_handle_get(tests[i].param1);
      return_value = genders_errnum(handle);
 
      err = genders_return_value_check("genders_errnum",
				       tests[i].num,
				       tests[i].expected_return_value,
				       return_value,
				       NULL,
				       verbose);

      errcount += err;
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_strerror_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_strerror_corner_case_t *tests = &genders_strerror_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *pointer;
      char *expected_pointer;
      int err;

      handle = genders_handle_get(tests[i].param1);
      expected_pointer = genders_errormsg(handle);
      pointer = genders_strerror(tests[i].expected_errnum_string);
 
      err = genders_pointer_check("genders_errormsg",
				  tests[i].num,
				  expected_pointer,
				  "expected_pointer",
				  pointer,
				  "pointer",
				  NULL,
				  verbose);
      errcount += err;
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_errormsg_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_errormsg_corner_case_t *tests = &genders_errormsg_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *pointer;
      char *expected_pointer;
      int err;

      handle = genders_handle_get(tests[i].param1);
      expected_pointer = genders_strerror(tests[i].expected_errnum_string);
      pointer = genders_errormsg(handle);
 
      err = genders_pointer_check("genders_errormsg",
				  tests[i].num,
				  expected_pointer,
				  "expected_pointer",
				  pointer,
				  "pointer",
				  NULL,
				  verbose);
      errcount += err;
      genders_handle_cleanup(handle);
      i++;
    }
  
  return errcount;
}

int
genders_perror_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  int stderr_save;
  int dev_null;
  genders_perror_corner_case_t *tests = &genders_perror_corner_case_tests[0];

  if ((stderr_save = dup(STDERR_FILENO)) < 0)
    genders_err_exit("dup: %s", strerror(errno));
  if ((dev_null = open(_PATH_DEVNULL, O_APPEND)) < 0)
    genders_err_exit("open: %s: %s", _PATH_DEVNULL, strerror(errno));
  
  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int err, errnum;

      /* Must route stderr somewhere else during call to genders_perror() */
      handle = genders_handle_get(tests[i].param1);
      if ((dup2(dev_null, STDERR_FILENO)) < 0)
	genders_err_exit("dup2: %s", strerror(errno));
      genders_perror(handle, tests[i].param2);
      if ((dup2(stderr_save, STDERR_FILENO)) < 0)
	genders_err_exit("dup2: %s", strerror(errno));
      errnum = genders_errnum(handle);

      err = genders_errnum_check("genders_errnum",
				 tests[i].num,
				 tests[i].expected_errnum,
				 errnum,
				 NULL,
				 verbose);

      errcount += err;
      genders_handle_cleanup(handle);
      i++;
    }

  close(dev_null);
  return errcount;
}

int
genders_get_flags_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_get_flags_corner_case_t *tests = &genders_get_flags_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      unsigned int flags;
      int return_value, errnum;
      unsigned int *flagsptr;

      handle = genders_handle_get(tests[i].param1);
      flagsptr = (tests[i].param2 == GENDERS_POINTER_NULL) ? NULL : &flags;
      return_value = genders_get_flags(handle, flagsptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_get_flags",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_set_flags_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_set_flags_corner_case_t *tests = &genders_set_flags_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      unsigned int flags;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);
      flags = tests[i].param2;
      return_value = genders_set_flags(handle, flags);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_set_flags",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

static int
_genders_getfunc_corner_case(GendersGetFunc func, 
			     char *funcname,
			     int num,
			     genders_handle_type_t param1,
			     int expected_return_value,
			     int expected_errnum,
			     int verbose)
{
  genders_t handle;
  int return_value, errnum, err;

  assert(func && funcname);

  handle = genders_handle_get(param1);
  return_value = func(handle);
  errnum = genders_errnum(handle);

  err = genders_return_value_errnum_check(funcname,
					  num,
					  expected_return_value,
					  expected_errnum,
					  return_value,
					  errnum,
					  NULL,
					  verbose);
  genders_handle_cleanup(handle);

  return err;
}

int
genders_getnumnodes_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getnumnodes_corner_case_t *tests = &genders_getnumnodes_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_getfunc_corner_case(genders_getnumnodes,
					       "genders_getnumnodes",
					       tests[i].num,
					       tests[i].param1,
					       tests[i].expected_return_value,
					       tests[i].expected_errnum,
					       verbose);
      i++;
    }

  return errcount;
}

int
genders_getnumattrs_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getnumattrs_corner_case_t *tests = &genders_getnumattrs_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_getfunc_corner_case(genders_getnumattrs,
					       "genders_getnumattrs",
					       tests[i].num,
					       tests[i].param1,
					       tests[i].expected_return_value,
					       tests[i].expected_errnum,
					       verbose);
      i++;
    }

  return errcount;
}

int
genders_getmaxattrs_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getmaxattrs_corner_case_t *tests = &genders_getmaxattrs_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_getfunc_corner_case(genders_getmaxattrs,
					       "genders_getmaxattrs",
					       tests[i].num,
					       tests[i].param1,
					       tests[i].expected_return_value,
					       tests[i].expected_errnum,
					       verbose);
      i++;
    }

  return errcount;
}

int
genders_getmaxnodelen_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getmaxnodelen_corner_case_t *tests = &genders_getmaxnodelen_corner_case_tests[0];

  /* Special case for test #3.  If you are on a host whose hostname
   * length is greater than the database maxnodelen, then that is the
   * actual maxnodelen.
   */
  {
    char buffer[MAXHOSTNAMELEN+1];
    char *ptr;

    memset(buffer, '\0', MAXHOSTNAMELEN+1);

    if (gethostname(buffer, MAXHOSTNAMELEN) < 0)
      genders_err_exit("gethostname: %s", strerror(errno));

    /* shorten if necessary */
    if ((ptr = strchr(buffer,'.')))
      *ptr = '\0';

    if (strlen(buffer) > tests[3].expected_return_value)
      tests[3].expected_return_value = strlen(buffer);
  }

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_getfunc_corner_case(genders_getmaxnodelen,
					       "genders_getmaxnodelen",
					       tests[i].num,
					       tests[i].param1,
					       tests[i].expected_return_value,
					       tests[i].expected_errnum,
					       verbose);
      i++;
    }

  return errcount;
}

int
genders_getmaxattrlen_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getmaxattrlen_corner_case_t *tests = &genders_getmaxattrlen_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_getfunc_corner_case(genders_getmaxattrlen,
					       "genders_getmaxattrlen",
					       tests[i].num,
					       tests[i].param1,
					       tests[i].expected_return_value,
					       tests[i].expected_errnum,
					       verbose);
      i++;
    }

  return errcount;
}

int
genders_getmaxvallen_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getmaxvallen_corner_case_t *tests = &genders_getmaxvallen_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_getfunc_corner_case(genders_getmaxvallen,
					       "genders_getmaxvallen",
					       tests[i].num,
					       tests[i].param1,
					       tests[i].expected_return_value,
					       tests[i].expected_errnum,
					       verbose);
      i++;
    }

  return errcount;
}

static int
_genders_list_create_corner_case(GendersListCreateFunc funccreate,
				 GendersListDestroyFunc funcdestroy,
				 char *funcname,
				 int num,
				 genders_handle_type_t param1,
				 genders_pointer_type_t param2,
				 int expected_return_value,
				 int expected_errnum,
				 int verbose)
{
  genders_t handle;
  char **list;
  char ***listptr;
  int return_value, errnum, err;

  assert(funccreate && funcdestroy && funcname);

  handle = genders_handle_get(param1);
  listptr = (param2 == GENDERS_POINTER_NULL) ? NULL : &list;
  return_value = funccreate(handle, listptr);
  errnum = genders_errnum(handle);
      
  err = genders_return_value_errnum_check(funcname,
					  num,
					  expected_return_value,
					  expected_errnum,
					  return_value,
					  errnum,
					  NULL,
					  verbose);

  if (expected_return_value >= 0
      && expected_return_value == err
      && errnum == GENDERS_ERR_SUCCESS)
    {
      if (funcdestroy(handle, list) < 0)
	genders_err_exit("funcdestroy: %s", genders_errormsg(handle));
    }

  genders_handle_cleanup(handle);

  return err;
}

static int
_genders_list_clear_corner_case(GendersListCreateFunc funccreate,
				GendersListClearFunc funcclear,
				GendersListDestroyFunc funcdestroy,
				char *funcname,
				int num,
				genders_handle_type_t param1,
				genders_pointer_type_t param2,
				int expected_return_value,
				int expected_errnum,
				int verbose)
{
  genders_t gh = NULL;
  genders_t handle;
  char **list;
  int return_value, errnum, err;

  assert(funccreate && funcclear && funcdestroy && funcname);

  /* Must do param2 first.  If not, during a param1 destroyed
   * handle test, the param2 could be created and be given a
   * pointer identical to the param1 handle.
   */
  if (param2 == GENDERS_POINTER_NON_NULL)
    {
      gh = genders_handle_get(GENDERS_HANDLE_LOADED);
      if (funccreate(gh, &list) < 0)
	genders_err_exit("funccreate: %s", genders_errormsg(gh));
    }
  else
    list = NULL;

  handle = genders_handle_get(param1);
  return_value = funcclear(handle, list);
  errnum = genders_errnum(handle);
      
  err = genders_return_value_errnum_check(funcname,
					  num,
					  expected_return_value,
					  expected_errnum,
					  return_value,
					  errnum,
					  NULL,
					  verbose);

  if (param2 == GENDERS_POINTER_NON_NULL)
    {
      if (funcdestroy(gh, list) < 0)
	genders_err_exit("funccreate: %s", genders_errormsg(gh));
      if (genders_handle_destroy(gh) < 0)
	genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));
    }
  genders_handle_cleanup(handle);
  
  return err;
}

static int
_genders_list_destroy_corner_case(GendersListCreateFunc funccreate,
				  GendersListDestroyFunc funcdestroy,
				  char *funcname,
				  int num,
				  genders_handle_type_t param1,
				  genders_pointer_type_t param2,
				  int expected_return_value,
				  int expected_errnum,
				  int verbose)
{
  genders_t gh = NULL;
  genders_t handle;
  char **list;
  int return_value, errnum, err;

  assert(funccreate && funcdestroy && funcname);

  /* Must do param2 first.  If not, during a param1 destroyed
   * handle test, the param2 could be created and be given a
   * pointer identical to the param1 handle.
   */
  if (param2 == GENDERS_POINTER_NON_NULL)
    {
      gh = genders_handle_get(GENDERS_HANDLE_LOADED);
      if (funccreate(gh, &list) < 0)
	genders_err_exit("funccreate: %s", genders_errormsg(gh));
    }
  else
    list = NULL;

  handle = genders_handle_get(param1);
  return_value = funcdestroy(handle, list);
  errnum = genders_errnum(handle);
      
  err = genders_return_value_errnum_check(funcname,
					  num,
					  expected_return_value,
					  expected_errnum,
					  return_value,
					  errnum,
					  NULL,
					  verbose);

  if (param2 == GENDERS_POINTER_NON_NULL)
    {
      if (!(return_value == 0 && errnum == GENDERS_ERR_SUCCESS))
	{
	  if (funcdestroy(gh, list) < 0)
	    genders_err_exit("funcdestroy: %s", genders_errormsg(gh));
	}
      if (genders_handle_destroy(gh) < 0)
	genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));
    }
  genders_handle_cleanup(handle);

  return err;
}

int
genders_nodelist_create_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_nodelist_create_corner_case_t *tests = &genders_nodelist_create_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_create_corner_case(genders_nodelist_create,
						   genders_nodelist_destroy,
						   "genders_nodelist_create",
						   tests[i].num,
						   tests[i].param1,
						   tests[i].param2,
						   tests[i].expected_return_value,
						   tests[i].expected_errnum,
						   verbose);
      i++;
    }

  return errcount;
}

int
genders_nodelist_clear_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_nodelist_clear_corner_case_t *tests = &genders_nodelist_clear_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_clear_corner_case(genders_nodelist_create,
						  genders_nodelist_clear,
						  genders_nodelist_destroy,
						  "genders_nodelist_clear",
						  tests[i].num,
						  tests[i].param1,
						  tests[i].param2,
						  tests[i].expected_return_value,
						  tests[i].expected_errnum,
						  verbose);
      i++;
    }

  return errcount;
}

int
genders_nodelist_destroy_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_nodelist_destroy_corner_case_t *tests = &genders_nodelist_destroy_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_destroy_corner_case(genders_nodelist_create,
						    genders_nodelist_destroy,
						    "genders_nodelist_destroy",
						    tests[i].num,
						    tests[i].param1,
						    tests[i].param2,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    verbose);
      i++;
    }

  return errcount;
}

int
genders_attrlist_create_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_attrlist_create_corner_case_t *tests = &genders_attrlist_create_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_create_corner_case(genders_attrlist_create,
						   genders_attrlist_destroy,
						   "genders_attrlist_create",
						   tests[i].num,
						   tests[i].param1,
						   tests[i].param2,
						   tests[i].expected_return_value,
						   tests[i].expected_errnum,
						   verbose);
      i++;
    }

  return errcount;
}

int
genders_attrlist_clear_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_attrlist_clear_corner_case_t *tests = &genders_attrlist_clear_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_clear_corner_case(genders_attrlist_create,
						  genders_attrlist_clear,
						  genders_attrlist_destroy,
						  "genders_attrlist_clear",
						  tests[i].num,
						  tests[i].param1,
						  tests[i].param2,
						  tests[i].expected_return_value,
						  tests[i].expected_errnum,
						  verbose);
      i++;
    }

  return errcount;
}

int
genders_attrlist_destroy_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_attrlist_destroy_corner_case_t *tests = &genders_attrlist_destroy_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_destroy_corner_case(genders_attrlist_create,
						    genders_attrlist_destroy,
						    "genders_attrlist_destroy",
						    tests[i].num,
						    tests[i].param1,
						    tests[i].param2,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    verbose);
      i++;
    }

  return errcount;
}

int
genders_vallist_create_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_vallist_create_corner_case_t *tests = &genders_vallist_create_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_create_corner_case(genders_vallist_create,
						   genders_vallist_destroy,
						   "genders_vallist_create",
						   tests[i].num,
						   tests[i].param1,
						   tests[i].param2,
						   tests[i].expected_return_value,
						   tests[i].expected_errnum,
						   verbose);
      i++;
    }

  return errcount;
}

int
genders_vallist_clear_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_vallist_clear_corner_case_t *tests = &genders_vallist_clear_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_clear_corner_case(genders_vallist_create,
						  genders_vallist_clear,
						  genders_vallist_destroy,
						  "genders_vallist_clear",
						  tests[i].num,
						  tests[i].param1,
						  tests[i].param2,
						  tests[i].expected_return_value,
						  tests[i].expected_errnum,
						  verbose);
      i++;
    }

  return errcount;
}

int
genders_vallist_destroy_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_vallist_destroy_corner_case_t *tests = &genders_vallist_destroy_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      errcount += _genders_list_destroy_corner_case(genders_vallist_create,
						    genders_vallist_destroy,
						    "genders_vallist_destroy",
						    tests[i].num,
						    tests[i].param1,
						    tests[i].param2,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    verbose);
      i++;
    }

  return errcount;
}

int
genders_getnodename_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getnodename_corner_case_t *tests = &genders_getnodename_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int return_value, errnum, len;
      char buf[MAXHOSTNAMELEN+1];
      char *bufptr;

      handle = genders_handle_get(tests[i].param1);
      bufptr = (tests[i].param2 == GENDERS_POINTER_NULL) ? NULL : buf;
      len = (tests[i].param3 == GENDERS_LENGTH_POSITIVE_LARGE) ? MAXHOSTNAMELEN : tests[i].param3;
      return_value = genders_getnodename(handle, bufptr, len);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_getnodename",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_getnodes_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getnodes_corner_case_t *tests = &genders_getnodes_corner_case_tests[0];
  genders_t gh;
  char **list;
  int list_len;

  gh = genders_handle_get(GENDERS_HANDLE_LOADED);
  if ((list_len = genders_nodelist_create(gh, &list)) < 0)
    genders_err_exit("genders_nodelist_create: %s", genders_errormsg(gh));

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int return_value, errnum, len;
      char **listptr;
      char *attrptr;
      char *valptr;

      handle = genders_handle_get(tests[i].param1);
      listptr = (tests[i].param2 == GENDERS_POINTER_NULL) ? NULL : list;
      len = (tests[i].param3 == GENDERS_LENGTH_POSITIVE_LARGE) ? list_len : tests[i].param3;

      if (tests[i].param4 == GENDERS_STRING_NULL)
	attrptr = NULL;
      else if (tests[i].param4 == GENDERS_STRING_NON_NULL_EMPTY)
	attrptr = "";
      else
	attrptr = genders_database_corner_case.data->attr_with_val;

      if (tests[i].param5 == GENDERS_STRING_NULL)
	valptr = NULL;
      else if (tests[i].param5 == GENDERS_STRING_NON_NULL_EMPTY)
	valptr = "";
      else
	valptr = genders_database_corner_case.data->val;

      return_value = genders_getnodes(handle, listptr, len, attrptr, valptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_getnodes",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  if (genders_nodelist_destroy(gh, list) < 0)
    genders_err_exit("genders_nodelist_destroy: %s", genders_errormsg(gh));
  if (genders_handle_destroy(gh) < 0)
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));

  return errcount;
}

int
genders_getattr_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getattr_corner_case_t *tests = &genders_getattr_corner_case_tests[0];
  genders_t gh;
  char **attrlist, **vallist;
  int attrlist_len;

  gh = genders_handle_get(GENDERS_HANDLE_LOADED);
  if ((attrlist_len = genders_attrlist_create(gh, &attrlist)) < 0)
    genders_err_exit("genders_attrlist_create: %s", genders_errormsg(gh));
  if (genders_vallist_create(gh, &vallist) < 0)
    genders_err_exit("genders_vallist_create: %s", genders_errormsg(gh));

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char **attrlistptr;
      char **vallistptr;
      int return_value, errnum, len;
      char *nodeptr;

      handle = genders_handle_get(tests[i].param1);
      attrlistptr = (tests[i].param2 == GENDERS_POINTER_NULL) ? NULL : attrlist;
      vallistptr = (tests[i].param3 == GENDERS_POINTER_NULL) ? NULL : vallist;
      len = (tests[i].param4 == GENDERS_LENGTH_POSITIVE_LARGE) ? attrlist_len : tests[i].param4;

      if (tests[i].param5 == GENDERS_STRING_NULL)
	nodeptr = NULL;
      else if (tests[i].param5 == GENDERS_STRING_NON_NULL_EMPTY)
	nodeptr = "";
      else
	nodeptr = genders_database_corner_case.data->node;

      return_value = genders_getattr(handle, attrlistptr, vallistptr, len, nodeptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_getattr",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  if (genders_attrlist_destroy(gh, attrlist) < 0)
    genders_err_exit("genders_attrlist_destroy: %s", genders_errormsg(gh));
  if (genders_vallist_destroy(gh, vallist) < 0)
    genders_err_exit("genders_vallist_destroy: %s", genders_errormsg(gh));
  if (genders_handle_destroy(gh) < 0)
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));

  return errcount;
}

int
genders_getattr_all_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_getattr_all_corner_case_t *tests = &genders_getattr_all_corner_case_tests[0];
  genders_t gh;
  char **attrlist;
  int attrlist_len;

  gh = genders_handle_get(GENDERS_HANDLE_LOADED);
  if ((attrlist_len = genders_attrlist_create(gh, &attrlist)) < 0)
    genders_err_exit("genders_attrlist_create: %s", genders_errormsg(gh));

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char **attrlistptr;
      int return_value, errnum, len;

      handle = genders_handle_get(tests[i].param1);
      attrlistptr = (tests[i].param2 == GENDERS_POINTER_NULL) ? NULL : attrlist;
      len = (tests[i].param3 == GENDERS_LENGTH_POSITIVE_LARGE) ? attrlist_len : tests[i].param3;
      return_value = genders_getattr_all(handle, attrlistptr, len);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_getattr_all",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  if (genders_attrlist_destroy(gh, attrlist) < 0)
    genders_err_exit("genders_attrlist_destroy: %s", genders_errormsg(gh));
  if (genders_handle_destroy(gh) < 0)
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));

  return errcount;
}

int
genders_testattr_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_testattr_corner_case_t *tests = &genders_testattr_corner_case_tests[0];
  genders_t gh;
  char *valbuf;
  int maxvallen;

  gh = genders_handle_get(GENDERS_HANDLE_LOADED);
  if ((maxvallen = genders_getmaxvallen(gh)) < 0)
    genders_err_exit("genders_getmaxvallen: %s", genders_errormsg(gh));
  if (!(valbuf = (char *)malloc(maxvallen + 1)))
    genders_err_exit("malloc: %s", strerror(errno));

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *nodeptr, *attrptr, *valptr;
      int return_value, errnum, len;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	nodeptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	nodeptr = "";
      else
	nodeptr = genders_database_corner_case.data->node;

      if (tests[i].param3 == GENDERS_STRING_NULL)
	attrptr = NULL;
      else if (tests[i].param3 == GENDERS_STRING_NON_NULL_EMPTY)
	attrptr = "";
      else
	attrptr = genders_database_corner_case.data->attr_with_val;

      valptr = (tests[i].param4 == GENDERS_POINTER_NULL) ? NULL: valbuf;
      len = (tests[i].param5 == GENDERS_LENGTH_POSITIVE_LARGE) ? maxvallen + 1: tests[i].param5;
      return_value = genders_testattr(handle, nodeptr, attrptr, valptr, len);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_testattr",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  free(valbuf);
  if (genders_handle_destroy(gh) < 0)
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));

  return errcount;
}

int
genders_testattrval_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_testattrval_corner_case_t *tests = &genders_testattrval_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *nodeptr, *attrptr, *valptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	nodeptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	nodeptr = "";
      else
	nodeptr = genders_database_corner_case.data->node;

      if (tests[i].param3 == GENDERS_STRING_NULL)
	attrptr = NULL;
      else if (tests[i].param3 == GENDERS_STRING_NON_NULL_EMPTY)
	attrptr = "";
      else
	attrptr = genders_database_corner_case.data->attr_with_val;

      if (tests[i].param4 == GENDERS_STRING_NULL)
	valptr = NULL;
      else if (tests[i].param4 == GENDERS_STRING_NON_NULL_EMPTY)
	valptr = "";
      else
	valptr = genders_database_corner_case.data->val;

      return_value = genders_testattrval(handle, nodeptr, attrptr, valptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_testattrval",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_isnode_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_isnode_corner_case_t *tests = &genders_isnode_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *nodeptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	nodeptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	nodeptr = "";
      else
	nodeptr = genders_database_corner_case.data->node;
      
      return_value = genders_isnode(handle, nodeptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_isnode",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_isattr_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_isattr_corner_case_t *tests = &genders_isattr_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *attrptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	attrptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	attrptr = "";
      else
	attrptr = genders_database_corner_case.data->attr_without_val;

      return_value = genders_isattr(handle, attrptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_isattr",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_isattrval_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_isattrval_corner_case_t *tests = &genders_isattrval_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *attrptr, *valptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	attrptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	attrptr = "";
      else
	attrptr = genders_database_corner_case.data->attr_with_val;

      if (tests[i].param3 == GENDERS_STRING_NULL)
	valptr = NULL;
      else if (tests[i].param3 == GENDERS_STRING_NON_NULL_EMPTY)
	valptr = "";
      else
	valptr = genders_database_corner_case.data->val;

      return_value = genders_isattrval(handle, attrptr, valptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_isattrval",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_index_attrvals_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_index_attrvals_corner_case_t *tests = &genders_index_attrvals_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *attrptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	attrptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	attrptr = "";
      else
	attrptr = genders_database_corner_case.data->attr_with_val;

      return_value = genders_index_attrvals(handle, attrptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_index_attrvals",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_query_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_query_corner_case_t *tests = &genders_query_corner_case_tests[0];
  genders_t gh;
  char **list;
  int list_len;

  gh = genders_handle_get(GENDERS_HANDLE_LOADED);
  if ((list_len = genders_nodelist_create(gh, &list)) < 0)
    genders_err_exit("genders_nodelist_create: %s", genders_errormsg(gh));

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int return_value, errnum, len;
      char **listptr;
      char *queryptr;

      handle = genders_handle_get(tests[i].param1);
      listptr = (tests[i].param2 == GENDERS_POINTER_NULL) ? NULL : list;
      len = (tests[i].param3 == GENDERS_LENGTH_POSITIVE_LARGE) ? list_len : tests[i].param3;
      if (tests[i].param4 == GENDERS_STRING_NULL)
	queryptr = NULL;
      else if (tests[i].param4 == GENDERS_STRING_NON_NULL_EMPTY)
	queryptr = "";
      else
	queryptr = genders_database_corner_case.data->attr_with_val;
      return_value = genders_query(handle, listptr, len, queryptr);
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_query",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  if (genders_nodelist_destroy(gh, list) < 0)
    genders_err_exit("genders_nodelist_destroy: %s", genders_errormsg(gh));
  if (genders_handle_destroy(gh) < 0)
    genders_err_exit("genders_handle_destroy: %s", genders_errormsg(gh));

  return errcount;
}

int
genders_testquery_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_testquery_corner_case_t *tests = &genders_testquery_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *nodeptr, *queryptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);

      if (tests[i].param2 == GENDERS_STRING_NULL)
	nodeptr = NULL;
      else if (tests[i].param2 == GENDERS_STRING_NON_NULL_EMPTY)
	nodeptr = "";
      else
	nodeptr = genders_database_corner_case.data->node;

      if (tests[i].param3 == GENDERS_STRING_NULL)
	queryptr = NULL;
      else if (tests[i].param3 == GENDERS_STRING_NON_NULL_EMPTY)
	queryptr = "";
      else
	queryptr = genders_database_corner_case.data->attr_with_val;

      return_value = genders_testquery(handle, nodeptr, queryptr);
      errnum = genders_errnum(handle);
      
      errcount += genders_return_value_errnum_check("genders_testquery",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);
      
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_parse_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_parse_corner_case_t *tests = &genders_parse_corner_case_tests[0];
  int stderr_save;
  int dev_null;

  /* Special case for tests.  It depends on if the default genders
   * file exists on the system and the tester has access to it.
   */
  {
    struct stat buf;

    if (stat(GENDERS_DEFAULT_FILE, &buf) < 0)
      {
	tests[8].expected_return_value = -1;
	tests[8].expected_errnum = GENDERS_ERR_OPEN;
	tests[9].expected_return_value = -1;
	tests[9].expected_errnum = GENDERS_ERR_OPEN;
	tests[10].expected_return_value = -1;
	tests[10].expected_errnum = GENDERS_ERR_OPEN;
	tests[11].expected_return_value = -1;
	tests[11].expected_errnum = GENDERS_ERR_OPEN;

	tests[24].expected_return_value = -1;
	tests[24].expected_errnum = GENDERS_ERR_OPEN;
	tests[25].expected_return_value = -1;
	tests[25].expected_errnum = GENDERS_ERR_OPEN;
	tests[26].expected_return_value = -1;
	tests[26].expected_errnum = GENDERS_ERR_OPEN;
	tests[27].expected_return_value = -1;
	tests[27].expected_errnum = GENDERS_ERR_OPEN;
      }
  }

  if ((stderr_save = dup(STDERR_FILENO)) < 0)
    genders_err_exit("dup: %s", strerror(errno));
  if ((dev_null = open(_PATH_DEVNULL, O_APPEND)) < 0)
    genders_err_exit("open: %s: %s", _PATH_DEVNULL, strerror(errno));

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      char *filename;
      FILE *streamptr;
      int return_value, errnum;

      handle = genders_handle_get(tests[i].param1);
      filename = genders_filename_get(tests[i].param2);
      streamptr = (tests[i].param3 == GENDERS_POINTER_NULL) ? NULL : stderr;
      /* Must route stderr somewhere else during call to genders_parse() */
      if ((dup2(dev_null, STDERR_FILENO)) < 0)
	genders_err_exit("dup2: %s", strerror(errno));
      return_value = genders_parse(handle, filename, streamptr);
      if ((dup2(stderr_save, STDERR_FILENO)) < 0)
	genders_err_exit("dup2: %s", strerror(errno));
      errnum = genders_errnum(handle);

      errcount += genders_return_value_errnum_check("genders_parse",
						    tests[i].num,
						    tests[i].expected_return_value,
						    tests[i].expected_errnum,
						    return_value,
						    errnum,
						    NULL,
						    verbose);

      genders_handle_cleanup(handle);
      i++;
    }

  close(dev_null);
  return errcount;
}

int
genders_set_errnum_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_set_errnum_corner_case_t *tests = &genders_set_errnum_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      int err, errnum;

      handle = genders_handle_get(tests[i].param1);
      genders_set_errnum(handle, tests[i].param2);
      errnum = genders_errnum(handle);
 
      err = genders_errnum_check("genders_set_errnum",
				 tests[i].num,
				 tests[i].expected_errnum,
				 errnum,
				 NULL,
				 verbose);

      errcount += err;
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}

int
genders_copy_corner_case(int verbose)
{
  int i = 0;
  int errcount = 0;
  genders_copy_corner_case_t *tests = &genders_copy_corner_case_tests[0];

  while (!(tests[i].num < 0)) 
    {
      genders_t handle;
      genders_t return_value;
      int errnum;

      handle = genders_handle_get(tests[i].param1);
      return_value = genders_copy(handle);
      errnum = genders_errnum(handle);
 
      errcount += genders_return_value_pointer_errnum_check("genders_copy",
							    tests[i].num,
							    tests[i].expected_return_value,
							    tests[i].expected_errnum,
							    return_value,
							    errnum,
							    NULL,
							    verbose);
      
      genders_handle_cleanup(handle);
      i++;
    }

  return errcount;
}
