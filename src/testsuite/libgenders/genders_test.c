/*****************************************************************************\
 *  $Id: genders_test.c,v 1.10 2010-02-02 00:04:34 chu11 Exp $
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#if HAVE_GETOPT_H
#include <getopt.h>
#endif /* HAVE_GETOPT_H */

#include "genders.h"
#include "genders_testlib.h"
#include "genders_test_corner_case.h"
#include "genders_test_functionality.h"

static int verbose = 0;
static int all_tests = 1;
static int corner_case_tests = 0;
static int functionality_tests = 0;

static void
_usage(void)
{
  fprintf(stderr, 
	  "Usage: genders_test [OPTIONS]\n"
	  "-h        output usage\n"
	  "-v        verbose output (can be used multiple times)\n"
	  "-a        run all tests (default)\n"
	  "-c        run only corner case tests\n"
	  "-f        run only functionality tests\n"
	  );
  exit(1);
}

static void 
_cmdline_parse(int argc, char **argv)
{
  int c;

  while ((c = getopt(argc, argv, "hvacf")) != -1)
    {
      switch (c)
	{
	case 'h':
	  _usage();
	case 'v':
	  verbose++;
	  break;
	case 'a':
	  all_tests++;
	  break;
	case 'c':
	  corner_case_tests++;
	  all_tests = 0;
	  break;
	case 'f':
	  functionality_tests++;
	  all_tests = 0;
	  break;
	default:
	  fprintf(stderr, "command line option error");
	  exit(1);
	}
    }
}

static int
_corner_case(GendersCornerCaseFunc func, char *funcname)
{
  int errcount = 0;

  if (verbose > 2)
    {
      fprintf(stderr, "***************************************************\n");
      fprintf(stderr, "* Testing \"%s\" corner cases\n", funcname);                
      fprintf(stderr, "***************************************************\n");
    }

  errcount = func(verbose);

  if (verbose > 2)
    {
      fprintf(stderr, "Total \"%s\" corner case errors: %d\n", funcname, errcount);
    }

  return errcount;
}

static int
_test_corner_cases(void)
{
  int errtotal = 0;
  
  errtotal += _corner_case(genders_handle_create_corner_case, "genders_handle_create");
  errtotal += _corner_case(genders_handle_destroy_corner_case, "genders_handle_destroy");
  errtotal += _corner_case(genders_load_data_corner_case, "genders_load_data");
  errtotal += _corner_case(genders_errnum_corner_case, "genders_errnum");
  errtotal += _corner_case(genders_strerror_corner_case, "genders_strerror");
  errtotal += _corner_case(genders_errormsg_corner_case, "genders_errormsg");
  errtotal += _corner_case(genders_perror_corner_case, "genders_perror");
  errtotal += _corner_case(genders_get_flags_corner_case, "genders_get_flags");
  errtotal += _corner_case(genders_set_flags_corner_case, "genders_set_flags");
  errtotal += _corner_case(genders_getnumnodes_corner_case, "genders_getnumnodes");
  errtotal += _corner_case(genders_getnumattrs_corner_case, "genders_getnumattrs");
  errtotal += _corner_case(genders_getmaxattrs_corner_case, "genders_getmaxattrs");
  errtotal += _corner_case(genders_getmaxnodelen_corner_case, "genders_getmaxnodelen");
  errtotal += _corner_case(genders_getmaxattrlen_corner_case, "genders_getmaxattrlen");
  errtotal += _corner_case(genders_getmaxvallen_corner_case, "genders_getmaxvallen");
  errtotal += _corner_case(genders_nodelist_create_corner_case, "genders_nodelist_create");
  errtotal += _corner_case(genders_nodelist_clear_corner_case, "genders_nodelist_clear");
  errtotal += _corner_case(genders_nodelist_destroy_corner_case, "genders_nodelist_destroy");
  errtotal += _corner_case(genders_attrlist_create_corner_case, "genders_attrlist_create");
  errtotal += _corner_case(genders_attrlist_clear_corner_case, "genders_attrlist_clear");
  errtotal += _corner_case(genders_attrlist_destroy_corner_case, "genders_attrlist_destroy");
  errtotal += _corner_case(genders_vallist_create_corner_case, "genders_vallist_create");
  errtotal += _corner_case(genders_vallist_clear_corner_case, "genders_vallist_clear");
  errtotal += _corner_case(genders_vallist_destroy_corner_case, "genders_vallist_destroy");
  errtotal += _corner_case(genders_getnodename_corner_case, "genders_getnodename");
  errtotal += _corner_case(genders_getnodes_corner_case, "genders_getnodes");
  errtotal += _corner_case(genders_getattr_corner_case, "genders_getattr");
  errtotal += _corner_case(genders_getattr_all_corner_case, "genders_getattr_all");
  errtotal += _corner_case(genders_testattr_corner_case, "genders_testattr"); 
  errtotal += _corner_case(genders_testattrval_corner_case, "genders_testattrval");
  errtotal += _corner_case(genders_isnode_corner_case, "genders_isnode");
  errtotal += _corner_case(genders_isattr_corner_case, "genders_isattr");
  errtotal += _corner_case(genders_isattrval_corner_case, "genders_isattrval");
  errtotal += _corner_case(genders_index_attrvals_corner_case, "genders_index_attrvals");
  errtotal += _corner_case(genders_query_corner_case, "genders_query");
  errtotal += _corner_case(genders_testquery_corner_case, "genders_testquery"); 
  errtotal += _corner_case(genders_parse_corner_case, "genders_parse");
  errtotal += _corner_case(genders_set_errnum_corner_case, "genders_set_errnum");
  errtotal += _corner_case(genders_copy_corner_case, "genders_copy");

  return errtotal;
}

