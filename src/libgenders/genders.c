/*****************************************************************************\
 *  $Id: genders.c,v 1.82 2004-01-28 01:06:39 achu Exp $
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
#include "hostlist.h"

#define GENDERS_MAGIC_NUM         0xdeadbeef

#define GENDERS_READLINE_BUFLEN   65536

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN            64
#endif

#ifndef MAX
#define MAX(x,y)                  ((x > y) ? x : y)
#endif

/* stores node name and a list of pointers to attrval lists containing
 * the attributes and values of this node.  The pointers point to
 * lists stored within the attrvalslist parameter of the genders
 * handle.
 */
struct genders_node {
  char name[MAXHOSTNAMELEN+1];
  List attrlist;
};
typedef struct genders_node *genders_node_t;

/* stores attribute name and its value (if no value, val == NULL) */
struct genders_attrval {
  char *attr;
  char *val;
};
typedef struct genders_attrval *genders_attrval_t;

/* Genders handle, caches all information loaded from a genders
 * database.  Consider the following genders database
 *
 * nodename[1-2]  attrname1=val1,attrname2=val2
 * nodename1      attrname3=val3,attrname4
 * nodename2      attrname5   
 * nodename3      attrname6
 *
 * After the genders database has been loaded using genders_load_data,
 * the lists and data in the handle can be viewed like the following:
 *
 * magic = GENDERS_MAGIC_NUM
 * errnum = current error code
 * is_loaded = 1
 * numnodes = 3
 * numattrs = 6 
 * maxnodelen = 9 (localhost)
 * maxattrlen = 14 (5 + 9, 9 added for potential %n substitution)
 * maxvallen = 13 (4 + 9, 9 added for %n substitution)
 * nodename = localhost
 * nodeslist = node1 -> node2 -> node3 -> \0
 *    node1.name = nodename1, node1.attrlist = listptr1 -> listptr2 -> \0
 *    node2.name = nodename2, node2.attrlist = listptr1 -> listptr3 -> \0
 *    node3.name = nodename3, node3.attrlist = listptr4 -> \0
 * attrvalslist = listptr1 -> listptr2 -> listptr3 -> listptr4 -> \0
 *    listptr1 = attr1 -> attr2 -> \0
 *    listptr2 = attr3 -> attr4 -> \0
 *    listptr3 = attr5 -> \0
 *    listptr4 = attr6 -> \0
 *      attr1.attr = attrname1, attr1.val = val1
 *      attr2.attr = attrname2, attr2.val = val2
 *      attr3.attr = attrname3, attr3.val = val3
 *      attr4.attr = attrname4, attr4.val = NULL
 *      attr5.attr = attrname5, attr5.val = NULL
 *      attr6.attr = attrname6, attr6.val = NULL
 * attrslist = attrname1 -> attrname2 -> attrname3 -> attrname4 -> 
 *             attrname5 -> attrname6 -> \0
 */
struct genders {
  int magic;                        /* magic number */ 
  int errnum;                       /* error code */
  int is_loaded;                    /* genders data loaded? */
  int numnodes;                     /* number of nodes in database */
  int numattrs;                     /* number of attrs in database */
  int maxnodelen;                   /* max node name length */
  int maxattrlen;                   /* max attr name length */
  int maxvallen;                    /* max value name length */
  char nodename[MAXHOSTNAMELEN+1];  /* local hostname */
  List nodeslist;                   /* Lists of genders_node */
  List attrvalslist;                /* Lists of ptrs to Lists of genders_attrvals */
  List attrslist;                   /* List of unique attribute strings */
};

