
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

class GendersExceptionOutmem : public GendersException
{
public:
  GendersExceptionOutmem();
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
  unsigned int parse(const std::string filename = "");
private:
  void _constructor(const std::string filename);
  void _throw_exception(int errnum) const;
  genders_t gh;
};

} // Gendersplusplus

#endif /* _GENDERSPLUSPLUS_HPP */
