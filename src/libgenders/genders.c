/*****************************************************************************\
 *  $Id: genders.c,v 1.72 2003-12-30 21:47:13 achu Exp $
 *****************************************************************************
 *  Copyright (C) 2001-2003 The Regents of the University of California.
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
 *  with Genders; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
\*****************************************************************************/

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "genders.h"
#include "fd.h"
#include "list.h"

#define GENDERS_MAGIC_NUM         0xdeadbeef

#define GENDERS_READLINE_BUFLEN   65536

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN            64
#endif

#ifndef MAX
#define MAX(x,y)                  ((x > y) ? x : y)
#endif

struct genders {
  int magic;                          /* magic number */ 
  int errnum;                         /* error code */
  int is_loaded;                      /* genders data loaded? */
  int numnodes;                       /* number of nodes in database */
  int numattrs;                       /* number of attrs in database */
  int maxattrs;                       /* max attrs any one node has */
  int maxnodelen;                     /* max node name length */
  int maxattrlen;                     /* max attr name length */
  int maxvallen;                      /* max value name length */
  char nodename[MAXHOSTNAMELEN+1];    /* current hostname */
  List nodes;                         /* node names */
  List attrs;                         /* unique attributes */
};

/* stores node name and node attributes */
struct genders_node {
  char name[MAXHOSTNAMELEN+1];
  List attrvals;
};
typedef struct genders_node *genders_node_t;

/* stores attribute name and its value (if no value, val == NULL) */
struct genders_attrval {
  char *attr;
  char *val;
};
typedef struct genders_attrval *genders_attrval_t;

/* Error messages */
static char * errmsg[] = {
  "success",
  "genders handle is null",
  "open file error",
  "read file error",
  "genders file parse error",
  "genders data not loaded",
  "genders data already loaded",
  "array or string passed in not large enough to store result",
  "incorrect parameters passed in",
  "null pointer reached in list", 
  "node not found",
  "out of memory",
  "genders handle magic number incorrect, improper handle passed in",
  "unknown internal error"
  "error number out of range",
};

/* List Helper Functions */

static int _is_all(void *x, void *key) {
  return 1;
}

static int _is_str(void *x, void *key) {
  if (strcmp((char *)x, (char *)key) == 0)
    return 1;
  return 0;
}

static int _is_node(void *x, void *key) {
  genders_node_t n = (genders_node_t)x;
  if (strcmp(n->name, (char *)key) == 0)
    return 1;
  return 0;
}

static int _is_attr(void *x, void *key) {
  genders_attrval_t av = (genders_attrval_t)x;
  if (strcmp(av->attr, (char *)key) == 0)
    return 1;
  return 0;
}

static void _free_genders_node(void *x) {
  genders_node_t n = (genders_node_t)x;
  list_destroy(n->attrvals);
  free(n);
}

static void _free_genders_attrval(void *x) {
  genders_attrval_t av = (genders_attrval_t)x;
  free(av->attr);
  free(av->val);
  free(av);
}

/* API and API helper functions */

static int _handle_error_check(genders_t handle) {
  if (handle == NULL || handle->magic != GENDERS_MAGIC_NUM)
    return -1;
  return 0;
}

static int _unloaded_handle_error_check(genders_t handle) {
  if (_handle_error_check(handle) < 0)
    return -1;

  if (handle->is_loaded) {
    handle->errnum = GENDERS_ERR_ISLOADED;
    return -1;
  }
  
  return 0;
}

static int _loaded_handle_error_check(genders_t handle) {
  if (_handle_error_check(handle) < 0)
    return -1;

  if (!handle->is_loaded) {
    handle->errnum = GENDERS_ERR_NOTLOADED;
    return -1;
  }
  
  return 0;
}

static void _initialize_handle(genders_t handle) {
  handle->magic = GENDERS_MAGIC_NUM;
  handle->is_loaded = 0;
  handle->numnodes = 0;
  handle->numattrs = 0;
  handle->maxattrs = 0;
  handle->maxnodelen = 0;
  handle->maxattrlen = 0;
  handle->maxvallen = 0;
  memset(handle->nodename,'\0',MAXHOSTNAMELEN+1);
}