/* Error messages */
static char * errmsg[] = {
  "success",
  "genders handle is null",
  "error opening genders file",
  "error reading genders file",
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

/* List API Helper Functions */

static int 
_is_all(void *x, void *key) 
{
  return 1;
}

static int 
_is_str(void *x, void *key) 
{
  if (strcmp((char *)x, (char *)key) == 0)
    return 1;
  return 0;
}

static int 
_is_node(void *x, void *key) 
{
  genders_node_t n = (genders_node_t)x;
  if (strcmp(n->name, (char *)key) == 0)
    return 1;
  return 0;
}

static int
_is_attr_in_attrvals(void *x, void *key)
{
  genders_attrval_t av = (genders_attrval_t)x;
  if (strcmp(av->attr, (char *)key) == 0)
    return 1;
  return 0;
}

static int 
_is_attr_in_attrlist(void *x, void *key) 
{
  List attrvals = (List)x;
  if (list_find_first(attrvals, _is_attr_in_attrvals, key) != NULL)
    return 1;
  return 0;
}

static void 
_free_genders_node(void *x) 
{
  genders_node_t n = (genders_node_t)x;
  list_destroy(n->attrlist);
  free(n);
}

static void 
_free_genders_attrval(void *x) 
{
  genders_attrval_t av = (genders_attrval_t)x;
  free(av->attr);
  free(av->val);
  free(av);
}

/* API and API helper functions */

static int 
_handle_error_check(genders_t handle) 
{
  if (handle == NULL || handle->magic != GENDERS_MAGIC_NUM)
    return -1;
  return 0;
}

static int 
_unloaded_handle_error_check(genders_t handle) 
{
  if (_handle_error_check(handle) < 0)
    return -1;

  if (handle->is_loaded) {
    handle->errnum = GENDERS_ERR_ISLOADED;
    return -1;
  }
  
  return 0;
}

static int 
_loaded_handle_error_check(genders_t handle) 
{
  if (_handle_error_check(handle) < 0)
    return -1;

  if (!handle->is_loaded) {
    handle->errnum = GENDERS_ERR_NOTLOADED;
    return -1;
  }
  
  return 0;
}

static int
_find_attrval_in_attrlist(genders_t handle, List attrlist, const char *attr, 
                          genders_attrval_t *avptr)
{
  ListIterator itr;
  ListIterator avitr;
  List attrvals;
  genders_attrval_t av;
  int retval = -1;
  
  if (!(itr = list_iterator_create(attrlist))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  *avptr = NULL;
  while ((attrvals = list_next(itr))) {
    if ((avitr = list_iterator_create(attrvals)) == NULL) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }

    while ((av = list_next(avitr))) {
      if (strcmp(av->attr, attr) == 0) {
	*avptr = av;
        retval = 0;
        goto cleanup;
      }
    }
    list_iterator_destroy(avitr);
    avitr = NULL;
  }

  retval 0;
 cleanup:
  if (itr)
    list_iterator_destroy(itr);
  if (avitr)
    list_iterator_destroy(avitr);
  return retval;  
}

static void 
_initialize_handle(genders_t handle) 
{
  handle->magic = GENDERS_MAGIC_NUM;
  handle->is_loaded = 0;
  handle->numnodes = 0;
  handle->numattrs = 0;
  handle->maxnodelen = 0;
  handle->maxattrlen = 0;
  handle->maxvallen = 0;
  memset(handle->nodename,'\0',MAXHOSTNAMELEN+1);
}

static void 
_free_handle_list_data(genders_t handle) 
{
  /* Can't pass NULL for key, so pass junk, _is_all() will ensure
   * everything is deleted
   */
  list_delete_all(handle->nodeslist, _is_all, ""); 
  list_delete_all(handle->attrvalslist, _is_all, ""); 
  list_delete_all(handle->attrslist, _is_all, ""); 
}

genders_t 
genders_handle_create(void) 
{
  genders_t handle = NULL;

  if (!(handle = (genders_t)malloc(sizeof(struct genders))))
    return NULL;
  
  _initialize_handle(handle);
  handle->nodeslist = NULL;
  handle->attrvalslist = NULL;
  handle->attrslist = NULL;

  if (!(handle->nodeslist = list_create(_free_genders_node)))
    goto cleanup;

  if (!(handle->attrvalslist = list_create(NULL)))
    goto cleanup;

  if (!(handle->attrslist = list_create(free)))
    goto cleanup;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle;

cleanup:
  if (handle) {
    if (handle->nodeslist)
      list_destroy(handle->nodeslist);
    if (handle->attrvalslist)
      list_destroy(handle->attrvalslist);
    if (handle->attrslist)
      list_destroy(handle->attrslist);
    free(handle);
  }
  return NULL;
}

int 
genders_handle_destroy(genders_t handle) 
{
  if (_handle_error_check(handle) < 0)
    return -1;

  list_destroy(handle->nodeslist);
  list_destroy(handle->attrvalslist);
  list_destroy(handle->attrslist);

  /* "clean" magic number */
  handle->magic = ~GENDERS_MAGIC_NUM;
  free(handle);
  return 0;
}

static int 
_readline(genders_t handle, int fd, char *buf, int buflen) 
{
  int ret; 

  if ((ret = fd_read_line(fd, buf, buflen)) < 0) {
    handle->errnum = GENDERS_ERR_READ;
    return -1;
  }
  
  /* buflen - 1 b/c fd_read_line guarantees null termination */
  if (ret >= (buflen-1)) {
      handle->errnum = GENDERS_ERR_PARSE;
      return -1;
  }

  return ret;
}

static genders_node_t
_insert_node(genders_t handle, List nodelist, char *nodename)
{
  genders_node_t n = NULL;

  n = list_find_first(nodelist, _is_node, nodename);

  /* must create if node not yet in list */ 
  if (n == NULL) {
    if (!(n = (genders_node_t)malloc(sizeof(struct genders_node)))) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }
    memset(n, '\0', sizeof(struct genders_node));

    strcpy(n->name, nodename);    /* length previously asserted */

    if (!(n->attrlist = list_create(NULL))) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }

    if (list_append(nodelist, n) == NULL) {
      handle->errnum = GENDERS_ERR_INTERNAL;
      goto cleanup;
    }
  }

  return n;

 cleanup:
  if (n != NULL) { 
    if (n->attrlist)
      list_destroy(n->attrlist);
    free(n);
  }
  return NULL;
}

