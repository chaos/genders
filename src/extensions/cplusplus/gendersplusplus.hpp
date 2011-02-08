
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
  Genders();			// done
  Genders(const std::string filename); // done
  Genders(const Genders &copy);
  ~Genders();			// done
  unsigned int getnumnodes() const; // done
  unsigned int getnumattrs() const; // done
  unsigned int getmaxattrs() const; // done
  std::string getnodename() const; // done
  std::vector< std::string > getnodes(const std::string attr = "", const std::string val = "") const; // done
  std::vector< std::pair< std::string, std::string > > getattr(const std::string node = "") const; // done
  std::vector< std::string > getattr_all() const; // done
  bool testattr(const std::string attr, std::string &val, const std::string node = "") const; // done
  bool testattrval(const std::string attr, const std::string val = "", const std::string node = "") const; // done
  bool isnode(const std::string node = "") const; // done
  bool isattr(const std::string attr) const; // done
  bool isattrval(const std::string attr, const std::string val) const; // done
  std::vector< std::string > query(const std::string query = "") const;
  bool testquery(const std::string query, const std::string node = "");
  void parse(const std::string filename);
private:
  genders_t gh;
};

} // Gendersplusplus

#endif /* _GENDERSPLUSPLUS_HPP */