genders_t genders_handle_create(void) {
  genders_t handle;

  if (!(handle = (genders_t)malloc(sizeof(struct genders))))
    return NULL;
  
  _initialize_handle(handle);
  if (!(handle->nodes = list_create(_free_genders_node))) {
    free(handle);
    return NULL;
  }
  if (!(handle->attrs = list_create(free))) {
    list_destroy(handle->nodes);
    free(handle);
    return NULL;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle;
}

static void _free_list_data(genders_t handle) {
  list_delete_all(handle->nodes, _is_all, ""); /* pass junk for key */
  list_delete_all(handle->attrs, _is_all, ""); /* pass junk for key */
}

int genders_handle_destroy(genders_t handle) {
  if (_handle_error_check(handle) < 0)
    return -1;

  _free_list_data(handle);
  list_destroy(handle->nodes);
  list_destroy(handle->attrs);

  /* "clean" magic number */
  handle->magic = ~GENDERS_MAGIC_NUM; /* ~0xdeadbeef == 0xlivebeef :-) */
  free(handle);
  return 0;
}

static List _insert_node(genders_t handle, char *nodename) {
  genders_node_t n = NULL;

  if (!(n = (genders_node_t)malloc(sizeof(struct genders_node)))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return NULL;
  }

  if (!(n->attrvals = list_create(_free_genders_attrval))) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    free(n);
    return NULL;
  }

  memset(n->name, '\0', MAXHOSTNAMELEN+1);
  strcpy(n->name, nodename);    /* length previously asserted */

  if (list_append(handle->nodes, n) == NULL) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    _free_genders_node(n);
    return NULL;
  }
  return n->attrvals;
}

static int _insert_attrval(genders_t handle, List attrvals, 
                           char *attr, char *val) {
  genders_attrval_t av = NULL;
  int size = sizeof(struct genders_attrval);

  if (!(av = (genders_attrval_t)malloc(size))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  if (!(av->attr = strdup(attr))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    free(av);
    return -1;
  }

  if (val != NULL) {
    if (!(av->val = strdup(val))) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      free(av->attr);
      free(av);
      return -1;
    }
  }
  else 
    av->val = NULL; 

  if (list_append(attrvals, av) == NULL) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    _free_genders_attrval(av);
    return -1;
  }
  return 0;
}

static int _insert_attr(genders_t handle, char *attr) {
  char *attr_new = NULL;

  if (list_find_first(handle->attrs, _is_str, attr) != NULL)
    return 0;

  if (!(attr_new = strdup(attr))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  if (list_append(handle->attrs, attr_new) == NULL) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    free(attr_new);
    return -1;
  }

  return 1;
}

static int _readline(genders_t handle, int fd, char *buf, int buflen) {
  int ret; 

  if ((ret = fd_read_line(fd, buf, buflen)) < 0) {
    handle->errnum = GENDERS_ERR_READ;
    return -1;
  }
  
  /* XXX assumes buflen >= 2 */
  if (ret == buflen && buf[buflen-2] != '\n') {
      handle->errnum = GENDERS_ERR_OVERFLOW;
      return -1;
  }

  return ret;
}

/* parse a genders file line
 * - If line_num == 0, parse and store genders data
 * - If line_num > 0, debug genders file, do not store genders data
 * - Returns -1 on error, 1 if there was a parse error, 0 if no errors
 */
