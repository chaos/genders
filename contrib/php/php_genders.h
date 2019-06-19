/*****************************************************************************\
 *  $Id: php_genders.h,v 1.4 2010-02-02 00:04:34 chu11 Exp $
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

#ifndef PHP_GENDERS_H
#define PHP_GENDERS_H

extern zend_module_entry genders_module_entry;
#define phpext_genders_ptr &genders_module_entry

#ifdef PHP_WIN32
#define PHP_GENDERS_API __declspec(dllexport)
#else
#define PHP_GENDERS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(genders);
PHP_MSHUTDOWN_FUNCTION(genders);
PHP_RINIT_FUNCTION(genders);
PHP_RSHUTDOWN_FUNCTION(genders);
PHP_MINFO_FUNCTION(genders);

PHP_FUNCTION(genders_getnumattrs);
PHP_FUNCTION(genders_getattr);
PHP_FUNCTION(genders_getattr_all);
PHP_FUNCTION(genders_getnodes);

#ifdef ZTS
#define GENDERS_G(v) TSRMG(genders_globals_id, zend_genders_globals *, v)
#else
#define GENDERS_G(v) (genders_globals.v)
#endif

#endif
