/*
 * $Id: genders.c,v 1.55 2003-06-30 15:59:56 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/libgenders/genders.c,v $
 */

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "genders.h"

/******************
 * defines        *
 ******************/

#define GENDERS_ERR_MIN           GENDERS_ERR_SUCCESS
#define GENDERS_ERR_MAX           GENDERS_ERR_ERRNUMRANGE

#define GENDERS_DATA_LOADED       1
#define GENDERS_DATA_NOT_LOADED   0

#define GENDERS_MAGIC_NUM         0xdeadbeef

#define GENDERS_GETLINE_BUFLEN    65536

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

struct genders {
  int magic;                          /* magic number */ 
  int errnum;                         /* error code */
  int loaded_flag;                    /* genders data loaded? */
  int numnodes;                       /* number of nodes in database */
  int numattrs;                       /* number of attrs in database */
  int maxattrs;                       /* max attrs any one node has */
  int maxnodelen;                     /* max node name length */
  int maxattrlen;                     /* max attr name length */
  int maxvallen;                      /* max value name length */
  char nodename[MAXHOSTNAMELEN+1];    /* current hostname */
  struct node_listnode *nodes_head;   /* node names list head */
  struct node_listnode *nodes_tail;   /* node names list tail */
  struct attr_listnode *attrs_head;   /* unique attrs list head */
  struct attr_listnode *attrs_tail;   /* unique attrs list tail */
};

/* node_listnode
 * - stores node name and node attributes
 * - if node has no attributes, attrvals_head = attrvals_tail = NULL
 */
struct node_listnode {
  char name[MAXHOSTNAMELEN+1];
  struct node_listnode *next;
  struct attrval_listnode *attrvals_head;
  struct attrval_listnode *attrvals_tail;
};

/* attrval_listnode
 * - stores an attribute name and its value
 * - if attribute has no value, val = NULL
 */
struct attrval_listnode {
  char *name;
  char *val;
  struct attrval_listnode *next;
};

/* attr_listnode
 * - stores name of a unique attribute
 */
struct attr_listnode {
  char *name;
  struct attr_listnode *next;
};

/* error messages */
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

/*****************************************
 * Static Function Declarations          *
 *****************************************/

static int  _handle_error_check(genders_t);
static int  _unloaded_handle_error_check(genders_t);
static int  _loaded_handle_error_check(genders_t);
static void _initialize_handle(genders_t);
static void _free_handle_lists(genders_t);
static int  _getline(genders_t, int, char **);
static int  _readline(genders_t, int, int, char **);
static int  _insert_node_listnode(genders_t, char *);
static int  _insert_attrval_listnode(genders_t, char *, char *);
static int  _insert_attr_listnode(genders_t, char *);
static int  _parse_line(genders_t, char *, int, FILE *);
static int  _list_create(genders_t, char ***, int, int);
static int  _list_clear(genders_t, char **, int, int);
static int  _list_destroy(genders_t, char **, int);
static int  _save_in_list(genders_t, char *, char **, int, int);

static struct node_listnode *   _have_node(genders_t, const char *);
static struct attrval_listnode *_have_attr(genders_t, const char *, 
                                           struct node_listnode *);

int _handle_error_check(genders_t handle) {

  if (handle == NULL || handle->magic != GENDERS_MAGIC_NUM)
    return -1;

  return 0;
}

int _unloaded_handle_error_check(genders_t handle) {

  if (_handle_error_check(handle) == -1)
    return -1;

  if (handle->loaded_flag == GENDERS_DATA_LOADED) {
    handle->errnum = GENDERS_ERR_ISLOADED;
    return -1;
  }
  
  return 0;
}

int _loaded_handle_error_check(genders_t handle) {

  if (_handle_error_check(handle) == -1)
    return -1;

  if (handle->loaded_flag == GENDERS_DATA_NOT_LOADED) {
    handle->errnum = GENDERS_ERR_NOTLOADED;
    return -1;
  }
  
  return 0;
}

