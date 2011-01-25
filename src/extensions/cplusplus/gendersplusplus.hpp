
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

class Genders
{
public:
  Genders();
  Genders(const std::string filename);
  Genders(const Genders &copy);
  ~Genders();
  int getnumnodes() const;
  int getnumattrs() const;
  int getmaxattrs() const;
  std::string getnodename() const;
  std::vector< std::string > getnodes(const std::string attr = "", const std::string val = "") const;
  std::vector< std::pair< std::string, std::string > > getattr(const std::string node) const;
  std::vector< std::string > getattr_all() const;
  bool testattr(const std::string node, const std::string attr, std::string &val) const;
  bool testattrval(const std::string node, const std::string attr, const std::string val) const;
  bool isnode(const std::string node) const;
  bool isattr(const std::string attr) const;
  bool isattrval(const std::string attr, const std::string val) const;
  std::vector< std::string > query(const std::string query) const;
  bool testquery(const std::string node, const std::string query);
  void parse(const std::string filename);
private:
  genders_t gh;
};

} // Gendersplusplus

#endif /* _GENDERSPLUSPLUS_HPP */
