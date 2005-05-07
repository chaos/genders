/*****************************************************************************\
 *  $Id: genders_parsing.h,v 1.4 2005-05-07 16:49:40 achu Exp $
 *****************************************************************************
 *  Copyright (C) 2001-2003 The Regents of the University of California.
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
 *  with Genders; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
\*****************************************************************************/

#ifndef _GENDERS_PARSING_H
#define _GENDERS_PARSING_H 1

#include <stdio.h>

#include "genders.h"
#include "genders_util.h"
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
			    List nodeslist,
			    List attrvalslist,
			    int debug,
			    FILE *stream);

/* 
 * _genders_index_nodes
 *
 * Create and populate node index
 *
 * Returns 0 on success, -1 on error
 */
int _genders_index_nodes(genders_t handle);

/* 
 * _genders_index_attrs
 *
 * Create and populate node index
 *
 * Returns 0 on success, -1 on error
 */
int _genders_index_attrs(genders_t handle);

#endif /* _GENDERS_PARSING_H */
