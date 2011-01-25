#include <gendersplusplus.hpp>

using namespace std;

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
  const char *c_str_attr = NULL;
  const char *c_str_val = NULL;

  if ((nodelist_len = genders_nodelist_create(gh, &nodelist)) < 0)
    {
      // XXX
    }

  if (!attr.empty())
    c_str_attr = attr.c_str();

  if (!val.empty())
    c_str_val = val.c_str();

  if ((nodelist_count = genders_getnodes(gh,
					 nodelist,
					 nodelist_len,
					 c_str_attr,
					 c_str_val)) < 0)
    {
      // XXX
    }
  
  for (int i = 0; i < nodelist_count; i++)
    rv.push_back(nodelist[i]);
  
 cleanup:
  genders_nodelist_destroy(gh, nodelist);
  return rv;
}