static int 
_insert_attrval(genders_t handle, List attrvals, char *attr, char *val) 
{
  genders_attrval_t av = NULL;

  if (!(av = (genders_attrval_t)malloc(sizeof(struct genders_attrval)))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }
  memset(av, '\0', sizeof(struct genders_attrval));

  if (!(av->attr = strdup(attr))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  if (val != NULL) {
    if (!(av->val = strdup(val))) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }
  }
  else 
    av->val = NULL; 

  if (list_append(attrvals, av) == NULL) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }

  return 0;
 cleanup:
  if (av != NULL) {
    if (av->attr)
      free(av->attr);
    if (av->val)
      free(av->val);
    free(av);
  }
  return -1;
}

static int 
_insert_ptr(genders_t handle, List ptrlist, void *ptr) 
{
  if (list_append(ptrlist, ptr) == NULL) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    return -1;
  }
  return 0;
}

static int 
_insert_attr(genders_t handle, char *attr) 
{
  char *attr_new = NULL;

  if (list_find_first(handle->attrslist, _is_str, attr) != NULL)
    return 0;

  if (!(attr_new = strdup(attr))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  if (list_append(handle->attrslist, attr_new) == NULL) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    free(attr_new);
    return -1;
  }

  return 1;
}

static int
_duplicate_attr_check(genders_t handle, List attrlist, List attrvals)
{
  ListIterator itr = NULL;
  char *attr = NULL;
  int retval = -1;

  if ((itr = list_iterator_create(attrvals)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  while ((attr = list_next(itr))) {
    if (list_find_first(attrlist, _is_attr_in_attrlist, attr) != NULL) {
      retval = 1;
      goto cleanup;
    }
  }

  retval = 0;
 cleanup:
  if (itr)
    list_iterator_destroy(itr);
  return retval;
}

/* parse a genders file line
 * - If line_num == 0, parse and store genders data
 * - If line_num > 0, debug genders file, do not store genders data, use
 *   debug lists instead of handle lists for debugging.
 * - Returns -1 on error, 1 if there was a parse error, 0 if no errors
 * - No need to cleanup if line_num == 0, will be done in genders_load_data.
 */
static int 
_parse_line(genders_t handle, char *line, int line_num, FILE *stream, 
	    List debugnodeslist, List debugattrvalslist) 
{
  char *linebuf, *temp, *attr, *nodenames, *node = NULL;
  int rv, attrcount = 0, retval = -1;
  List attrvals = NULL;
  List nodelist = NULL;
  List attrvalslist = NULL;
  genders_node_t n;
  hostlist_t hl = NULL;
  hostlist_iterator_t hlitr = NULL;

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

  /* move forward to node name(s) */
  while(isspace(*line))  
    line++;

  if ((attrvals = list_create(_free_genders_attrval)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  /* get node name(s) */
  if ((nodenames = strsep(&line, " \t\0")) == NULL)
    /* should be impossible to hit this */
    return 0;
  
  /* if stresp() sets line == NULL, line has no attributes */
  if (line != NULL) {
    /* move forward to attributes */
    while(isspace(*line))  
      line++;

    /* *line == '\0' means line has no attributes */
    if (*line != '\0') {
      if (strchr(line,' ') != NULL || strchr(line,'\t') != NULL) {
	if (line_num > 0) {
	  fprintf(stream, "Line %d: white space in attribute list\n", line_num);
	  retval = 1;
	}
	handle->errnum = GENDERS_ERR_PARSE;
	goto cleanup;
      }
      
      /* parse attributes */
      attr = strtok_r(line,",\0",&linebuf);
      while (attr != NULL) {
	char *val = NULL;
	int is_new_attr;
      
	/* parse value out of attribute */
	if ((val = strchr(attr,'=')) != NULL)
	  *val++ = '\0'; 
	
	if (_insert_attrval(handle, attrvals, attr, val) < 0)
	  goto cleanup;
	
	if (line_num == 0) { 
	  if ((is_new_attr = _insert_attr(handle, attr)) < 0)
	    goto cleanup;
	  
	  handle->numattrs += is_new_attr;
	  attrcount++;
	  
	  handle->maxattrlen = MAX(strlen(attr), handle->maxattrlen);
	  if (val)
	    handle->maxvallen = MAX(strlen(val), handle->maxvallen);
	}
	
	attr = strtok_r(NULL,",\0",&linebuf);
      }
    }
  }

  if (line_num == 0) {
    nodelist = handle->nodeslist;
    attrvalslist = handle->attrvalslist;
  }
  else {
    nodelist = debugnodeslist;
    attrvalslist = debugattrvalslist;
  }

  if ((hl = hostlist_create(nodenames)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  if ((hlitr = hostlist_iterator_create(hl)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  while ((node = hostlist_next(hlitr)) != NULL) {
    if (strlen(node) > MAXHOSTNAMELEN) {
      if (line_num > 0) {
	fprintf(stream, "Line %d: hostname too long\n", line_num);
	retval = 1;
      }
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }
    
    if (strchr(node, '.') != NULL) {
      if (line_num > 0) {
	fprintf(stream, "Line %d: node not a shortened hostname\n", line_num);
	retval = 1;
      }
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }

    if ((n =_insert_node(handle, nodelist, node)) == NULL)
      goto cleanup;

    if ((rv = _duplicate_attr_check(handle, n->attrlist, attrvals)) < 0)
      goto cleanup;

    if (rv == 1) {
      if (line_num > 0) {
	fprintf(stream, "Line %d: duplicate attributed listed for node %s\n",
		line_num, node);
	retval = 1;
      }
      handle->errnum = GENDERS_ERR_PARSE;
      goto cleanup;
    }
    
    if (_insert_ptr(handle, n->attrlist, attrvals) < 0)
      goto cleanup;

    if (line_num == 0)
      handle->maxnodelen = MAX(strlen(node), handle->maxnodelen);
    
    free(node);
  }
  node = NULL;

  /* Append at the very end, so cleanup area cleaner */
  if (_insert_ptr(handle, attrvalslist, attrvals) < 0)
    goto cleanup;
  attrvals = NULL;

  retval = 0;
 cleanup:
  if (hl != NULL) {
    if (hlitr != NULL)
      hostlist_iterator_destroy(hlitr);
    hostlist_destroy(hl);
  }
  if (attrvals)
    list_destroy(attrvals);
  free(node);
  return retval;
}

int 
genders_load_data(genders_t handle, const char *filename) 
{
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
    if (_parse_line(handle, buf, 0, NULL, NULL, NULL) < 0)
      goto cleanup;
  }

  if (ret < 0)
    goto cleanup;

  handle->numnodes = list_count(handle->nodeslist);

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
  _free_handle_list_data(handle);
  _initialize_handle(handle);
  return -1;
}

int 
genders_errnum(genders_t handle) 
{
  if (handle == NULL)
    return GENDERS_ERR_NULLHANDLE;
  else if (handle->magic != GENDERS_MAGIC_NUM)
    return GENDERS_ERR_MAGIC;
  else
    return handle->errnum;
}

char *
genders_strerror(int errnum) 
{
  if (errnum >= GENDERS_ERR_SUCCESS && errnum <= GENDERS_ERR_ERRNUMRANGE)
    return errmsg[errnum];
  else
    return errmsg[GENDERS_ERR_ERRNUMRANGE];
}

char *
genders_errormsg(genders_t handle) 
{
  return genders_strerror(genders_errnum(handle));
}

void 
genders_perror(genders_t handle, const char *msg) 
{
  char *errormsg = genders_strerror(genders_errnum(handle));

  if (msg == NULL)
    fprintf(stderr, "%s\n", errormsg);
  else
    fprintf(stderr, "%s: %s\n", msg, errormsg);
}

int 
genders_getnumnodes(genders_t handle) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numnodes;
}

int 
genders_getnumattrs(genders_t handle) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numattrs;
}

int 
genders_getmaxnodelen(genders_t handle) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxnodelen;
}

int 
genders_getmaxattrlen(genders_t handle) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrlen;
}

int 
genders_getmaxvallen(genders_t handle) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxvallen;
}

static int 
_list_create(genders_t handle, char ***list, int len, int buflen) 
{
  if (len > 0) {
    char **temp;
    int i,j;

    if (list == NULL) {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

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

static int 
_list_clear(genders_t handle, char **list, int len, int buflen) 
{
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

static int 
_list_destroy(genders_t handle, char **list, int len) 
{
  if (len > 0) {
    int i;

    if (list == NULL) {
      handle->errnum = GENDERS_ERR_PARAMETERS;
      return -1;
    }

    for (i = 0; i < len; i++)
      free(list[i]);
    free(list);
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int 
genders_nodelist_create(genders_t handle, char ***list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_create(handle, list, handle->numnodes, handle->maxnodelen+1);
}

int 
genders_nodelist_clear(genders_t handle, char **list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_clear(handle, list, handle->numnodes, handle->maxnodelen+1);
}

int 
genders_nodelist_destroy(genders_t handle, char **list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_destroy(handle, list, handle->numnodes);
}

int 
genders_attrlist_create(genders_t handle, char ***list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_create(handle, list, handle->numattrs, handle->maxattrlen+1);
}

int 
genders_attrlist_clear(genders_t handle, char **list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_clear(handle, list, handle->numattrs, handle->maxattrlen+1);
}

int 
genders_attrlist_destroy(genders_t handle, char **list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;
  
  return _list_destroy(handle, list, handle->numattrs);
}

int 
genders_vallist_create(genders_t handle, char ***list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_create(handle, list, handle->numattrs, handle->maxvallen+1);
}

int 
genders_vallist_clear(genders_t handle, char **list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_clear(handle, list, handle->numattrs, handle->maxvallen+1);
}

int 
genders_vallist_destroy(genders_t handle, char **list) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  return _list_destroy(handle, list, handle->numattrs);
}

int 
genders_getnodename(genders_t handle, char *node, int len) 
{
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

static int 
_put_in_list(genders_t handle, char *str, char **list, int index, int len) 
{
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

int 
genders_getnodes(genders_t handle, char *nodes[], int len, 
                 const char *attr, const char *val) 
{
  ListIterator itr;
  genders_node_t n;
  genders_attrval_t av;
  int rv, index = 0;
  int retval = -1;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((nodes == NULL && len > 0) || len < 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (!(itr = list_iterator_create(handle->nodeslist))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  while ((n = list_next(itr))) {
    int save = 0;

    if (attr == NULL)
      save++;
    else {
      if ((rv = _find_attrval(handle, n->attrlist, attr, &av)) < 0)
	goto cleanup;
      if (av != NULL) {
	if (val == NULL || (av->val != NULL && strcmp(av->val, val) == 0))
	  save++;
      }
    }
  
    if (save && _put_in_list(handle, n->name, nodes, index++, len) < 0) {
      list_iterator_destroy(itr);
      return -1;
    }
  }

  retval = index;
 cleanup:
  if (itr)
    list_iterator_destroy(itr);
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int 
genders_getattr(genders_t handle, char *attrs[], char *vals[], 
                int len, const char *node) 
{
  ListIterator itr;
  ListIterator avitr;
  List attrvals;
  genders_attrval_t av;
  genders_node_t n;
  int index = 0;
  int retval = -1;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if ((attrs == NULL && len > 0) || len < 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (!(n = list_find_first(handle->nodeslist, _is_node, (char *)node))) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if (!(itr = list_iterator_create(n->attrlist))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  while ((attrvals = list_next(itr))) {
    if ((avitr = list_iterator_create(attrvals)) == NULL) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      goto cleanup;
    }

    while ((av = list_next(avitr))) {
      if (_put_in_list(handle, av->attr, attrs, index, len) < 0)
	goto cleanup;
      
      if (vals != NULL && av->val != NULL) {
	if (_put_in_list(handle, av->val, vals, index, len) < 0)
	  goto cleanup;
      }
      index++;
    }
    list_iterator_destroy(avitr);
  }
  avitr = NULL;

  retval = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  if (itr)
    list_iterator_destroy(itr);
  if (avitr)
    list_iterator_destroy(avitr);
  return retval;  
}

int 
genders_getattr_all(genders_t handle, char *attrs[], int len) 
{
  ListIterator itr; 
  char *attr;
  int index = 0;
  int retval = -1;
  
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

  if (!(itr = list_iterator_create(handle->attrslist))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }

  while ((attr = list_next(itr))) {
    if (_put_in_list(handle, attr, attrs, index++, len) < 0)
      goto cleanup;
  }

  retval = index;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  if (itr)
    list_iterator_destroy(itr);
  return retval;  
}

int 
genders_testattr(genders_t handle, const char *node, const char *attr, 
                 char *val, int len) 
{
  genders_node_t n;
  genders_attrval_t av;
  int rv, retval = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL || (val != NULL && len <= 0)) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (!(n = list_find_first(handle->nodeslist, _is_node, (char *)node))) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if ((rv = _find_attrval(handle, n->attrlist, attr, &av)) < 0)
    return -1;

  if (av != NULL) {
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

int 
genders_testattrval(genders_t handle, const char *node, 
                    const char *attr, const char *val) 
{
  genders_node_t n;
  genders_attrval_t av;
  int rv, retval = 0;

  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (!(n = list_find_first(handle->nodeslist, _is_node, (char *)node))) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }
  
  if ((rv = _find_attrval(handle, n->attrlist, attr, &av)) < 0)
    return -1;
  
  if (av != NULL) {
    if (val == NULL || ((av->val != NULL) && (strcmp(av->val, val) == 0)))
      retval = 1;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int 
genders_isnode(genders_t handle, const char *node) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (node == NULL)
    node = handle->nodename;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return (list_find_first(handle->nodeslist, _is_node, (char *)node)) ? 1 : 0;
}

int 
genders_isattr(genders_t handle, const char *attr) 
{
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return (list_find_first(handle->attrslist, _is_str, (char *)attr)) ? 1 : 0;
}

int 
genders_isattrval(genders_t handle, const char *attr, const char *val) 
{
  ListIterator itr;
  genders_node_t n;
  genders_attrval_t av;
  int rv, retval = -1;
  
  if (_loaded_handle_error_check(handle) < 0)
    return -1;

  if (attr == NULL || val == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }
  
  if (!(itr = list_iterator_create(handle->nodeslist))) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  while ((n = list_next(itr))) {
    if ((rv = _find_attrval(handle, n->attrlist, attr, &av)) < 0) 
      goto cleanup;
    if (av != NULL) {
      if (av->val != NULL && strcmp(av->val, val) == 0)
        retval = 1;
      goto cleanup;
    }
  }

  retval = 0;
  handle->errnum = GENDERS_ERR_SUCCESS;
 cleanup:
  if (itr)
    list_iterator_destroy(itr);
  return retval;
}

int 
genders_parse(genders_t handle, const char *filename, FILE *stream) 
{
  int line_count = 1;
  int retval = 0;
  int rv, ret, fd = -1;
  char buf[GENDERS_READLINE_BUFLEN];
  List debugnodeslist = NULL;
  List debugattrvalslist = NULL;

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

  if ((debugnodeslist = list_create(_free_genders_node)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    retval = -1;
    goto finish;
  }

  if ((debugattrvalslist = list_create(NULL)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    retval = -1;
    goto finish;
  }

  while ((ret = _readline(handle, fd, buf, GENDERS_READLINE_BUFLEN)) > 0) {
    if ((rv = _parse_line(handle, buf, line_count, stream, 
			  debugnodeslist, debugattrvalslist)) < 0) {
      retval = -1;
      goto finish;
    }

    retval += rv;
    line_count++;
  }

  if (ret < 0 && handle->errnum == GENDERS_ERR_OVERFLOW) {
    fprintf(stderr, "Line %d: exceeds maximum allowed length\n", line_count);
    retval = -1;
    goto finish;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
 finish:
  close(fd);
  if (debugnodeslist)
    list_destroy(debugnodeslist);
  if (debugattrvalslist)
    list_destroy(debugattrvalslist);
  return retval;
}

void 
genders_set_errnum(genders_t handle, int errnum) 
{
  if (_handle_error_check(handle) < 0)
    return;

  if (errnum >= GENDERS_ERR_SUCCESS && errnum <= GENDERS_ERR_ERRNUMRANGE)
    handle->errnum = errnum;
  else
    handle->errnum = GENDERS_ERR_INTERNAL;
}
