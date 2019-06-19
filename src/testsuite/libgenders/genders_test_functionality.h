/*****************************************************************************\
 *  $Id: genders_test_functionality.h,v 1.7 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_TEST_FUNCTIONALITY_H
#define _GENDERS_TEST_FUNCTIONALITY_H 1

#include "genders.h"
#include "genders_testlib.h"

typedef int (*GendersFunctionalityFunc)(int);

int genders_handle_create_functionality(int verbose);
int genders_handle_destroy_functionality(int verbose);
int genders_load_data_functionality(int verbose);
int genders_errnum_functionality(int verbose);
int genders_strerror_functionality(int verbose);
int genders_errormsg_functionality(int verbose);
int genders_perror_functionality(int verbose);
int genders_get_flags_functionality(int verbose);
int genders_set_flags_functionality(int verbose);
int genders_getnumnodes_functionality(int verbose);
int genders_getnumattrs_functionality(int verbose);
int genders_getmaxattrs_functionality(int verbose);
int genders_getmaxnodelen_functionality(int verbose);
int genders_getmaxattrlen_functionality(int verbose);
int genders_getmaxvallen_functionality(int verbose);
int genders_nodelist_create_functionality(int verbose);
int genders_nodelist_clear_functionality(int verbose);
int genders_nodelist_destroy_functionality(int verbose);
int genders_attrlist_create_functionality(int verbose);
int genders_attrlist_clear_functionality(int verbose);
int genders_attrlist_destroy_functionality(int verbose);
int genders_vallist_create_functionality(int verbose);
int genders_vallist_clear_functionality(int verbose);
int genders_vallist_destroy_functionality(int verbose);
int genders_getnodename_functionality(int verbose);
int genders_getnodes_functionality(int verbose);
int genders_getattr_functionality(int verbose);
int genders_getattr_all_functionality(int verbose);
int genders_testattr_functionality(int verbose);
int genders_testattrval_functionality(int verbose);
int genders_isnode_functionality(int verbose);
int genders_isattr_functionality(int verbose);
int genders_isattrval_functionality(int verbose);
int genders_index_attrvals_functionality(int verbose);
int genders_query_functionality(int verbose);
int genders_testquery_functionality(int verbose);
int genders_parse_functionality(int verbose);
int genders_set_errnum_functionality(int verbose);
int genders_copy_functionality(int verbose);

#endif /* _GENDERS_TEST_FUNCTIONALITY_H */
