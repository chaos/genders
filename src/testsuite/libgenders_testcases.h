/*
 * $Id: libgenders_testcases.h,v 1.4 2003-05-21 00:43:55 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/testsuite/libgenders_testcases.h,v $
 */

/* Definitions */

#define GENDERS_GOOD_NODENAME                  "test0"
#define GENDERS_BAD_NODENAME                  "foobar"

#define GENDERS_FILE_GOOD          "genders_file.good"
#define GENDERS_FILE_BAD            "genders_file.bad"

#define GENDERS_GOOD_FILENAME      "genders_file.good"
#define GENDERS_BAD_FILENAME                  "foobar"

#define GENDERS_FILE_GOOD_NUMNODES                  10
#define GENDERS_FILE_GOOD_NUMATTRS                  13
#define GENDERS_FILE_GOOD_MAXATTRS                   4
#define GENDERS_FILE_GOOD_MAXNODELEN                 5
#define GENDERS_FILE_GOOD_MAXATTRLEN                 8
#define GENDERS_FILE_GOOD_MAXVALLEN                  6 

#define IS_NULL                                     -9   
#define IS_NOT_NULL                                 -8   
#define IS_NOT_NULL_GOOD                            -7   
#define IS_NOT_NULL_BAD                             -6   
#define IS_NOT_NULL_LOADED                          -5
#define IS_NOT_NULL_NOT_LOADED                      -4
#define IS_NOT_NULL_DESTROYED                       -3
#define NO_VALUE                                    -2

#define IS_NEGATIVE                                 -1
#define IS_ZERO                                      0
#define IS_POSITIVE_SMALL                            1
#define IS_POSITIVE_LARGE                        99999

#define GOOD_NODENAME                                0
#define BAD_NODENAME                                 1

/* Functions.  Function numbers are all unique, and include bits so
 * that similarities between common functions can be exploited by the
 * test suite for more efficient programming.
 */ 
#define GENDERS_HANDLE_DESTROY                      0x1000 
#define GENDERS_LOAD_DATA                           0x1001 
#define GENDERS_ERRNUM                              0x1002  
#define GENDERS_STRERROR                            0x2003 
#define GENDERS_ERRORMSG                            0x2004  
#define GENDERS_PERROR                              0x1005  
#define GENDERS_GETNUMNODES                         0x1006  
#define GENDERS_GETNUMATTRS                         0x1007  
#define GENDERS_GETMAXATTRS                         0x1008  
#define GENDERS_GETMAXNODELEN                       0x1009 
#define GENDERS_GETMAXATTRLEN                       0x100a 
#define GENDERS_GETMAXVALLEN                        0x100b 
#define GENDERS_NODELIST_CREATE                     0x110c 
#define GENDERS_NODELIST_CLEAR                      0x120d 
#define GENDERS_NODELIST_DESTROY                    0x140e  
#define GENDERS_ATTRLIST_CREATE                     0x110f 
#define GENDERS_ATTRLIST_CLEAR                      0x1210 
#define GENDERS_ATTRLIST_DESTROY                    0x1411 
#define GENDERS_VALLIST_CREATE                      0x1112 
#define GENDERS_VALLIST_CLEAR                       0x1213 
#define GENDERS_VALLIST_DESTROY                     0x1414 
#define GENDERS_GETNODENAME                         0x1015 
#define GENDERS_GETNODES                            0x1016 
#define GENDERS_GETATTR                             0x1017 
#define GENDERS_GETATTR_ALL                         0x1018 
#define GENDERS_TESTATTR                            0x1019 
#define GENDERS_TESTATTRVAL                         0x101a 
#define GENDERS_ISNODE                              0x101b 
#define GENDERS_ISATTR                              0x101c 
#define GENDERS_ISATTRVAL                           0x101d 
#define GENDERS_PARSE                               0x101e

#define GENDERS_FUNCTION_MASK                       0x00ff

#define GENDERS_CREATE                              0x0100
#define GENDERS_CLEAR                               0x0200
#define GENDERS_DESTROY                             0x0400

#define GENDERS_INT_RESULT                          0x1000
#define GENDERS_CHAR_RESULT                         0x2000

char *function_names[] = {
  "genders_handle_destroy",
  "genders_load_data",
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
  "genders_isnode",
  "genders_isattr",
  "genders_isattrval",
  "genders_parse",
};

/* Parameter test cases for all functions.  Currently, "successful" test cases
 * are commented out.  Those will be handled in functionality testing.
 *
 * params do not necessarily correlate exactly to the ordering of
 * parameters of the function.  They are values used for certain
 * parameters within the function
 */
