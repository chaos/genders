/*****************************************************************************\
 *  $Id: genders.c,v 1.146 2010-02-02 00:04:34 chu11 Exp $
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

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>

#include <genders.h>

#include <Python.h>
#include <structmember.h>

typedef struct {
  PyObject_HEAD
  genders_t gh;
  int genders_err_open;
  int genders_err_read;
  int genders_err_parse;
  int genders_err_notfound;
  int genders_err_syntax;
  int genders_err_internal;
} Libgenders;

/* support Python earlier than 2.6 */
#ifndef Py_TYPE
#define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
#endif

static void
Libgenders_dealloc(Libgenders* self)
{
  genders_handle_destroy(self->gh);
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Libgenders_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  Libgenders *self;

  self = (Libgenders *)type->tp_alloc(type, 0);
  if (self) {
    if (!(self->gh = genders_handle_create())) {
      Py_DECREF(self);
      return NULL;
    }

    self->genders_err_open = GENDERS_ERR_OPEN;
    self->genders_err_read = GENDERS_ERR_READ;
    self->genders_err_parse = GENDERS_ERR_PARSE;
    self->genders_err_notfound = GENDERS_ERR_NOTFOUND;
    self->genders_err_syntax = GENDERS_ERR_SYNTAX;
    self->genders_err_internal = GENDERS_ERR_INTERNAL;
  }

  return (PyObject *)self;
}

static PyMemberDef Libgenders_members[] = {
  {"genders_err_open", T_INT, offsetof(Libgenders, genders_err_open), 0,
   "GENDERS_ERR_OPEN number"},
  {"genders_err_read", T_INT, offsetof(Libgenders, genders_err_read), 0,
   "GENDERS_ERR_READ number"},
  {"genders_err_parse", T_INT, offsetof(Libgenders, genders_err_parse), 0,
   "GENDERS_ERR_PARSE number"},
  {"genders_err_notfound", T_INT, offsetof(Libgenders, genders_err_notfound), 0,
   "GENDERS_ERR_NOTFOUND number"},
  {"genders_err_syntax", T_INT, offsetof(Libgenders, genders_err_syntax), 0,
   "GENDERS_ERR_SYNTAX number"},
  {"genders_err_internal", T_INT, offsetof(Libgenders, genders_err_internal), 0,
   "GENDERS_ERR_INTERNAL number"},
  {NULL},  /* Sentinel */
};

static void
_genders_exception_check(Libgenders *self)
{
  /* achu: Most errors are passed up for people to deal with on their
   * own, but we'll set the out of memory exception to match malloc()
   * calls
   */
  if (genders_errnum(self->gh) == GENDERS_ERR_OUTMEM)
    PyErr_NoMemory();
}

static PyObject *
Libgenders_load_data(Libgenders *self, PyObject *args)
{
  const char *filename = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "|z", &filename))
    goto cleanup;

  if ((ret = genders_load_data(self->gh, filename)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }
  
  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;
  
 cleanup:
  return rv; 
}

static PyObject *
Libgenders_errnum(Libgenders *self)
{
  PyObject *rv = NULL;
  int errnum;

  errnum = genders_errnum(self->gh);

  if (!(rv = Py_BuildValue("i", errnum)))
    return NULL;

  return rv;
}

