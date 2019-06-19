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

#ifndef _GENDERSPLUSPLUS_HPP
#define _GENDERSPLUSPLUS_HPP

#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include <genders.h>
}

namespace Gendersplusplus
{

/*
 * GendersException
 *
 * May occur in any Genders class function.
 */
class GendersException
{
public:
  GendersException();
  GendersException(int errnum);
  const char *errormsg() const;
  int errnum() const;
private:
  int _errnum;
};

std::ostream &operator<<(std::ostream &output, const GendersException &e);

class GendersExceptionOpen : public GendersException
{
public:
  GendersExceptionOpen();
};

class GendersExceptionRead : public GendersException
{
public:
  GendersExceptionRead();
};

class GendersExceptionParse : public GendersException
{
public:
  GendersExceptionParse();
};

class GendersExceptionParameters : public GendersException
{
public:
  GendersExceptionParameters();
};

class GendersExceptionNotfound : public GendersException
{
public:
  GendersExceptionNotfound();
};

class GendersExceptionSyntax : public GendersException
{
public:
  GendersExceptionSyntax();
};

class GendersExceptionInternal : public GendersException
{
public:
  GendersExceptionInternal();
};

/*
 * Genders
 *
 * C++ class for libgenders.  Operates nearly identically to classic
 * genders C library.  Changes include:
 *
 * - Errors are returned via exceptions
 * - Use of STL instead of genders specific data structures
 * - Functions may take empty strings instead of NULL pointers for
 *   defaults.
 *
 */
class Genders
{
public:
  Genders();
  Genders(const std::string filename);
  Genders(const Genders &copy);
  const Genders &operator=(const Genders &right);
  ~Genders();
  unsigned int getnumnodes() const;
  unsigned int getnumattrs() const;
  unsigned int getmaxattrs() const;
  std::string getnodename() const;
  std::vector< std::string > getnodes(const std::string attr = "", const std::string val = "") const;
  std::vector< std::pair< std::string, std::string > > getattr(const std::string node = "") const;
  std::vector< std::string > getattr_all() const;
  bool testattr(const std::string attr, std::string &val, const std::string node = "") const;
  bool testattrval(const std::string attr, const std::string val = "", const std::string node = "") const;
  bool isnode(const std::string node = "") const;
  bool isattr(const std::string attr) const;
  bool isattrval(const std::string attr, const std::string val) const;
  std::vector< std::string > query(const std::string query = "") const;
  bool testquery(const std::string query, const std::string node = "");
private:
  void _constructor(const std::string filename);
  void _throw_exception(int errnum) const;
  genders_t gh;
};

} // Gendersplusplus

#endif /* _GENDERSPLUSPLUS_HPP */