struct {
  int function;
  int param1;
  int param2;
  int param3;
  int param4;
  int return_value;
  int return_errnum;
} parameter_tests[] =  {
  {GENDERS_HANDLE_DESTROY,   IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_HANDLE_DESTROY,   IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
 
  {GENDERS_LOAD_DATA,        IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_LOAD_DATA,        IS_NULL,                IS_NOT_NULL_GOOD, NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_LOAD_DATA,        IS_NULL,                IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_LOAD_DATA,        IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_ISLOADED  },
  {GENDERS_LOAD_DATA,        IS_NOT_NULL_LOADED,     IS_NOT_NULL_GOOD, NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_ISLOADED  },
  {GENDERS_LOAD_DATA,        IS_NOT_NULL_LOADED,     IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_ISLOADED  },
  {GENDERS_LOAD_DATA,        IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_LOAD_DATA,        IS_NOT_NULL_DESTROYED,  IS_NOT_NULL_GOOD, NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_LOAD_DATA,        IS_NOT_NULL_DESTROYED,  IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_ERRNUM,           IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           GENDERS_ERR_NULLHANDLE, GENDERS_ERR_NULLHANDLE},
  {GENDERS_ERRNUM,           IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           GENDERS_ERR_MAGIC,      GENDERS_ERR_MAGIC},

  {GENDERS_GETNUMNODES,      IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNUMNODES,      IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNUMNODES,      IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETNUMATTRS,      IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNUMATTRS,      IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNUMATTRS,      IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETMAXATTRS,      IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETMAXATTRS,      IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNUMATTRS,      IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETMAXNODELEN,    IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETMAXNODELEN,    IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETMAXNODELEN,    IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETMAXATTRLEN,    IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETMAXATTRLEN,    IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETMAXATTRLEN,    IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETMAXVALLEN,     IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETMAXVALLEN,     IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETMAXVALLEN,     IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_NODELIST_CREATE,  IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_NODELIST_CREATE,  IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_NODELIST_CREATE,  IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_NODELIST_CLEAR,   IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_NODELIST_CLEAR,   IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_NODELIST_CLEAR,   IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_NODELIST_DESTROY, IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_NODELIST_DESTROY, IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_NODELIST_DESTROY, IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_ATTRLIST_CREATE,  IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ATTRLIST_CREATE,  IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ATTRLIST_CREATE,  IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_ATTRLIST_CLEAR,   IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ATTRLIST_CLEAR,   IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ATTRLIST_CLEAR,   IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_ATTRLIST_DESTROY, IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ATTRLIST_DESTROY, IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ATTRLIST_DESTROY, IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_VALLIST_CREATE,   IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_VALLIST_CREATE,   IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_VALLIST_CREATE,   IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_VALLIST_CLEAR,    IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_VALLIST_CLEAR,    IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_VALLIST_CLEAR,    IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_VALLIST_DESTROY,  IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_VALLIST_DESTROY,  IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_VALLIST_DESTROY,  IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETNODENAME,      IS_NULL,                IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODENAME,      IS_NULL,                IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODENAME,      IS_NULL,                IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODENAME,      IS_NULL,                IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODENAME,      IS_NULL,                IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODENAME,      IS_NULL,                IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED }, 
  {GENDERS_GETNODENAME,      IS_NOT_NULL_LOADED,     IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_LOADED,     IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_LOADED,     IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODENAME,      IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_OVERFLOW  },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODENAME,      IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETNODES,         IS_NULL,                IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODES,         IS_NULL,                IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODES,         IS_NULL,                IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODES,         IS_NULL,                IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODES,         IS_NULL,                IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODES,         IS_NULL,                IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODES,         IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETNODES,         IS_NOT_NULL_LOADED,     IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_LOADED,     IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_LOADED,     IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETNODES,         IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_OVERFLOW  },
  {GENDERS_GETNODES,         IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODES,         IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODES,         IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODES,         IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODES,         IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETNODES,         IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETATTR,          IS_NULL,                IS_NULL,          IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NOT_NULL,      IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NULL,          IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NOT_NULL,      IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NULL,          IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NOT_NULL,      IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NULL,          IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NOT_NULL,      IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NULL,          IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NOT_NULL,      IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NULL,          IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NULL,                IS_NOT_NULL,      IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NULL,          IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NULL,          IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NULL,          IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_OVERFLOW  },
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NULL,          IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NULL,          IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NULL,          IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR,          IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_NOTFOUND  },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NEGATIVE,       GOOD_NODENAME,      -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_ZERO,           GOOD_NODENAME,      -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_POSITIVE_SMALL, GOOD_NODENAME,      -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NEGATIVE,       BAD_NODENAME,       -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_ZERO,           BAD_NODENAME,       -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR,          IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_POSITIVE_SMALL, BAD_NODENAME,       -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_GETATTR_ALL,      IS_NULL,                IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR_ALL,      IS_NULL,                IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR_ALL,      IS_NULL,                IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR_ALL,      IS_NULL,                IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR_ALL,      IS_NULL,                IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR_ALL,      IS_NULL,                IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_LOADED,     IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_LOADED,     IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_LOADED,     IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_OVERFLOW  },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NEGATIVE,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_ZERO,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_GETATTR_ALL,      IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_POSITIVE_SMALL, NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_TESTATTR,         IS_NULL,                GOOD_NODENAME,    IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                GOOD_NODENAME,    IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                BAD_NODENAME,     IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                BAD_NODENAME,     IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                GOOD_NODENAME,    IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                GOOD_NODENAME,    IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                BAD_NODENAME,     IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                BAD_NODENAME,     IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                GOOD_NODENAME,    IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                GOOD_NODENAME,    IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                BAD_NODENAME,     IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NULL,                BAD_NODENAME,     IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_OVERFLOW  },
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTR,         IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_NOTFOUND  },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NULL,           IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NOT_NULL,       IS_POSITIVE_SMALL,  -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NULL,           IS_ZERO,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NOT_NULL,       IS_ZERO,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NULL,           IS_NEGATIVE,        -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTR,         IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NOT_NULL,       IS_NEGATIVE,        -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_TESTATTRVAL,      IS_NULL,                GOOD_NODENAME,    IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                GOOD_NODENAME,    IS_NOT_NULL,       IS_NULL,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                GOOD_NODENAME,    IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                GOOD_NODENAME,    IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                BAD_NODENAME,     IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                BAD_NODENAME,     IS_NOT_NULL,       IS_NULL,            -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                BAD_NODENAME,     IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NULL,                BAD_NODENAME,     IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NOT_NULL,       IS_NULL,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, GOOD_NODENAME,    IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NOT_NULL,       IS_NULL,            -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_NOT_LOADED, BAD_NODENAME,     IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_LOADED,     GOOD_NODENAME,    IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_LOADED,     BAD_NODENAME,     IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_NOTFOUND  },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NOT_NULL,       IS_NULL,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  GOOD_NODENAME,    IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NULL,           IS_NULL,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NOT_NULL,       IS_NULL,            -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NULL,           IS_NOT_NULL,        -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_TESTATTRVAL,      IS_NOT_NULL_DESTROYED,  BAD_NODENAME,     IS_NOT_NULL,       IS_NOT_NULL,        -1,                     GENDERS_ERR_MAGIC     },
 
  {GENDERS_ISNODE,           IS_NULL,                NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISNODE,           IS_NOT_NULL_NOT_LOADED, NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISNODE,           IS_NOT_NULL_DESTROYED,  NO_VALUE,         NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_ISATTR,           IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISATTR,           IS_NULL,                IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISATTR,           IS_NOT_NULL_NOT_LOADED, IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISATTR,           IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISATTR,           IS_NOT_NULL_LOADED,     IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ISATTR,           IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ISATTR,           IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_ISATTRVAL,        IS_NULL,                IS_NULL,          IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISATTRVAL,        IS_NULL,                IS_NOT_NULL,      IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISATTRVAL,        IS_NULL,                IS_NULL,          IS_NOT_NULL,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISATTRVAL,        IS_NULL,                IS_NOT_NULL,      IS_NOT_NULL,       NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_NOT_LOADED, IS_NULL,          IS_NOT_NULL,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL,      IS_NOT_NULL,       NO_VALUE,           -1,                     GENDERS_ERR_NOTLOADED },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_LOADED,     IS_NULL,          IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_LOADED,     IS_NOT_NULL,      IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_LOADED,     IS_NULL,          IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_PARAMETERS},
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NULL,           NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_DESTROYED,  IS_NULL,          IS_NOT_NULL,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_ISATTRVAL,        IS_NOT_NULL_DESTROYED,  IS_NOT_NULL,      IS_NOT_NULL,       NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {GENDERS_PARSE,            IS_NULL,                IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_PARSE,            IS_NULL,                IS_NOT_NULL_GOOD, NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_PARSE,            IS_NULL,                IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_NULLHANDLE},
  {GENDERS_PARSE,            IS_NOT_NULL_LOADED,     IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_OPEN      },
  {GENDERS_PARSE,            IS_NOT_NULL_NOT_LOADED, IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_OPEN      },
  {GENDERS_PARSE,            IS_NOT_NULL_DESTROYED,  IS_NULL,          NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_PARSE,            IS_NOT_NULL_DESTROYED,  IS_NOT_NULL_GOOD, NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },
  {GENDERS_PARSE,            IS_NOT_NULL_DESTROYED,  IS_NOT_NULL_BAD,  NO_VALUE,          NO_VALUE,           -1,                     GENDERS_ERR_MAGIC     },

  {-1, -1, -1, -1, -1, -1},
};
