/*****************************************************************************\
 *  $Id: genders_parsing.h,v 1.10 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef _GENDERS_PARSING_H
#define _GENDERS_PARSING_H 1

#include <stdio.h>

#include "genders.h"
#include "genders_util.h"
#include "hash.h"
#include "list.h"

/* 
 * _genders_open_and_parse
 *
 * Common file open and file parsing function for genders_load_data
 * and genders_parse.
 *
 * Returns 0 on success, -1 on error
 */
int _genders_open_and_parse(genders_t handle,
			    const char *filename,
			    int *numattrs,
			    int *maxattrs,
			    int *maxnodelen,
			    int *maxattrlen,
			    int *maxvallen,
			    List nodeslist,
			    List attrvalslist,
			    List attrslist,
                            hash_t *node_index,
                            int *node_index_size,
                            hash_t *attr_index,
                            int *attr_index_size,
			    int debug,
			    FILE *stream);

#endif /* _GENDERS_PARSING_H */
