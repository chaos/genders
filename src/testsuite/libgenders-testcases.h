/*
 * $Id: libgenders-testcases.h,v 1.1 2003-03-13 00:22:29 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/testsuite/libgenders-testcases.h,v $
 */

/* Definitions */

#define GENDERS_FILE_GOOD          "genders_file.good"
#define GENDERS_FILE_BAD            "genders_file.bad"

#define GENDERS_FILE_GOOD_NUMNODES                  10
#define GENDERS_FILE_GOOD_NUMATTRS                  13
#define GENDERS_FILE_GOOD_MAXATTRS                   4
#define GENDERS_FILE_GOOD_MAXNODELEN                 5
#define GENDERS_FILE_GOOD_MAXATTRLEN                 8
#define GENDERS_FILE_GOOD_MAXVALLEN                  6 

#define IS_NULL                                     -9   
#define IS_NOT_NULL                                 -8   
#define IS_NOT_NULL_OPEN                            -7
#define IS_NOT_NULL_NOT_OPEN                        -6
#define NO_VALUE                                    -5

#define IS_NEGATIVE                                 -1
#define IS_ZERO                                      0
#define IS_POSITIVE_SMALL                            1
#define IS_POSITIVE_LARGE                        99999

#define NO_ERRNUM                                    0

#define GENDERS_HANDLE_DESTROY                       0
#define GENDERS_OPEN                                 1
#define GENDERS_CLOSE                                2 
#define GENDERS_ERRNUM                               3 
#define GENDERS_STRERROR                             4
#define GENDERS_ERRORMSG                             5
#define GENDERS_PERROR                               6
#define GENDERS_GETNUMNODES                          7
#define GENDERS_GETNUMATTRS                          8
#define GENDERS_GETMAXATTRS                          9
#define GENDERS_GETMAXNODELEN                       10
#define GENDERS_GETMAXATTRLEN                       11
#define GENDERS_GETMAXVALLEN                        12
#define GENDERS_NODELIST_CREATE                     13
#define GENDERS_NODELIST_CLEAR                      14
#define GENDERS_NODELIST_DESTROY                    15
#define GENDERS_ATTRLIST_CREATE                     16
#define GENDERS_ATTRLIST_CLEAR                      17
#define GENDERS_ATTRLIST_DESTROY                    18
#define GENDERS_VALLIST_CREATE                      19
#define GENDERS_VALLIST_CLEAR                       20
#define GENDERS_VALLIST_DESTROY                     21
#define GENDERS_GETNODENAME                         22
#define GENDERS_GETNODES                            23
#define GENDERS_GETATTR                             24
#define GENDERS_GETATTR_ALL                         25
#define GENDERS_TESTATTR                            26
#define GENDERS_TESTATTRVAL                         27
#define GENDERS_TESTNODE                            28
#define GENDERS_ELAPSEDTIME                         29

char *function_names[] = {
  "genders_handle_destroy()",
  "genders_open()",
  "genders_close()",
  "genders_errnum",
  "genders_strerror",
  "genders_errormsg",
  "genders_perror",
  "genders_getnumnodes",
  "genders_getnumattrs",
  "genders_getmaxattrs",
  "genders_getmaxnodelen",
  "genders_getmaxattrlen",
  "genders_getmaxvallen",
  "genders_nodelist_create",
  "genders_nodelist_clear",
  "genders_nodelist_destroy",
  "genders_attrlist_create",
  "genders_attrlist_clear",
  "genders_attrlist_destroy",
  "genders_vallist_create",
  "genders_vallist_clear",
  "genders_vallist_destroy",
  "genders_getnodename",
  "genders_getnodes",
  "genders_getattr",
  "genders_getattr_all",
  "genders_testattr",
  "genders_testattrval",
  "genders_testnode",
  "genders_elapsedtime",
};

/* Parameter test cases for all functions.  Currently, "successful" test cases
 * are commented out.  Those will be handled in functionality testing.
 *
 * param1, param2, param3, do not necessarily correlate exactly to the
 * ordering of parameters of the function.  They are values used for
 * certain parameters within of the function
 */
