/*****************************************************************************\
 *  $Id: genders.c,v 1.5 2010-02-02 00:04:34 chu11 Exp $
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_genders.h"

#include <genders.h>

static int le_genders;

zend_function_entry genders_functions[] = {
	PHP_FE(genders_getnumattrs,	NULL)
	PHP_FE(genders_getattr,		NULL)
	PHP_FE(genders_getattr_all,	NULL)
	PHP_FE(genders_getnodes,	NULL)
	{NULL, NULL, NULL}
};


zend_module_entry genders_module_entry = {
	STANDARD_MODULE_HEADER,
	"genders",
	genders_functions,
	PHP_MINIT(genders),
	PHP_MSHUTDOWN(genders),
	PHP_RINIT(genders),	
	PHP_RSHUTDOWN(genders),
	PHP_MINFO(genders),
	NO_VERSION_YET,
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_GENDERS
ZEND_GET_MODULE(genders)
#endif

PHP_MINIT_FUNCTION(genders)
{
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(genders)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(genders)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(genders)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(genders)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "genders support", "enabled");
	php_info_print_table_end();
}

/* {{{ proto int genders_getnumattrs (string file) */
PHP_FUNCTION(genders_getnumattrs)
{
  genders_t ghandle;
  unsigned long retval;
  zval *file;

  if (zend_get_parameters(ht, 1, &file) == SUCCESS)
  {
    ghandle = genders_handle_create();
    genders_load_data(ghandle, file->value.str.val);

    retval = genders_getnumattrs(ghandle);

    if(retval < 0)
    {
      php_error(E_WARNING, genders_strerror(genders_errnum(ghandle)));
      RETURN_FALSE;
    }

    RETVAL_LONG(retval);
    genders_handle_destroy(ghandle);
  }
  else
  {
    WRONG_PARAM_COUNT;
  }
  return;
}

/* {{{ proto string array genders_getattr (string file, string node, bool ret_type) */
/* if ret_type is 0, return the attribute list.  Otherwise, return the value list. */
PHP_FUNCTION(genders_getattr)
{
  genders_t ghandle;
  zval *file;
  zval *node;
  zval *ret_type;
  char **attrlist;
  char **vallist;
  int len, num, k;

  if ( ZEND_NUM_ARGS() == 3 && zend_get_parameters(ht, 3, &file, &node, &ret_type) == SUCCESS)
  {
    ghandle = genders_handle_create();
    genders_load_data(ghandle, file->value.str.val);

    len = genders_attrlist_create(ghandle, &attrlist);
    genders_vallist_create(ghandle, &vallist);

    num = genders_getattr(ghandle, attrlist, vallist, len, node->value.str.val);
    if(num < 0)
    {
      if(genders_errnum(ghandle) == GENDERS_ERR_NOTFOUND)
      {
        RETURN_FALSE;
      }
      else
      {
        php_error(E_WARNING, genders_strerror(genders_errnum(ghandle)));
        RETURN_FALSE;
      }
    }

    array_init(return_value);

    if(ret_type->value.lval)
    {
      for(k=0;k<num;k++)
      {
        add_next_index_string(return_value, vallist[k], 1);
      }
    }
    else
    {
      for(k=0;k<num;k++)
      {
        add_next_index_string(return_value, attrlist[k], 1);
      }
    }

    genders_attrlist_destroy(ghandle, attrlist);
    genders_vallist_destroy(ghandle, vallist);

    genders_handle_destroy(ghandle);
  }
  else
  {
    WRONG_PARAM_COUNT;
  }
  return;
}

/* {{{ proto string array genders_getnodes (string file, string attr, string val) */
PHP_FUNCTION(genders_getnodes)
{
  genders_t ghandle;
  zval *file;
  zval *attr;
  zval *val;
  char **nodelist;
  int len, num, k;

  if ( ZEND_NUM_ARGS() == 3 && zend_get_parameters(ht, 3, &file, &attr, &val) == SUCCESS)
  {
    ghandle = genders_handle_create();
    genders_load_data(ghandle, file->value.str.val);

    len = genders_nodelist_create(ghandle, &nodelist);

    num = genders_getnodes(ghandle, nodelist, len, attr->value.str.val, val->value.str.val);
    if(num < 0)
    {
      php_error(E_WARNING, genders_strerror(genders_errnum(ghandle)));
      RETURN_FALSE;
    }

    array_init(return_value);

    for(k=0;k<num;k++)
    {
      add_next_index_string(return_value, nodelist[k], 1);
    }

    genders_nodelist_destroy(ghandle, nodelist);
    genders_handle_destroy(ghandle);
  }
  else
  {
    WRONG_PARAM_COUNT;
  }
  return;
}

/* {{{ proto string array genders_getattr_all (string file) */
PHP_FUNCTION(genders_getattr_all)
{
  genders_t ghandle;
  zval *file;
  char **attrlist;
  int len, num, k;

  if ( ZEND_NUM_ARGS() == 1 && zend_get_parameters(ht, 1, &file) == SUCCESS)
  {
    ghandle = genders_handle_create();
    genders_load_data(ghandle, file->value.str.val);

    len = genders_attrlist_create(ghandle, &attrlist);

    num = genders_getattr_all(ghandle, attrlist, len);
    if(num < 0)
    {
      php_error(E_WARNING, genders_strerror(genders_errnum(ghandle)));
      RETURN_FALSE;
    }

    array_init(return_value);

    for(k=0;k<num;k++)
    {
      add_next_index_string(return_value, attrlist[k], 1);
    }

    genders_attrlist_destroy(ghandle, attrlist);
    genders_handle_destroy(ghandle);
  }
  else
  {
    WRONG_PARAM_COUNT;
  }
  return;
}