static PyObject *
Libgenders_strerror(Libgenders *self, PyObject *args)
{
  int errnum;
  char *str;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "i", &errnum))
    goto cleanup;

  str = genders_strerror(errnum);

  if (!(rv = Py_BuildValue("s", str)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_errormsg(Libgenders *self)
{
  char *str;
  PyObject *rv = NULL;

  str = genders_errormsg(self->gh);

  if (!(rv = Py_BuildValue("s", str)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_getnumnodes(Libgenders *self)
{
  int numnodes;
  PyObject *rv = NULL;

  if ((numnodes = genders_getnumnodes(self->gh)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", numnodes)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_getnumattrs(Libgenders *self)
{
  int numattrs;
  PyObject *rv = NULL;

  if ((numattrs = genders_getnumattrs(self->gh)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", numattrs)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
_build_comma_separated_string(int maxitemlen, char **itemlist, int itemlistlen)
{
  char *stringbuf = NULL;
  unsigned int stringbuflen;
  PyObject *rv = NULL;
  int i;

  /* achu: As far as I can tell, there is no (present) way to
   * (reasonbly and easily) return a Python list instead of a single
   * object.  If there is a way, I can't find it and/or it's advanced Python stuff.
   *
   * Until I learn of a way, I'll return a giant comma separated
   * string.  It's the job of an upper level library to deal with the
   * string and convert it to something more useful like a list.
   */

  /* +1 to deal with commas */
  stringbuflen = itemlistlen * (maxitemlen + 1);

  if (!(stringbuf = (char *)malloc(stringbuflen))) {
    PyErr_NoMemory();
    goto cleanup;
  }

  memset(stringbuf, '\0', stringbuflen);

  for (i = 0; i < itemlistlen; i++) {
    strcat(stringbuf, itemlist[i]);
    if (i != (itemlistlen - 1))
      strcat(stringbuf, ",");
  }

  if (!(rv = Py_BuildValue("s", stringbuf)))
    goto cleanup;
  
 cleanup:
  free(stringbuf);
  return rv;
}

static PyObject *
Libgenders_getnodes(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  const char *val = NULL;
  char **nodelist = NULL;
  int nodelistlen;
  int nodelen;
  PyObject *rv = NULL;
  int errnum;

  if (!PyArg_ParseTuple(args, "|zz", &attr, &val))
    goto cleanup;

  if ((nodelistlen = genders_nodelist_create(self->gh, &nodelist)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if ((nodelen = genders_getnodes(self->gh, nodelist, nodelistlen, attr, val)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (nodelen) {
    int maxnodelen;

    if ((maxnodelen = genders_getmaxnodelen(self->gh)) < 0) {
      _genders_exception_check(self);
      goto cleanup;
    }

    if (!(rv = _build_comma_separated_string(maxnodelen,
					     nodelist,
					     nodelen)))
      goto cleanup;
  }
  else {
    if (!(rv = Py_BuildValue("z", NULL)))
      goto cleanup;
  }
  
 cleanup:
  errnum = genders_errnum(self->gh);
  genders_nodelist_destroy(self->gh, nodelist);
  genders_set_errnum(self->gh, errnum);
  return rv; 
}

static PyObject *
Libgenders_getattr(Libgenders *self, PyObject *args)
{
  const char *node = NULL;
  char **attrlist = NULL;
  int attrlistlen;
  int attrlen;
  PyObject *rv = NULL;
  int errnum;

  if (!PyArg_ParseTuple(args, "|z", &node))
    goto cleanup;

  if ((attrlistlen = genders_attrlist_create(self->gh, &attrlist)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if ((attrlen = genders_getattr(self->gh,
				 attrlist,
				 NULL,
				 attrlistlen,
				 node)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (attrlen) {
    int maxattrlen;

    if ((maxattrlen = genders_getmaxattrlen(self->gh)) < 0) {
      _genders_exception_check(self);
      goto cleanup;
    }

    if (!(rv = _build_comma_separated_string(maxattrlen,
					     attrlist,
					     attrlen)))
      goto cleanup;
  }
  else {
    if (!(rv = Py_BuildValue("z", NULL)))
      goto cleanup;
  }
  
 cleanup:
  errnum = genders_errnum(self->gh);
  genders_attrlist_destroy(self->gh, attrlist);
  genders_set_errnum(self->gh, errnum);
  return rv; 
}

static PyObject *
Libgenders_getattr_all(Libgenders *self)
{
  char **attrlist = NULL;
  int attrlistlen;
  int attrlen;
  PyObject *rv = NULL;
  int errnum;

  if ((attrlistlen = genders_attrlist_create(self->gh, &attrlist)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if ((attrlen = genders_getattr_all(self->gh,
				     attrlist,
				     attrlistlen)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (attrlen) {
    int maxattrlen;

    if ((maxattrlen = genders_getmaxattrlen(self->gh)) < 0) {
      _genders_exception_check(self);
      goto cleanup;
    }

    if (!(rv = _build_comma_separated_string(maxattrlen,
					     attrlist,
					     attrlen)))
      goto cleanup;
  }
  else {
    if (!(rv = Py_BuildValue("z", NULL)))
      goto cleanup;
  }
  
 cleanup:
  errnum = genders_errnum(self->gh);
  genders_attrlist_destroy(self->gh, attrlist);
  genders_set_errnum(self->gh, errnum);
  return rv; 
}

static PyObject *
Libgenders_getnodename(Libgenders* self)
{
  char *nodenamebuf = NULL;
  int maxnodelen;
  PyObject *rv = NULL;

  if ((maxnodelen = genders_getmaxnodelen(self->gh)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(nodenamebuf = (char *)malloc(maxnodelen + 1))) {
    PyErr_NoMemory();
    goto cleanup;
  }

  memset(nodenamebuf, '\0', maxnodelen + 1);

  if (genders_getnodename(self->gh, nodenamebuf, maxnodelen + 1) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("s", nodenamebuf)))
    goto cleanup;

 cleanup:
  free(nodenamebuf);
  return rv;
}

static PyObject *
Libgenders_getattrval(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  const char *node = NULL;
  int maxvallen;
  char *valbuf = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "s|z", &attr, &node))
    goto cleanup;

  if ((maxvallen = genders_getmaxvallen(self->gh)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(valbuf = (char *)malloc(maxvallen + 1))) {
    PyErr_NoMemory();
    goto cleanup;
  }
 
  memset(valbuf, '\0', maxvallen + 1);

  if ((ret = genders_testattr(self->gh, node, attr, valbuf, maxvallen + 1)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (ret && strlen(valbuf)) {
    if (!(rv = Py_BuildValue("z", valbuf)))
      goto cleanup;
  }
  else {
    if (!(rv = Py_BuildValue("z", NULL)))
      goto cleanup;
  }
  
 cleanup:
  free(valbuf);
  return rv; 
}

static PyObject *
Libgenders_testattr(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  const char *node = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "s|z", &attr, &node))
    goto cleanup;

  if ((ret = genders_testattr(self->gh, node, attr, NULL, 0)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_testattrval(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  const char *val = NULL;
  const char *node = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "ss|z", &attr, &val, &node))
    goto cleanup;

  if ((ret = genders_testattrval(self->gh, node, attr, val)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;
  
 cleanup:
  return rv;
}

static PyObject *
Libgenders_isnode(Libgenders *self, PyObject *args)
{
  const char *node = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "|z", &node))
    goto cleanup;

  if ((ret = genders_isnode(self->gh, node)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_isattr(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "s", &attr))
    goto cleanup;

  if ((ret = genders_isattr(self->gh, attr)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_isattrval(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  const char *val = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "ss", &attr, &val))
    goto cleanup;

  if ((ret = genders_isattrval(self->gh, attr, val)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_index_attrvals(Libgenders *self, PyObject *args)
{
  const char *attr = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "s", &attr))
    goto cleanup;

  if ((ret = genders_index_attrvals(self->gh, attr)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyObject *
Libgenders_query(Libgenders *self, PyObject *args)
{
  const char *query = NULL;
  char **nodelist = NULL;
  int nodelistlen;
  int nodelen;
  PyObject *rv = NULL;
  int errnum;

  if (!PyArg_ParseTuple(args, "|z", &query))
    goto cleanup;

  if ((nodelistlen = genders_nodelist_create(self->gh, &nodelist)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if ((nodelen = genders_query(self->gh, nodelist, nodelistlen, query)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (nodelen) {
    int maxnodelen;
    
    if ((maxnodelen = genders_getmaxnodelen(self->gh)) < 0) {
      _genders_exception_check(self);
      goto cleanup;
    }

    if (!(rv = _build_comma_separated_string(maxnodelen,
					     nodelist,
					     nodelen)))
      goto cleanup;
  }
  else {
    if (!(rv = Py_BuildValue("z", NULL)))
      goto cleanup;
  }
  
 cleanup:
  errnum = genders_errnum(self->gh);
  genders_nodelist_destroy(self->gh, nodelist);
  genders_set_errnum(self->gh, errnum);
  return rv; 
}

static PyObject *
Libgenders_testquery(Libgenders *self, PyObject *args)
{
  const char *query = NULL;
  const char *node = NULL;
  int ret;
  PyObject *rv = NULL;

  if (!PyArg_ParseTuple(args, "s|z", &query, &node))
    goto cleanup;

  if ((ret = genders_testquery(self->gh, node, query)) < 0) {
    _genders_exception_check(self);
    goto cleanup;
  }

  if (!(rv = Py_BuildValue("i", ret)))
    goto cleanup;

 cleanup:
  return rv;
}

static PyMethodDef Libgenders_methods[] = {
  {
    "load_data",
    (PyCFunction)Libgenders_load_data,
    METH_VARARGS,
    "Opens, reads, and parses the genders file specified.  If a file is not specified, the default genders file is parsed."
  },
  {
    "errnum",
    (PyCFunction)Libgenders_errnum,
    METH_NOARGS,
    "Returns the error code most recently set."
  },
  {
    "strerror",
    (PyCFunction)Libgenders_strerror,
    METH_VARARGS,
    "Returns a string describing an errnum."
  },
  {
    "errormsg",
    (PyCFunction)Libgenders_errormsg,
    METH_NOARGS,
    "Returns a string describing the most recent error."
  },
  {
    "getnumnodes",
    (PyCFunction)Libgenders_getnumnodes,
    METH_NOARGS,
    "Returns the number of nodes listed in the genders file."
  },
  {
    "getnumattrs",
    (PyCFunction)Libgenders_getnumattrs,
    METH_NOARGS,
    "Returns the number of attributes listed in the genders file."
  },
  {
    "getnodename",
    (PyCFunction)Libgenders_getnodename,
    METH_NOARGS,
    "Get the name of the current node.  Node name returned is the shortened hostname."
  },
  {
    "getnodes",
    (PyCFunction)Libgenders_getnodes,
    METH_VARARGS,
    "Returns a comma separated string of nodes with the specified attribute and value.  If a value is not specified, only the attribute is considered.  If the attribute is not specified, all nodes listed in the genders file are returned."
  },
  {
    "getattr",
    (PyCFunction)Libgenders_getattr,
    METH_VARARGS,
    "Returns a comma separated string of attributes for the specified node. If the node is not specified, the local node's attributes returned."
  },
  {
    "getattr_all",
    (PyCFunction)Libgenders_getattr_all,
    METH_NOARGS,
    "Returns a comma separated string of all attributes in the genders file."
  },
  {
    "getattrval",
    (PyCFunction)Libgenders_getattrval,
    METH_VARARGS,
    "Returns the value of the specified attribute for the specified node.  If the attribute does not exist or the attribute has no value, None is returned.  If the node is not specified, the local node's attribute value is returned."
  },
  {
    "testattr",
    (PyCFunction)Libgenders_testattr,
    METH_VARARGS,
    "Returns 1 if the specified node has the specified attribute, 0 if it does not.  If the node is not specified, the local node is checked."
  },
  {
    "testattrval",
    (PyCFunction)Libgenders_testattrval,
    METH_VARARGS,
    "Returns 1 if the specified node has the specified attribute and value, 0 if it does not.  If the node is not specified, the local node is checked."
  },
  {
    "isnode",
    (PyCFunction)Libgenders_isnode,
    METH_VARARGS,
    "Returns 1 if the specified node is listed in the genders file, 0 if it is not.  If the node is not specified, the local node is checked."
  },
  {
    "isattr",
    (PyCFunction)Libgenders_isattr,
    METH_VARARGS,
    "Returns 1 if the specified attribute is listed in the genders file, 0 if it is not."
  },
  {"isattrval",
   (PyCFunction)Libgenders_isattrval,
   METH_VARARGS,
   "Returns 1 if the specified attribute is equal to the specified value for some node in the genders file, 0 if it is not."
  },
  {
    "index_attrvals",
    (PyCFunction)Libgenders_index_attrvals,
    METH_VARARGS,
    "Internally indexes genders attribute values for faster search times.  Subsequent calls with a different attribute will overwrite earlier indexes."
  },
  {
    "query",
    (PyCFunction)Libgenders_query,
    METH_VARARGS,
    "Returns a comma separated string of nodes specified by a genders query.  A genders query is based on the union, intersection, set difference, or complement between genders attributes and values.  Union is represented by two pipe symbols ('||'), intersection by two ampersand symbols ('&&'), difference by two minus symbols ('--'), and complement by a tilde ('~') Operations are performed from left to right.  Parentheses may be used to change the order of operations.  For example, the following query would retrieve all nodes other than management or login nodes:\"all-(mgmt+login)\".  If the query is not specified, all nodes listed in the genders file are returned."
  },
  {
    "testquery",
    (PyCFunction)Libgenders_testquery,
    METH_VARARGS,
    "Returns 1 if the specified node meets the conditions of the specified query, 0 if it does not.  If the node is not specified, the local node is checked."
  },
  {NULL},  /* Sentinel */
};

/* support Python 2.5 or earlier while still avoiding "missing braces around initializer" warning */
#ifndef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size) \
    PyObject_HEAD_INIT(type) size,
#endif

static PyTypeObject LibgendersType = {
  PyVarObject_HEAD_INIT(NULL, 0)  /*type, ob_size*/
  "libgenders.Libgenders",	/*tp_name*/
  sizeof(Libgenders),	        /*tp_basicsize*/
  0,				/*tp_itemsize*/
  (destructor)Libgenders_dealloc, /*tp_dealloc*/
  0,                            /*tp_print*/
  0,                            /*tp_getattr*/
  0,                            /*tp_setattr*/
  0,                            /*tp_compare*/
  0,                            /*tp_repr*/
  0,                            /*tp_as_number*/
  0,                            /*tp_as_sequence*/
  0,                            /*tp_as_mapping*/
  0,                            /*tp_hash */
  0,                            /*tp_call*/
  0,                            /*tp_str*/
  0,                            /*tp_getattro*/
  0,                            /*tp_setattro*/
  0,                            /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
  "Libgenders Database Object",	/*tp_doc */
  0,				/* tp_traverse */
  0,				/* tp_clear */
  0,				/* tp_richcompare */
  0,				/* tp_weaklistoffset */
  0,				/* tp_iter */
  0,				/* tp_iternext */
  Libgenders_methods,		/* tp_methods */
  Libgenders_members,		/* tp_members */
  0,				/* tp_getset */
  0,				/* tp_base */
  0,				/* tp_dict */
  0,				/* tp_descr_get */
  0,				/* tp_descr_set */
  0,				/* tp_dictoffset */
  0, 				/* tp_init */
  0,				/* tp_alloc */
  Libgenders_new,			/* tp_new */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT,
  "libgenders",          /* m_name */
  "Libgenders module for genders database querying.",  /* m_doc */
  -1,                    /* m_size */
  Libgenders_methods,    /* m_methods */
  NULL,                  /* m_reload */
  NULL,                  /* m_traverse */
  NULL,                  /* m_clear */
  NULL,                  /* m_free */
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_libgenders(void)

#else  /* Python 2 */
#define INITERROR return

#ifndef PyMODINIT_FUNC /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC
initlibgenders(void) 
#endif
{
  PyObject* m;

  if (PyType_Ready(&LibgendersType) < 0)
    INITERROR;

#if PY_MAJOR_VERSION >= 3
  m = PyModule_Create(&moduledef);
#else  /* Python 2 */
  m = Py_InitModule3("libgenders", Libgenders_methods,
		     "Libgenders module for genders database querying.");
#endif

  if (m == NULL)
    INITERROR;

  Py_INCREF(&LibgendersType);
  PyModule_AddObject(m, "Libgenders", (PyObject *)&LibgendersType);

#if PY_MAJOR_VERSION >= 3
  return m;
#endif
}