static int
_functionality(GendersCornerCaseFunc func, char *funcname)
{
  int errcount = 0;

  if (verbose > 2)
    {
      fprintf(stderr, "***************************************************\n");
      fprintf(stderr, "* Testing \"%s\" functionality\n", funcname);                
      fprintf(stderr, "***************************************************\n");
    }

  errcount = func(verbose);

  if (verbose > 2)
    {
      fprintf(stderr, "Total \"%s\" functionality errors: %d\n", funcname, errcount);
    }

  return errcount;
}

static int
_test_functionality(void)
{
  int errtotal = 0;
  
  errtotal += _functionality(genders_handle_create_functionality, "genders_handle_create");
  errtotal += _functionality(genders_handle_destroy_functionality, "genders_handle_destroy");
  errtotal += _functionality(genders_load_data_functionality, "genders_load_data");
  errtotal += _functionality(genders_errnum_functionality, "genders_errnum");
  errtotal += _functionality(genders_strerror_functionality, "genders_strerror");
  errtotal += _functionality(genders_errormsg_functionality, "genders_errormsg");
  errtotal += _functionality(genders_perror_functionality, "genders_perror");
  errtotal += _functionality(genders_get_flags_functionality, "genders_get_flags");
  errtotal += _functionality(genders_set_flags_functionality, "genders_set_flags");
  errtotal += _functionality(genders_getnumnodes_functionality, "genders_getnumnodes");
  errtotal += _functionality(genders_getnumattrs_functionality, "genders_getnumattrs");
  errtotal += _functionality(genders_getmaxattrs_functionality, "genders_getmaxattrs");
  errtotal += _functionality(genders_getmaxnodelen_functionality, "genders_getmaxnodelen");
  errtotal += _functionality(genders_getmaxattrlen_functionality, "genders_getmaxattrlen");
  errtotal += _functionality(genders_getmaxvallen_functionality, "genders_getmaxvallen");
  errtotal += _functionality(genders_nodelist_create_functionality, "genders_nodelist_create");
  errtotal += _functionality(genders_nodelist_clear_functionality, "genders_nodelist_clear");
  errtotal += _functionality(genders_nodelist_destroy_functionality, "genders_nodelist_destroy");
  errtotal += _functionality(genders_attrlist_create_functionality, "genders_attrlist_create");
  errtotal += _functionality(genders_attrlist_clear_functionality, "genders_attrlist_clear");
  errtotal += _functionality(genders_attrlist_destroy_functionality, "genders_attrlist_destroy");
  errtotal += _functionality(genders_vallist_create_functionality, "genders_vallist_create");
  errtotal += _functionality(genders_vallist_clear_functionality, "genders_vallist_clear");
  errtotal += _functionality(genders_vallist_destroy_functionality, "genders_vallist_destroy");
  errtotal += _functionality(genders_getnodename_functionality, "genders_getnodename");
  errtotal += _functionality(genders_getnodes_functionality, "genders_getnodes");
  errtotal += _functionality(genders_getattr_functionality, "genders_getattr");
  errtotal += _functionality(genders_getattr_all_functionality, "genders_getattr_all");
  errtotal += _functionality(genders_testattr_functionality, "genders_testattr"); 
  errtotal += _functionality(genders_testattrval_functionality, "genders_testattrval");
  errtotal += _functionality(genders_isnode_functionality, "genders_isnode");
  errtotal += _functionality(genders_isattr_functionality, "genders_isattr");
  errtotal += _functionality(genders_isattrval_functionality, "genders_isattrval");
  errtotal += _functionality(genders_index_attrvals_functionality, "genders_index_attrvals");
  errtotal += _functionality(genders_query_functionality, "genders_query");
  errtotal += _functionality(genders_testquery_functionality, "genders_testquery");
  errtotal += _functionality(genders_parse_functionality, "genders_parse");
  errtotal += _functionality(genders_set_errnum_functionality, "genders_set_errnum");
  errtotal += _functionality(genders_copy_functionality, "genders_copy");

  return errtotal;
}

int
main(int argc, char **argv)
{
  int errcount, errtotal = 0;
  genders_err_init(argv[0]);
  _cmdline_parse(argc, argv);

  if (all_tests || corner_case_tests)
    {
      errcount = _test_corner_cases();
      if ((verbose && errcount)
	  || (verbose > 1))
	fprintf(stderr, "Total Corner Case Failures: %d\n", errcount);
      errtotal += errcount;
    }
  
  if (all_tests || functionality_tests)
    {
      errcount = _test_functionality();
      if ((verbose && errcount)
	  || (verbose > 1))
	fprintf(stderr, "Total Functionality Failures: %d\n", errcount);
      errtotal += errcount;
    }

  if (all_tests 
      || corner_case_tests 
      || functionality_tests)
    {
      if ((verbose && errtotal)
	  || (verbose > 1))
	fprintf(stderr, "Total Failures: %d\n", errtotal);
    }

  exit((errtotal) ? 1 : 0);
}