struct {
  int function;
  int param1;
  int param2;
  int param3;
  int return_value;
  int return_errnum;
} parameter_tests[] = {
  {GENDERS_HANDLE_DESTROY,   IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_HANDLE_DESTROY,   IS_NOT_NULL_OPEN,     NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTCLOSED },
 
  {GENDERS_OPEN,             IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_OPEN,             IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },

  {GENDERS_CLOSE,            IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_CLOSE,            IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_ERRNUM,           IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ERRNUM,           IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_STRERROR,         IS_NEGATIVE,          NO_VALUE,    NO_VALUE,          IS_NULL,  NO_ERRNUM             },        
  {GENDERS_STRERROR,         IS_POSITIVE_LARGE,    NO_VALUE,    NO_VALUE,          IS_NULL,  NO_ERRNUM             },

  {GENDERS_ERRORMSG,         IS_NULL,              NO_VALUE,    NO_VALUE,          IS_NULL,  NO_ERRNUM             },
  {GENDERS_ERRORMSG,         IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          IS_NULL,  NO_ERRNUM             },

  {GENDERS_GETNUMNODES,      IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_GETNUMNODES,      IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },,

  {GENDERS_GETNUMATTRS,      IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_GETNUMATTRS,      IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_GETMAXATTRS,      IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_GETMAXATTRS,      IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_GETMAXNODELEN,    IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_GETMAXNODELEN,    IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_GETMAXATTRLEN,    IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_GETMAXATTRLEN,    IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_GETMAXVALLEN,     IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_GETMAXVALLEN,     IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_NODELIST_CREATE,  IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_NODELIST_CREATE,  IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_OPEN,     IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},

  {GENDERS_NODELIST_CLEAR,   IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_NODELIST_CLEAR,   IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_NODELIST_DESTROY, IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_NODELIST_DESTROY, IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_ATTRLIST_CREATE,  IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ATTRLIST_CREATE,  IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_ATTRLIST_CLEAR,   IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ATTRLIST_CLEAR,   IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_ATTRLIST_DESTROY, IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ATTRLIST_DESTROY, IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_VALLIST_CREATE,   IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_VALLIST_CREATE,   IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_VALLIST_CLEAR,    IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_VALLIST_CLEAR,    IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_VALLIST_DESTROY,  IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_VALLIST_DESTROY,  IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_GETNODENAME,      IS_NULL,              IS_NULL,     IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETNODENAME,      IS_NULL,              IS_NOT_NULL, IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETNODENAME,      IS_NULL,              IS_NULL,     IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETNODENAME,      IS_NULL,              IS_NOT_NULL, IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETNODENAME,      IS_NULL,              IS_NULL,     IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETNODENAME,      IS_NULL,              IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_NOTOPEN   }, 
  {GENDERS_GETNODENAME,      IS_NOT_NULL_OPEN,     IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_OPEN,     IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_OPEN,     IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_OVERFLOW  },

  {GENDERS_GETNODES,         IS_NULL,              IS_NULL,     IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETNODES,         IS_NULL,              IS_NOT_NULL, IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETNODES,         IS_NULL,              IS_NULL,     IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETNODES,         IS_NULL,              IS_NOT_NULL, IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETNODES,         IS_NULL,              IS_NULL,     IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETNODES,         IS_NULL,              IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_GETNODES,         IS_NOT_NULL_OPEN,     IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_OPEN,     IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_OPEN,     IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_OVERFLOW  },

  {GENDERS_GETATTR,          IS_NULL,              IS_NULL,     IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETATTR,          IS_NULL,              IS_NOT_NULL, IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETATTR,          IS_NULL,              IS_NULL,     IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETATTR,          IS_NULL,              IS_NOT_NULL, IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETATTR,          IS_NULL,              IS_NULL,     IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETATTR,          IS_NULL,              IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_GETATTR,          IS_NOT_NULL_OPEN,     IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_OPEN,     IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_OPEN,     IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_OVERFLOW  },

  {GENDERS_GETATTR_ALL,      IS_NULL,              IS_NULL,     IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETATTR_ALL,      IS_NULL,              IS_NOT_NULL, IS_NEGATIVE,       -1,       NO_ERRNUM             },
  {GENDERS_GETATTR_ALL,      IS_NULL,              IS_NULL,     IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETATTR_ALL,      IS_NULL,              IS_NOT_NULL, IS_ZERO,           -1,       NO_ERRNUM             },
  {GENDERS_GETATTR_ALL,      IS_NULL,              IS_NULL,     IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETATTR_ALL,      IS_NULL,              IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       NO_ERRNUM             },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_NOTOPEN   },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_OPEN,     IS_NULL,     IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_NEGATIVE,       -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_OPEN,     IS_NULL,     IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_ZERO,           -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_OPEN,     IS_NULL,     IS_POSITIVE_SMALL, -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, IS_POSITIVE_SMALL, -1,       GENDERS_ERR_OVERFLOW  },

  {GENDERS_TESTATTR,         IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_TESTATTR,         IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_OPEN,     IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_TESTATTRVAL,      IS_NULL,              IS_NULL,     NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_TESTATTRVAL,      IS_NULL,              IS_NOT_NULL, NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_OPEN, IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_OPEN, IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_OPEN,     IS_NULL,     NO_VALUE,          -1,       GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_OPEN,     IS_NOT_NULL, NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },
 
  {GENDERS_TESTNODE,         IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             },
  {GENDERS_TESTNODE,         IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {GENDERS_ELAPSEDTIME,      IS_NULL,              NO_VALUE,    NO_VALUE,          -1,       NO_ERRNUM             }
  {GENDERS_ELAPSEDTIME,      IS_NOT_NULL_NOT_OPEN, NO_VALUE,    NO_VALUE,          -1,       GENDERS_ERR_NOTOPEN   },

  {-1, -1, -1, -1, -1, -1},
};
