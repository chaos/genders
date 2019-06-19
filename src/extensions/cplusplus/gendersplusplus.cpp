/*****************************************************************************\
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

#include <stdlib.h>
#include <string.h>
#include <gendersplusplus.hpp>

using namespace std;
using namespace Gendersplusplus;

GendersException::GendersException()
{
  _errnum = GENDERS_ERR_ERRNUMRANGE;
}

GendersException::GendersException(int errnum)
{
  _errnum = errnum;
}

const char *GendersException::errormsg() const
{
  return genders_strerror(_errnum);
}

ostream &operator<<(ostream &output, const GendersException &e)
{
  output << e.errormsg();
  return output;
}

GendersExceptionOpen::GendersExceptionOpen() : GendersException(GENDERS_ERR_OPEN)
{
}

GendersExceptionRead::GendersExceptionRead() : GendersException(GENDERS_ERR_READ)
{
}

GendersExceptionParse::GendersExceptionParse() : GendersException(GENDERS_ERR_PARSE)
{
}

GendersExceptionParameters::GendersExceptionParameters() : GendersException(GENDERS_ERR_PARAMETERS)
{
}

GendersExceptionNotfound::GendersExceptionNotfound() : GendersException(GENDERS_ERR_NOTFOUND)
{
}

GendersExceptionSyntax::GendersExceptionSyntax() : GendersException(GENDERS_ERR_SYNTAX)
{
}

GendersExceptionInternal::GendersExceptionInternal() : GendersException(GENDERS_ERR_INTERNAL)
{
}

void Genders::_throw_exception(int errnum) const
{
  switch (errnum)
    {
    case GENDERS_ERR_OPEN:
      throw GendersExceptionOpen();
    case GENDERS_ERR_READ:
      throw GendersExceptionRead();
    case GENDERS_ERR_PARSE:
      throw GendersExceptionParse();
    case GENDERS_ERR_NOTFOUND:
      throw GendersExceptionNotfound();
    case GENDERS_ERR_SYNTAX:
      throw GendersExceptionSyntax();
    case GENDERS_ERR_OUTMEM:
      throw std::bad_alloc();
    default:
      throw GendersExceptionInternal();
    }
}

void Genders::_constructor(const string filename)
{
  if (!(gh = genders_handle_create()))
    _throw_exception(GENDERS_ERR_OUTMEM);
  
  if (genders_load_data(gh, filename.c_str()) < 0)
    {
      int errnum = genders_errnum(gh);
      genders_handle_destroy(gh);
      _throw_exception(errnum);
    }
}

Genders::Genders()
{
  _constructor("");
}

Genders::Genders(const string filename)
{
  _constructor(filename);
}

Genders::Genders(const Genders &copy)
{
  if (!(gh = genders_copy(copy.gh)))
    _throw_exception(genders_errnum(copy.gh));
}

const Genders &Genders::operator=(const Genders &right)
{
  if (&right != this)
    {
      genders_handle_destroy(this->gh);
      if (!(this->gh = genders_copy(right.gh)))
	_throw_exception(genders_errnum(right.gh));
    }

  return *this;
}

Genders::~Genders()
{
  (void)genders_handle_destroy(gh);
}

unsigned int Genders::getnumnodes() const
{
  int rv;

  if ((rv = genders_getnumnodes(gh)) < 0)
    _throw_exception(genders_errnum(gh));

  return rv;
}

unsigned int Genders::getnumattrs() const
{
  int rv;

  if ((rv = genders_getnumattrs(gh)) < 0)
    _throw_exception(genders_errnum(gh));

  return rv;
}

unsigned int Genders::getmaxattrs() const
{
  int rv;

  if ((rv = genders_getmaxattrs(gh)) < 0)
    _throw_exception(genders_errnum(gh));

  return rv;
}

string Genders::getnodename() const
{
  string rv;
  char *buf = NULL;
  int maxnodelen;

  if ((maxnodelen = genders_getmaxnodelen(gh)) < 0)
    _throw_exception(genders_errnum(gh));

  if (!(buf = (char *)malloc(maxnodelen + 1)))
    _throw_exception(genders_errnum(gh));

  if (genders_getnodename(gh, buf, maxnodelen + 1) < 0)
    {
      free(buf);
      _throw_exception(genders_errnum(gh));
    }

  rv = buf;
  free(buf);
  return rv;
}

vector< string > Genders::getnodes(const string attr, const string val) const
{
  vector<string> rv;
  char **nodelist = NULL;
  int nodelist_len;
  int nodelist_count;

  if ((nodelist_len = genders_nodelist_create(gh, &nodelist)) < 0)
    _throw_exception(genders_errnum(gh));

  if ((nodelist_count = genders_getnodes(gh,
					 nodelist,
					 nodelist_len,
					 attr.c_str(),
					 val.c_str())) < 0)
    {
      int errnum = genders_errnum(gh);
      genders_nodelist_destroy(gh, nodelist);
      _throw_exception(errnum);
    }
  
  for (int i = 0; i < nodelist_count; i++)
    rv.push_back(nodelist[i]);
  
  genders_nodelist_destroy(gh, nodelist);
  return rv;
}

vector< pair< string, string > > Genders::getattr(const std::string node) const
{
  vector< pair< string, string> > rv;
  char **attrlist = NULL;
  int attrlist_len;
  char **vallist = NULL;
  int vallist_len;
  int list_count;

  if ((attrlist_len = genders_attrlist_create(gh, &attrlist)) < 0)
    _throw_exception(genders_errnum(gh));

  if ((vallist_len = genders_vallist_create(gh, &vallist)) < 0)
    {
      int errnum = genders_errnum(gh);
      genders_attrlist_destroy(gh, attrlist);
      _throw_exception(errnum);
    }

  if ((list_count = genders_getattr(gh,
				    attrlist,
				    vallist,
				    attrlist_len,
				    node.c_str())) < 0)
    {
      int errnum = genders_errnum(gh);
      genders_attrlist_destroy(gh, attrlist);
      genders_vallist_destroy(gh, vallist);
      _throw_exception(errnum);
    }

  for (int i = 0; i < list_count; i++)
    rv.push_back(pair<string, string>(attrlist[i], vallist[i]));

  genders_attrlist_destroy(gh, attrlist);
  genders_vallist_destroy(gh, vallist);
  return rv;
}

vector< string > Genders::getattr_all() const
{
  vector<string> rv;
  char **attrlist = NULL;
  int attrlist_len;
  int attrlist_count;

  if ((attrlist_len = genders_attrlist_create(gh, &attrlist)) < 0)
    _throw_exception(genders_errnum(gh));

  if ((attrlist_count = genders_getattr_all(gh,
					    attrlist,
					    attrlist_len)) < 0)
    {
      int errnum = genders_errnum(gh);
      genders_attrlist_destroy(gh, attrlist);
      _throw_exception(errnum);
    }
  
  for (int i = 0; i < attrlist_count; i++)
    rv.push_back(attrlist[i]);

  genders_attrlist_destroy(gh, attrlist);
  return rv;
}

bool Genders::testattr(const string attr, string &val, const string node) const
{
  char *valbuf = NULL;
  int maxvallen;
  int ret;
  bool rv = false;
  
  if ((maxvallen = genders_getmaxvallen(gh)) < 0)
    _throw_exception(genders_errnum(gh));
  
  if (!(valbuf = (char *)malloc(maxvallen + 1)))
    _throw_exception(genders_errnum(gh));

  memset(valbuf, '\0', maxvallen + 1);

  if ((ret = genders_testattr(gh,
			      node.c_str(),
			      attr.c_str(),
			      valbuf,
			      maxvallen + 1)) < 0)
    {
      free(valbuf);
      _throw_exception(genders_errnum(gh));
    }

  if (ret)
    {
      rv = true;
      val = valbuf;
    }
  else
    val = "";

  free(valbuf);
  return rv;
}

bool Genders::testattrval(const string attr, const string val, const string node) const
{
  bool rv = false;
  int ret;

  if ((ret = genders_testattrval(gh,
				 node.c_str(),
				 attr.c_str(),
				 val.c_str())) < 0)
    _throw_exception(genders_errnum(gh));
  
  if (ret)
    rv = true;

  return rv;
}

bool Genders::isnode(const string node) const
{
  bool rv = false;
  int ret;

  if ((ret = genders_isnode(gh, node.c_str())) < 0)
    _throw_exception(genders_errnum(gh));

  if (ret)
    rv = true;

  return rv;
}

bool Genders::isattr(const string attr) const
{
  bool rv = false;
  int ret;

  if ((ret = genders_isattr(gh, attr.c_str())) < 0)
    _throw_exception(genders_errnum(gh));

  if (ret)
    rv = true;

  return rv;
}

bool Genders::isattrval(const string attr, const string val) const
{
  bool rv = false;
  int ret;

  if ((ret = genders_isattrval(gh, attr.c_str(), val.c_str())) < 0)
    _throw_exception(genders_errnum(gh));

  if (ret)
    rv = true;

  return rv;
}

vector< string > Genders::query(const string query) const
{
  vector<string> rv;
  char **nodelist = NULL;
  int nodelist_len;
  int nodelist_count;

  if ((nodelist_len = genders_nodelist_create(gh, &nodelist)) < 0)
    _throw_exception(genders_errnum(gh));

  if ((nodelist_count = genders_query(gh,
				      nodelist,
				      nodelist_len,
				      query.c_str())) < 0)
    {
      int errnum = genders_errnum(gh);
      genders_nodelist_destroy(gh, nodelist);
      _throw_exception(errnum);
    }
  
  for (int i = 0; i < nodelist_count; i++)
    rv.push_back(nodelist[i]);
  
  genders_nodelist_destroy(gh, nodelist);
  return rv;
}

bool Genders::testquery(const std::string query, const std::string node)
{
  bool rv = false;
  int ret;

  if ((ret = genders_testquery(gh, node.c_str(), query.c_str())) < 0)
    _throw_exception(genders_errnum(gh));

  if (ret)
    rv = true;

  return rv;
}