void _initialize_handle(genders_t handle) {
  handle->magic = GENDERS_MAGIC_NUM;
  handle->loaded_flag = GENDERS_DATA_NOT_LOADED;
  handle->numnodes = 0;
  handle->numattrs = 0;
  handle->maxattrs = 0;
  handle->maxnodelen = 0;
  handle->maxattrlen = 0;
  handle->maxvallen = 0;
  memset(handle->nodename,'\0',MAXHOSTNAMELEN+1);
  handle->nodes_head = NULL;
  handle->nodes_tail = NULL;
  handle->attrs_head = NULL;
  handle->attrs_tail = NULL;
}

genders_t genders_handle_create(void) {
  genders_t handle;

  if ((handle = (genders_t)malloc(sizeof(struct genders))) == NULL)
    return NULL;
  
  _initialize_handle(handle);

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle;
}

void _free_handle_lists(genders_t handle) {
  struct node_listnode *nptr = handle->nodes_head;
  struct attr_listnode *aptr = handle->attrs_head;
  
  /* free node list and each node's attributes */
  while (nptr != NULL) {
    struct node_listnode *temp_nptr = nptr->next;
    struct attrval_listnode *avptr = nptr->attrvals_head;
 
    while (avptr != NULL) {
      struct attrval_listnode * temp_avptr = avptr->next;
      free(avptr->name);
      free(avptr->val);
      free(avptr);
      avptr = temp_avptr;
    }
    
    free(nptr);
    nptr = temp_nptr;
  }

  /* free unique attribute list */
  while (aptr != NULL) {
    struct attr_listnode *temp_aptr = aptr->next;
    free(aptr->name);
    free(aptr);
    aptr = temp_aptr;
  }
}

int genders_handle_destroy(genders_t handle) {

  if (_handle_error_check(handle) == -1)
    return -1;

  _free_handle_lists(handle);
  _initialize_handle(handle);

  /* "clean" magic number 
   * ~0xdeadbeef == 0xlivebeef :-)
   */
  handle->magic = ~GENDERS_MAGIC_NUM;

  free(handle);
  return 0;
}

/* create buffer of length buflen and read a line from fd
 * - buffer returned only if read > 0 chars
 * - truncation occurred if read >= buflen
 * Returns numbers of bytes read on success, -1 on error
 */
int _readline(genders_t handle, int fd, int buflen, char **buf) {
  int ret, count = 0;
  char *buffer = NULL;
  char chr;

  if ((buffer = (char *)malloc(buflen)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    goto cleanup;
  }
  memset(buffer, '\0', buflen);
    
  /* read line */
  do {
    if ((ret = read(fd, &chr, 1)) == -1) {
      handle->errnum = GENDERS_ERR_READ;
      goto cleanup;
    }
    
    if (ret == 1)
      buffer[count++] = chr;

  } while (ret == 1 && chr != '\n' && count < buflen);

  if (count == 0)
    free(buffer);
  else
    *buf = buffer;
  
  return count;

 cleanup:

  free(buffer);
  return -1;
}

/* portable version of getline(3) for genders
 * - user is responsible for freeing memory
 * Returns length of line read on success, 0 for EOF, -1 on error 
 */ 
int _getline(genders_t handle, int fd, char **buf) {
  int retval, start_offset;
  int buflen = GENDERS_GETLINE_BUFLEN;
  
  /* get beginning seek position */
  if ((start_offset = lseek(fd, 0, SEEK_CUR)) == (off_t)-1) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    return -1;
  }

  while ((retval = _readline(handle, fd, buflen, buf)) >= buflen) {
    if (lseek(fd, start_offset, SEEK_SET) != start_offset) {
      handle->errnum = GENDERS_ERR_INTERNAL;
      return -1;
    }
    buflen += GENDERS_GETLINE_BUFLEN;
    free(buf);
  }

  return retval;
}

/* insert a node into the node list 
 * Returns -1 on error, 0 on success
 */
