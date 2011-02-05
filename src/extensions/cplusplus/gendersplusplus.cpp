#include <gendersplusplus.hpp>

using namespace std;
using namespace Gendersplusplus;

static genders_t genders_constructor(const string filename)
{
  genders_t gh;
  const char *c_str_filename = NULL;

  if (!(gh = genders_handle_create()))
    {
      // XXX
    }
  
  if (!filename.empty())
    c_str_filename = filename.c_str();
  
  if (genders_load_data(gh, c_str_filename) < 0)
    {
      // XXX
    }

  return gh;
}

Genders::Genders()
{
  gh = genders_constructor("");
}

Genders::Genders(const string filename)
{
  gh = genders_constructor(filename);
}

Genders::~Genders()
{
  (void)genders_handle_destroy(gh);
}

unsigned int Genders::getnumnodes() const
{
  int rv;

  if ((rv = genders_getnumnodes(gh)) < 0)
    {
      // XXX
    }

  return rv;
}

unsigned int Genders::getnumattrs() const
{
  int rv;

  if ((rv = genders_getnumattrs(gh)) < 0)
    {
      // XXX
    }

  return rv;
}

unsigned int Genders::getmaxattrs() const
{
  int rv;

  if ((rv = genders_getmaxattrs(gh)) < 0)
    {
      // XXX
    }

  return rv;
}

string Genders::getnodename() const
{
  string rv;
  char *buf = NULL;
  int maxnodelen;

  if ((maxnodelen = genders_getmaxnodelen(gh)) < 0)
    {
      // XXX
    }

  if (!(buf = (char *)malloc(maxnodelen + 1)))
    {
      // XXX
    }

  if (genders_getnodename(gh, buf, maxnodelen + 1) < 0)
    {
      // XXX
    }

  rv = buf;
  
 cleanup:
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
    {
      // XXX
    }

  if ((nodelist_count = genders_getnodes(gh,
					 nodelist,
					 nodelist_len,
					 attr.c_str(),
					 val.c_str())) < 0)
    {
      // XXX
    }
  
  for (int i = 0; i < nodelist_count; i++)
    rv.push_back(nodelist[i]);
  
 cleanup:
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
    {
      // XXX
    }

  if ((vallist_len = genders_vallist_create(gh, &vallist)) < 0)
    {
      // XXX
    }

  if ((list_count = genders_getattr(gh,
				    attrlist,
				    vallist,
				    attrlist_len,
				    node.c_str())) < 0)
    {
      // XXX
    }

  for (int i = 0; i < list_count; i++)
    rv.push_back(pair<string, string>(attrlist[i], vallist[i]));

 cleanup:
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
    {
      // XXX
    }

  if ((attrlist_count = genders_getattr_all(gh,
					    attrlist,
					    attrlist_len)) < 0)
    {
      // XXX
    }
  
  for (int i = 0; i < attrlist_count; i++)
    rv.push_back(attrlist[i]);

 cleanup:
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
    {
      // XXX
    }
  
  if (!(valbuf = (char *)malloc(maxvallen + 1)))
    {
      // XXX
    }

  memset(valbuf, '\0', maxvallen + 1);

  if ((ret = genders_testattr(gh,
			      node.c_str(),
			      attr.c_str(),
			      valbuf,
			      maxvallen + 1)) < 0)
    {
      // XXX
    }

  if (ret)
    {
      rv = true;
      val = valbuf;
    }

 cleanup:
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
    {
      // XXX
    }
  
  if (ret)
    rv = true;

 cleanup:
  return rv;
}