static int _parse_line(genders_t handle, char *line, 
                       int line_num, FILE *stream) {
  char *linebuf, *temp, *attr, *node;
  int attrcount = 0;
  List attrvals = NULL;

  /* "remove" comments */
  if ((temp = strchr(line, '#'))) 
    *temp = '\0';

  /* remove trailing white space, including newline */
  temp = line + strlen(line);
  for (--temp; temp >= line; temp--) {
    if (isspace(*temp))
      *temp = '\0';
    else
      break;
  }

  /* empty line */
  if (*line == '\0')
    return 0;

  /* move forward to node name */
  while(isspace(*line))  
    line++;

  /* get node name */
  if ((node = strsep(&line, " \t\0")) != NULL) { 

    if (strlen(node) > MAXHOSTNAMELEN) {
      if (line_num > 0) {
        fprintf(stream, "Line %d: hostname too long\n", line_num);
        return 1;
      }
      handle->errnum = GENDERS_ERR_PARSE;
      return -1;
    }
      
    if (line_num == 0) {
      if (!(attrvals = _insert_node(handle, node)))
        return -1;
      
      handle->maxnodelen = MAX(strlen(node), handle->maxnodelen);
      handle->numnodes++;
    }
      
    /* line has no attributes */
    if (line == NULL)
      return 0;

    /* move forward to attributes */
    while(isspace(*line))  
      line++;
  }

  /* just incase */
  if (*line == '\0')
    return 0;

  if (strchr(line,' ') != NULL || strchr(line,'\t') != NULL) {
    if (line_num > 0) {
      fprintf(stream, "Line %d: white space in attribute list\n", line_num);
      return 1;
    }
    handle->errnum = GENDERS_ERR_PARSE;
    return -1;
  }

  if (line_num > 0)
    return 0;

  /* parse attributes */
  attr = strtok_r(line,",\0",&linebuf);
  while (attr != NULL) {
    char *val = NULL;
    int is_new_attr;

    /* parse value out of attribute */
    if ((val = strchr(attr,'=')) != NULL)
      *val++ = '\0'; 

    if (_insert_attrval(handle, attrvals, attr, val) < 0)
      return -1;

    if ((is_new_attr = _insert_attr(handle, attr)) < 0)
      return -1;
      
    handle->numattrs += is_new_attr;
    attrcount++;

    handle->maxattrlen = MAX(strlen(attr), handle->maxattrlen);
    if (val)
      handle->maxvallen = MAX(strlen(val), handle->maxvallen);

    attr = strtok_r(NULL,",\0",&linebuf);
  }

  handle->maxattrs = MAX(attrcount, handle->maxattrs);

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int genders_load_data(genders_t handle, const char *filename) {
  char *temp;
  int ret, fd = -1;
  char buf[GENDERS_READLINE_BUFLEN];

  if (_unloaded_handle_error_check(handle) < 0)
    return -1;

  if (filename == NULL)
    filename = GENDERS_DEFAULT_FILE;

  if ((fd = open(filename, O_RDONLY)) < 0) {
    handle->errnum = GENDERS_ERR_OPEN;
    goto cleanup;
  }

  /* parse line by line */
  while ((ret = _readline(handle, fd, buf, GENDERS_READLINE_BUFLEN)) > 0) {
    if (_parse_line(handle, buf, 0, NULL) < 0)
      goto cleanup;
  }

  if (ret < 0)
    goto cleanup;

  /* umm, where are the nodes? */
  if (handle->numnodes == 0) {
    handle->errnum = GENDERS_ERR_PARSE;
    goto cleanup;
  }

  if (gethostname(handle->nodename, MAXHOSTNAMELEN+1) < 0) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }
  handle->nodename[MAXHOSTNAMELEN]='\0';

  /* shorten hostname if necessary */
  if ((temp = strchr(handle->nodename,'.')))
    *temp = '\0';
  
  handle->maxnodelen = MAX(strlen(handle->nodename), handle->maxnodelen);

  close(fd);
  handle->is_loaded++;
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;

cleanup:
  close(fd);
  _free_list_data(handle);
  _initialize_handle(handle);
  return -1;
}

int genders_errnum(genders_t handle) {
  if (handle == NULL)
    return GENDERS_ERR_NULLHANDLE;
  else if (handle->magic != GENDERS_MAGIC_NUM)
    return GENDERS_ERR_MAGIC;
  else
    return handle->errnum;
}

char *genders_strerror(int errnum) {
  if (errnum >= GENDERS_ERR_SUCCESS && errnum <= GENDERS_ERR_ERRNUMRANGE)
    return errmsg[errnum];
  else
    return errmsg[GENDERS_ERR_ERRNUMRANGE];
}

char *genders_errormsg(genders_t handle) {
  return genders_strerror(genders_errnum(handle));
}

void genders_perror(genders_t handle, const char *msg) {
  char *errormsg = genders_strerror(genders_errnum(handle));

  if (msg == NULL)
    fprintf(stderr, "%s\n", errormsg);
  else
    fprintf(stderr, "%s: %s\n", msg, errormsg);
}

int genders_getnumnodes(genders_t handle) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numnodes;
}

int genders_getnumattrs(genders_t handle) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numattrs;
}

int genders_getmaxattrs(genders_t handle) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrs;
}

int genders_getmaxnodelen(genders_t handle) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxnodelen;
}

int genders_getmaxattrlen(genders_t handle) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrlen;
}

int genders_getmaxvallen(genders_t handle) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxvallen;
}