int _insert_node_listnode(genders_t handle, char *name) {
  struct node_listnode *nptr;
  int size = sizeof(struct node_listnode);

  /* create new node at the end of the list */
  if ((nptr = (struct node_listnode *)malloc(size)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }
  
  if (handle->nodes_tail == NULL) {
    handle->nodes_head = nptr;
    handle->nodes_tail = handle->nodes_head;
  }
  else {
    (handle->nodes_tail)->next = nptr;
    handle->nodes_tail = (handle->nodes_tail)->next;
  }
  memset(nptr->name, '\0', MAXHOSTNAMELEN+1);

  /* previously asserted that name is < MAXHOSTNAMELEN+1 */
  strcpy(nptr->name, name);
  nptr->next = NULL;
  nptr->attrvals_head = NULL;
  nptr->attrvals_tail = NULL;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

/* insert an attribute (and value if value != NULL) into an attrval list 
 * Returns -1 on error, 0 on success
 */
int _insert_attrval_listnode(genders_t handle, char *attr, char *val) {
  struct attrval_listnode *avptr;
  struct node_listnode *nptr = handle->nodes_tail;
  int size = sizeof(struct attrval_listnode);

  /* create new node at the end of the list */
  if ((avptr = (struct attrval_listnode *)malloc(size)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  if (nptr->attrvals_tail == NULL) {
    nptr->attrvals_head = avptr;
    nptr->attrvals_tail = nptr->attrvals_head;
  }
  else {
    (nptr->attrvals_tail)->next = avptr;
    nptr->attrvals_tail = (nptr->attrvals_tail)->next;
  }

  /* store attribute */
  if ((avptr->name = strdup(attr)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  /* store value? */
  if (val != NULL) {
    if ((avptr->val = strdup(val)) == NULL) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      return -1;
    }
  }
  else 
    avptr->val = NULL; 

  avptr->next = NULL;
 
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

/* place unique attribute names into the unique attribute list 
 * Returns -1 on error, 1 attr inserted, 0 attr not inserted
 */
int _insert_attr_listnode(genders_t handle, char *attr) {
  struct attr_listnode *aptr = handle->attrs_head;
  int size = sizeof(struct attr_listnode);

  /* is attribute already in the list? */
  while (aptr != NULL && strcmp(aptr->name, attr) != 0)
    aptr = aptr->next;

  if (aptr != NULL)
    return 0;

  /* create new node at the end of the list */
  if ((aptr = (struct attr_listnode *)malloc(size)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  if (handle->attrs_tail == NULL) {
    handle->attrs_head = aptr;
    handle->attrs_tail = handle->attrs_head;
  }
  else {
    (handle->attrs_tail)->next = aptr;
    handle->attrs_tail = (handle->attrs_tail)->next;
  }

  /* store attribute */
  if ((aptr->name = strdup(attr)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }
  aptr->next = NULL;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 1;
}

/* parse a genders file line
 * - If line_num == 0, parse and store genders data
 * - Returns -1 on error, 0 on success
 * - If line_num > 0, debug genders file, do not store genders data
 * - Returns -1 on error, number of parse errors on success (0 if no errors)
 */
int _parse_line(genders_t handle, char *line, int line_num, FILE *stream) {
  char *linebuf, *temp;
  char *line_token = NULL;
  int ret, attrcount = 0;

  /* "remove" comments */
  if ((temp = strchr(line, '#')) != NULL) 
    *temp = '\0';

  /* move forward to node name */
  while(isspace(*line))  
    line++;

  if (*line == '\0')
    return 0;

  /* get node name */
  if ((line_token = strsep(&line, " \t\n\0")) != NULL) { 

    if (strlen(line_token) > MAXHOSTNAMELEN) {
      if (line_num > 0) {
        fprintf(stream, "Line %d: hostname too long\n", line_num);
        return 1;
      }
      handle->errnum = GENDERS_ERR_PARSE;
      return -1;
    }
      
    if (line_num == 0) {
      if (_insert_node_listnode(handle,line_token) == -1)
        return -1;
      
      handle->numnodes++;
      
      if (strlen(line_token) > handle->maxnodelen)
        handle->maxnodelen = strlen(line_token);
    }
      
    /* move forward to attributes */
    while(isspace(*line))  
      line++;
  }

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
  line_token = strtok_r(line,",\n\0",&linebuf);
  while (line_token != NULL) {
    char *attrbuf = NULL;
    char *attr = NULL;
    char *val = NULL;

    /* parse value out of attribute */
    if (strchr(line_token,'=') == NULL) {
      attr = line_token;
      val = NULL;
    }
    else {
      attr = strtok_r(line_token,"=",&attrbuf);
      val = strtok_r(NULL,"\0",&attrbuf);
    }

    if (_insert_attrval_listnode(handle, attr, val) == -1)
      return -1;
    
    if (strlen(attr) > handle->maxattrlen)
      handle->maxattrlen = strlen(attr);
      
    if ((val != NULL) && (strlen(val) > handle->maxvallen))
      handle->maxvallen = strlen(val);

    if ((ret = _insert_attr_listnode(handle, attr)) == -1)
      return -1;
      
    handle->numattrs += ret;
      
    attrcount++;

    line_token = strtok_r(NULL,",\n\0",&linebuf);
  }

  if (attrcount > handle->maxattrs)
    handle->maxattrs = attrcount;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int genders_load_data(genders_t handle, const char *filename) {
  char *temp, *line = NULL;
  int ret, fd = -1;

  if (_unloaded_handle_error_check(handle) == -1)
    return -1;

  if (filename == NULL)
    filename = GENDERS_DEFAULT_FILE;

  if ((fd = open(filename, O_RDONLY)) == -1) {
    handle->errnum = GENDERS_ERR_OPEN;
    goto cleanup;
  }

  /* parse genders file line by line */
  while ((ret = _getline(handle, fd, &line)) > 0) {
    if (_parse_line(handle, line, 0, NULL) == -1)
      goto cleanup;
    free(line);
  }
  line = NULL;

  if (ret == -1) 
    goto cleanup;

  if (gethostname(handle->nodename, MAXHOSTNAMELEN+1) == -1) {
    handle->errnum = GENDERS_ERR_INTERNAL;
    goto cleanup;
  }

  /* NUL-terminate nodename in case gethostname truncated */
  handle->nodename[MAXHOSTNAMELEN]='\0';

  /* shorten hostname if necessary */
  if ((temp = strchr(handle->nodename,'.')) != NULL)
    *temp = '\0';
  
  if (strlen(handle->nodename) > handle->maxnodelen) 
    handle->maxnodelen = strlen(handle->nodename);

  close(fd);

  /* data completely loaded, indicate in handle */
  handle->loaded_flag = GENDERS_DATA_LOADED;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;

cleanup:

  close(fd);
  free(line);
 
  _free_handle_lists(handle);
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
  if (errnum >= GENDERS_ERR_MIN && errnum <= GENDERS_ERR_MAX)
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

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numnodes;
}

int genders_getnumattrs(genders_t handle) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numattrs;
}

int genders_getmaxattrs(genders_t handle) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrs;
}

int genders_getmaxnodelen(genders_t handle) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxnodelen;
}

int genders_getmaxattrlen(genders_t handle) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrlen;
}

int genders_getmaxvallen(genders_t handle) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxvallen;
}

/* creates lists 
 * Returns -1 on error, 'entries' on success
 */
int _list_create(genders_t handle, char ***list, int len, int entry_len) {
  char **temp;
  int i,j;

  if (list == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (len > 0) {
    if ((temp = (char **)malloc(sizeof(char *) * len)) == NULL) { 
      handle->errnum = GENDERS_ERR_OUTMEM;
      return -1;
    }

    for (i = 0; i < len; i++) {
      if ((temp[i] = (char *)malloc(entry_len)) == NULL) {
        
        for (j = 0; j < i; j++)
          free(temp[j]);
        free(temp);

        handle->errnum = GENDERS_ERR_OUTMEM;
        return -1;
      }
      memset(temp[i], '\0', entry_len);
    }

    *list = temp;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return len;
}

/* clears lists 
 * Returns -1 on error, 0 on success
 */
int _list_clear(genders_t handle, char **list, int len, int entry_len) {
  int i;

  if (list == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  for (i = 0; i < len; i++) {
    if (list[i] == NULL) {
      handle->errnum = GENDERS_ERR_NULLPTR;
      return -1;
    }
    memset(list[i], '\0', entry_len);
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

/* destroys lists 
 * Returns -1 on error, 0 on success
 */
int _list_destroy(genders_t handle, char **list, int len) {
  int i;

  if (list == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (len > 0) {
    for (i = 0; i < len; i++)
      free(list[i]);
    free(list);
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int genders_nodelist_create(genders_t handle, char ***nodelist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_create(handle, nodelist, handle->numnodes, handle->maxnodelen+1);
}

int genders_nodelist_clear(genders_t handle, char **nodelist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_clear(handle, nodelist, handle->numnodes, handle->maxnodelen+1);
}

int genders_nodelist_destroy(genders_t handle, char **nodelist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_destroy(handle, nodelist, handle->numnodes);
}

int genders_attrlist_create(genders_t handle, char ***attrlist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_create(handle, attrlist, handle->numattrs, handle->maxattrlen+1);
}

int genders_attrlist_clear(genders_t handle, char **attrlist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_clear(handle, attrlist, handle->numattrs, handle->maxattrlen+1);
}

int genders_attrlist_destroy(genders_t handle, char **attrlist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;
  
  return _list_destroy(handle, attrlist, handle->numattrs);
}

int genders_vallist_create(genders_t handle, char ***vallist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_create(handle, vallist, handle->numattrs, handle->maxvallen+1);
}

int genders_vallist_clear(genders_t handle, char **vallist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_clear(handle, vallist, handle->numattrs, handle->maxvallen+1);
}

int genders_vallist_destroy(genders_t handle, char **vallist) {

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  return _list_destroy(handle, vallist, handle->numattrs);
}

int genders_getnodename(genders_t handle, char *node, int len) {
  
  if (_loaded_handle_error_check(handle) == -1)
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

int genders_getnodes(genders_t handle, 
                     char *nodes[], 
                     int len, 
                     const char *attr, 
                     const char *val) {
  struct node_listnode *nptr;
  int count = 0;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (nodes == NULL || len <= 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  /* search nodes */
  nptr = handle->nodes_head;
  while (nptr != NULL) {
    int save = 0;

    /* if attr is NULL, or if node has attribute=value, save node */
    if (attr == NULL)
      save++;
    else {
      struct attrval_listnode *avptr = _have_attr(handle, attr, nptr);
      save = (avptr) && ((val == NULL) || 
                         (avptr->val != NULL && strcmp(avptr->val, val) == 0));
    }
  
    if (save)
      if (_save_in_list(handle, nptr->name, nodes, count++, len) == -1)
        return -1;

    nptr = nptr->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return count;
}

int genders_getattr(genders_t handle, 
                    char *attrs[], 
                    char *vals[], 
                    int len, 
                    const char *node) {
  struct node_listnode *nptr;
  struct attrval_listnode *avptr;
  int count = 0;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (attrs == NULL || len <= 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if ((nptr = _have_node(handle, node)) == NULL) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  /* store attributes */
  avptr = nptr->attrvals_head;
  while (avptr != NULL) {
    if (_save_in_list(handle, avptr->name, attrs, count, len) == -1)
      return -1;
    
    if (vals != NULL && avptr->val != NULL) {
      if (_save_in_list(handle, avptr->val, vals, count, len) == -1)
        return -1;
    }
    count++;

    avptr = avptr->next;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return count;  
}

int genders_getattr_all(genders_t handle, char *attrs[], int len) {
  struct attr_listnode *aptr;
  int count = 0;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (attrs == NULL || len <= 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (handle->numattrs > len) {
    handle->errnum = GENDERS_ERR_OVERFLOW;
    return -1;
  }

  /* store attributes */
  aptr = handle->attrs_head;
  while (aptr != NULL) {
    if (_save_in_list(handle, aptr->name, attrs, count++, len) == -1)
      return -1;

    aptr = aptr->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return count;  
}

/* save a string into a char ** list
 * Returns -1 on error, 0 on success
 */
int _save_in_list(genders_t handle, char *str, char **list, int count, int len) {

  if (count >= len) {
    handle->errnum = GENDERS_ERR_OVERFLOW;
    return -1;
  }

  if (list[count] == NULL) {
    handle->errnum = GENDERS_ERR_NULLPTR;
    return -1;
  }
  
  strcpy(list[count], str); 

  return 0;
}

int genders_testattr(genders_t handle, 
                     const char *node, 
                     const char *attr, 
                     char *val, 
                     int len) {
  struct node_listnode *nptr;
  struct attrval_listnode *avptr;
  int retval = 0;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (attr == NULL || (val != NULL && len <= 0)) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if ((nptr = _have_node(handle, node)) == NULL) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if ((avptr = _have_attr(handle, attr, nptr)) != NULL) {
    if (val != NULL && avptr->val != NULL) {
      if ((strlen(avptr->val) + 1) > len) {
        handle->errnum = GENDERS_ERR_OVERFLOW;
        return -1;
      }
      strcpy(val, avptr->val);
    }
    retval = 1;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_testattrval(genders_t handle, 
                        const char *node, 
                        const char *attr, 
                        const char *val) {
  struct node_listnode *nptr;
  struct attrval_listnode *avptr;
  int retval = 0;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if ((nptr = _have_node(handle, node)) == NULL) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if ((avptr = _have_attr(handle, attr, nptr)) != NULL) {
    if ((val == NULL) || 
        ((avptr->val != NULL) && (strcmp(avptr->val, val) == 0)))
      retval = 1;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_isnode(genders_t handle, const char *node) {
  struct node_listnode *nptr;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (node == NULL)
    node = handle->nodename;

  nptr = _have_node(handle, node);

  handle->errnum = GENDERS_ERR_SUCCESS;
  return (nptr != NULL) ? 1 : 0;
}

int genders_isattr(genders_t handle, const char *attr) {
  struct attr_listnode *aptr;
  int retval = 0;

  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  aptr = handle->attrs_head;
  while (aptr != NULL && strcmp(aptr->name,attr) != 0)
    aptr = aptr->next;

  if (aptr != NULL)
    retval = 1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_isattrval(genders_t handle, const char *attr, const char *val) {
  struct node_listnode *nptr;
  struct attrval_listnode *avptr;
  int retval = 0;
  
  if (_loaded_handle_error_check(handle) == -1)
    return -1;

  if (attr == NULL || val == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }
  
  nptr = handle->nodes_head;
  while (nptr != NULL) {
    if ((avptr = _have_attr(handle, attr, nptr)) != NULL) {
      if (avptr->val != NULL && strcmp(avptr->val, val) == 0)
        retval = 1;
    }
   
    nptr = nptr->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

/* determine if node is stored in the handle's nodelist
 * Returns pointer to node if it is stored, NULL if it is not  
 */        
struct node_listnode *_have_node(genders_t handle, const char *node) {
  struct node_listnode *nptr = handle->nodes_head;

  while (nptr != NULL && strcmp(nptr->name,node) != 0)
    nptr = nptr->next;

  return nptr;
}

/* determines if attr is an attribute in a node_listnode
 * Returns pointer to attrval if it is stored, NULL if it is not  
 */        
struct attrval_listnode *_have_attr(genders_t handle, 
                                    const char *attr, 
                                    struct node_listnode *nptr) {
  struct attrval_listnode *avptr = nptr->attrvals_head;

  while (avptr != NULL && strcmp(avptr->name,attr) != 0)
    avptr = avptr->next;
  
  return avptr;
}

int genders_parse(genders_t handle, const char *filename, FILE *stream) {
  int line_count = 1;
  int retval = 0;
  int fd = -1;
  int ret;
  char *line = NULL;

  if (_handle_error_check(handle) == -1)
    return -1;

  if (filename == NULL)
    filename = GENDERS_DEFAULT_FILE;
  
  if (stream == NULL)
    stream = stderr;

  if ((fd = open(filename,O_RDONLY)) == -1) {
    handle->errnum = GENDERS_ERR_OPEN;
    goto cleanup;
  }

  while (_getline(handle, fd, &line) != 0) {
    if ((ret = _parse_line(handle, line, line_count, stream)) == -1)
      goto cleanup;

    retval += ret;
    free(line);
    line_count++;
  }
  line = NULL;

  close(fd);
  return retval;

 cleanup:

  free(line);
  close(fd);

  return -1;
}

void genders_set_errnum(genders_t handle, int errnum) {

  if (_handle_error_check(handle) == -1)
    return;

  if (errnum >= GENDERS_ERR_MIN && errnum <= GENDERS_ERR_MAX) 
    handle->errnum = errnum;
  else
    handle->errnum = GENDERS_ERR_INTERNAL;
}