static int _list_create(genders_t handle, char ***list, int len, int buflen) {
  if (len > 0) {

    if (list == NULL) {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

    char **temp;
    int i,j;

    if (!(temp = (char **)malloc(sizeof(char *) * len))) { 
      handle->errnum = GENDERS_ERR_OUTMEM;
      return -1;
    }

    for (i = 0; i < len; i++) {
      if (!(temp[i] = (char *)malloc(buflen))) {
        for (j = 0; j < i; j++)
          free(temp[j]);
        free(temp);

        handle->errnum = GENDERS_ERR_OUTMEM;
        return -1;
      }
      memset(temp[i], '\0', buflen);
    }

    *list = temp;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return len;
}

static int _list_clear(genders_t handle, char **list, int len, int buflen) {
  int i;

  if (len > 0) {

    if (list == NULL) {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }
    
    for (i = 0; i < len; i++) {
      if (list[i] == NULL) {
        handle->errnum = GENDERS_ERR_NULLPTR;
        return -1;
      }
      memset(list[i], '\0', buflen);
    }
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

static int _list_destroy(genders_t handle, char **list, int len) {
  if (len > 0) {
    if (list == NULL) {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

    int i;
    for (i = 0; i < len; i++)
      free(list[i]);
    free(list);
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int genders_nodelist_create(genders_t handle, char ***nodelist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_create(handle, nodelist, handle->numnodes, handle->maxnodelen+1);
}

int genders_nodelist_clear(genders_t handle, char **nodelist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_clear(handle, nodelist, handle->numnodes, handle->maxnodelen+1);
}

int genders_nodelist_destroy(genders_t handle, char **nodelist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_destroy(handle, nodelist, handle->numnodes);
}

int genders_attrlist_create(genders_t handle, char ***attrlist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_create(handle, attrlist, handle->numattrs, handle->maxattrlen+1);
}

int genders_attrlist_clear(genders_t handle, char **attrlist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_clear(handle, attrlist, handle->numattrs, handle->maxattrlen+1);
}

int genders_attrlist_destroy(genders_t handle, char **attrlist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;
  
  return _list_destroy(handle, attrlist, handle->numattrs);
}

int genders_vallist_create(genders_t handle, char ***vallist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_create(handle, vallist, handle->numattrs, handle->maxvallen+1);
}

int genders_vallist_clear(genders_t handle, char **vallist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_clear(handle, vallist, handle->numattrs, handle->maxvallen+1);
}

int genders_vallist_destroy(genders_t handle, char **vallist) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_destroy(handle, vallist, handle->numattrs);
}

int genders_getnodename(genders_t handle, char *node, int len) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (node == NULL || len <= 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if ((strlen(handle->nodename) + 1) > len) {
    handle->errnum = GENDERS_ERR_OVERFLOW;
    return -1;
  }

  strcpy(node, handle->nodename);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

static int _put_in_list(genders_t handle, char *str, 
                        char **list, int index, int len) {
  if (index >= len) {
    handle->errnum = GENDERS_ERR_OVERFLOW;
    return -1;
  }

  if (list[index] == NULL) {
    handle->errnum = GENDERS_ERR_NULLPTR;
    return -1;
  }
  
  strcpy(list[index], str); 
  return 0;
}

int genders_getnodes(genders_t handle, char *nodes[], int len, 
                     const char *attr, const char *val) {
  ListIterator itr;
  genders_node_t n;
  genders_attrval_t av;
  int index = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((nodes == NULL && len > 0) || len < 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (!(itr = list_iterator_create(handle->nodes))) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    return -1;
  }

  while ((n = list_next(itr))) {
    int save = 0;

    if (attr == NULL)
      save++;
    else {
      if ((av = list_find_first(n->attrvals, _is_attr, (char *)attr)))
        save = (val == NULL) || (av->val != NULL && strcmp(av->val, val) == 0);
    }
  
    if (save && _put_in_list(handle, n->name, nodes, index++, len) < 0)
      return -1;
  }

  list_iterator_destroy(itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return index;
}

int genders_getattr(genders_t handle, char *attrs[], char *vals[], 
                    int len, const char *node) {
  ListIterator itr;
  genders_node_t n;
  genders_attrval_t av;
  int index = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((attrs == NULL && len > 0) || len < 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (!(n = list_find_first(handle->nodes, _is_node, (char *)node))) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if (!(itr = list_iterator_create(n->attrvals))) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    return -1;
  }

  while ((av = list_next(itr))) {
    if (_put_in_list(handle, av->attr, attrs, index, len) < 0)
      return -1;
    
    if (vals != NULL && av->val != NULL) {
      if (_put_in_list(handle, av->val, vals, index, len) < 0)
        return -1;
    }
    index++;
  }
  
  list_iterator_destroy(itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return index;  
}

int genders_getattr_all(genders_t handle, char *attrs[], int len) {
  ListIterator itr; 
  char *attr;
  int index = 0;
  
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((attrs == NULL && len > 0) || len < 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (handle->numattrs > len) {
    handle->errnum = GENDERS_ERR_OVERFLOW;
    return -1;
  }

  if (!(itr = list_iterator_create(handle->attrs))) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    return -1;
  }

  while ((attr = list_next(itr))) {
    if (_put_in_list(handle, attr, attrs, index++, len) < 0)
      return -1;
  }

  list_iterator_destroy(itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return index;  
}

int genders_testattr(genders_t handle, const char *node, const char *attr, 
                     char *val, int len) {
  genders_node_t n;
  genders_attrval_t av;
  int retval = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL || (val != NULL && len <= 0)) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (!(n = list_find_first(handle->nodes, _is_node, (char *)node))) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if ((av = list_find_first(n->attrvals, _is_attr, (char *)attr))) {
    if (val != NULL && av->val != NULL) {
      if ((strlen(av->val) + 1) > len) {
        handle->errnum = GENDERS_ERR_OVERFLOW;
        return -1;
      }
      strcpy(val, av->val);
    }
    retval = 1;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_testattrval(genders_t handle, const char *node, 
                        const char *attr, const char *val) {
  genders_node_t n;
  genders_attrval_t av;
  int retval = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (!(n = list_find_first(handle->nodes, _is_node, (char *)node))) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }
  
  if ((av = list_find_first(n->attrvals, _is_attr, (char *)attr))) {
    if ((val == NULL) || ((av->val != NULL) && (strcmp(av->val, val) == 0)))
      retval = 1;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_isnode(genders_t handle, const char *node) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (node == NULL)
    node = handle->nodename;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return (list_find_first(handle->nodes, _is_node, (char *)node)) ? 1 : 0;
}

int genders_isattr(genders_t handle, const char *attr) {
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return (list_find_first(handle->attrs, _is_str, (char *)attr)) ? 1 : 0;
}

int genders_isattrval(genders_t handle, const char *attr, const char *val) {
  ListIterator itr;
  genders_node_t n;
  genders_attrval_t av;
  int retval = 0;
  
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL || val == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }
  
  if (!(itr = list_iterator_create(handle->nodes))) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    return -1;
  }

  while ((n = list_next(itr))) {
    if ((av = list_find_first(n->attrvals, _is_attr, (char *)attr))) {
      if (av->val != NULL && strcmp(av->val, val) == 0)
        retval = 1;
    }
  }

  list_iterator_destroy(itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_parse(genders_t handle, const char *filename, FILE *stream) {
  int line_count = 1;
  int retval = 0;
  int rv, ret, fd = -1;
  char buf[GENDERS_READLINE_BUFLEN];

  if (_handle_error_check(handle) < 0)
    return -1;

  if (filename == NULL)
    filename = GENDERS_DEFAULT_FILE;
  
  if (stream == NULL)
    stream = stderr;

  if ((fd = open(filename,O_RDONLY)) < 0) {
    handle->errnum = GENDERS_ERR_OPEN;
    return -1;
  }

  while ((ret = _readline(handle, fd, buf, GENDERS_READLINE_BUFLEN)) > 0) {
    if ((rv = _parse_line(handle, buf, line_count, stream)) < 0) {
      retval = -1;
      goto finish;
    }

    retval += rv;
    line_count++;
  }

  if (ret < 0 && handle->errnum == GENDERS_ERR_OVERFLOW) {
    fprintf(stderr, "Line %d: exceeds maximum allowed length\n", line_count);
    retval = -1;
  }

 finish:
  close(fd);
  return retval;
}

void genders_set_errnum(genders_t handle, int errnum) {
  if (_handle_error_check(handle) < 0)
    return;

  if (errnum >= GENDERS_ERR_SUCCESS && errnum <= GENDERS_ERR_ERRNUMRANGE)
    handle->errnum = errnum;
  else
    handle->errnum = GENDERS_ERR_INTERNAL;
}
