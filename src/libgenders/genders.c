/*
 * $Id: genders.c,v 1.49 2003-05-29 16:22:40 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/libgenders/genders.c,v $
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif                         
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

/* genders handle
 * magic - magic number, ensure proper pointer passed in
 * errnum - error number returned to user
 * loaded_flag - indicates if genders data has been loaded
 * numnodes - number of nodes in the genders file
 * numattrs - number of attributes in the genders file
 * maxattrs - maximum number of attributes for one node
 * maxnodelen - maximum length of a node name
 * maxattrlen - maximum length of an attribute name
 * maxvallen - maximum length of a value
 * nodename - name of the current node
 * nodes_head - head of list that stores node names
 * nodes_tail - tail of list that stores node names
 * attrs_head - head of list that stores all unique attributes
 * attrs_tail - tail of list that stores all unique attributes
 */

struct genders {
  int magic;
  int errnum;
  int loaded_flag;
  int numnodes;
  int numattrs;
  int maxattrs;
  int maxnodelen;
  int maxattrlen;
  int maxvallen;
  char nodename[MAXHOSTNAMELEN+1];
  struct node_listnode *nodes_head;
  struct node_listnode *nodes_tail;
  struct attr_listnode *attrs_head;
  struct attr_listnode *attrs_tail;
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
 * Internal Function Declarations        *
 *****************************************/

/* common error checks for genders handle 
 * Returns -1 on error, 0 on success
 */
static int handle_err_check(genders_t handle);

/* common error checks for non-loaded genders data 
 * Returns -1 on error, 0 on success
 */
static int unloaded_handle_err_check(genders_t handle);

/* common error checks for loaded genders data 
 * Returns -1 on error, 0 on success
 */
static int loaded_handle_err_check(genders_t handle);

/* initialize handle variables 
 */
static void initialize_handle(genders_t handle);

/* portable version of getline(3) for genders
 * - user is responsible for freeing memory
 * - Returns length of line read on success, 0 for EOF, -1 on error 
 */ 
static int _getline(genders_t handle, int fd, char **buf);

/* create buffer of length buflen and read a line from fd
 * - buffer returned only if read > 0 chars
 * - truncation occurred if returns >= buflen
 * Returns numbers of bytes read on success, -1 on error
 */
static int _readline(genders_t handle, int fd, int buflen, char **buf);

/* parse a line from the genders file 
 * If line_num == 0, parse and store genders data
 * - Returns -1 on error, 0 on success
 * If line_num > 0, debug genders file, do not store genders data
 * - Returns -1 on error, number of parse errors on success
 */
static int parse_line(genders_t handle, char *line, int line_num, FILE *stream);

/* insert a node into the node list 
 * Returns -1 on error, 0 on success
 */
static int insert_node_listnode(genders_t handle, char *name);

/* insert an attribute (and value if value != NULL) into an attr-val list 
 * Returns -1 on error, 0 on success
 */
static int insert_attrval_listnode(genders_t handle, char *attr, char *val);

/* place unique attribute names into the unique attribute list 
 * Returns -1 on error, 1 attr inserted, 0 attr not inserted
 */
static int insert_attr_listnode(genders_t handle, char *attr);

/* free all members of the node, attribute-value, and and attribute lists 
 * Returns -1 on error, 0 on success
 */
static void free_handle_cache(genders_t handle);

/* creates lists 
 * Returns -1 on error, 'entries' on success
 */
static int list_create(genders_t handle, char ***list, int len, int entry_len);

/* clears lists 
 * Returns -1 on error, 0 on success
 */
static int list_clear(genders_t handle, char **list, int len, int entry_len);

/* destroys lists 
 * Returns -1 on error, 0 on success
 */
static int list_destroy(genders_t handle, char **list, int len);

/* copy a string into a char ** list
 * Returns -1 on error, 0 on success
 */
static int save_in_list(genders_t handle, 
                        char *str, 
                        char **list, 
                        int count, 
                        int len);

/* determines if node is stored in the handle's nodelist
 * Returns 1 if it is stored, 0 if it is not  
 * Pointer to node is stored in node_ptr
 */        
static int has_node(genders_t handle, 
                    const char *node, 
                    struct node_listnode **node_ptr);

/* determines if attr is an attribute in the node pointed to by node_ptr
 * Returns 1 if it is stored, 0 if it is not  
 * Pointer to attribute is stored in attrval_ptr
 */        
static int has_attr(genders_t handle, 
                    const char *attr, 
                    struct node_listnode *node_ptr, 
                    struct attrval_listnode **attrval_ptr);

int handle_err_check(genders_t handle) {
  if (handle == NULL || handle->magic != GENDERS_MAGIC_NUM)
    return -1;

  return 0;
}

int unloaded_handle_err_check(genders_t handle) {
  
  if (handle_err_check(handle) == -1)
    return -1;

  if (handle->loaded_flag == GENDERS_DATA_LOADED) {
    handle->errnum = GENDERS_ERR_ISLOADED;
    return -1;
  }
  
  return 0;
}

int loaded_handle_err_check(genders_t handle) {
 
  if (handle_err_check(handle) == -1)
    return -1;

  if (handle->loaded_flag == GENDERS_DATA_NOT_LOADED) {
    handle->errnum = GENDERS_ERR_NOTLOADED;
    return -1;
  }
  
  return 0;
}

genders_t genders_handle_create(void) {
  genders_t handle;

  if ((handle = (genders_t)malloc(sizeof(struct genders))) == NULL)
    return NULL;
  
  initialize_handle(handle);

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle;
}

void initialize_handle(genders_t handle) {
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

int genders_handle_destroy(genders_t handle) {

  if (handle_err_check(handle) == -1)
    return -1;

  free_handle_cache(handle);

  initialize_handle(handle);

  /* "clean" magic number 
   * ~0xdeadbeef == 0xlivebeef :-)
   */
  handle->magic = ~GENDERS_MAGIC_NUM;

  free(handle);
  return 0;
}

int genders_load_data(genders_t handle, const char *filename) {
  char *temp, *line = NULL;
  int ret, fd = -1;

  if (unloaded_handle_err_check(handle) == -1)
    return -1;

  if (filename == NULL)
    filename = GENDERS_DEFAULT_FILE;

  if ((fd = open(filename, O_RDONLY)) == -1) {
    handle->errnum = GENDERS_ERR_OPEN;
    goto cleanup;
  }

  /* parse genders file line by line */
  while ((ret = _getline(handle, fd, &line)) > 0) {
    if (parse_line(handle, line, 0, NULL) == -1)
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
 
  free_handle_cache(handle);
  initialize_handle(handle);

  return -1;
}

int _getline(genders_t handle, int fd, char **buf) {
  int retval, start_offset = 0;
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
    ret = read(fd, &chr, 1);
    if (ret == -1) {
      handle->errnum = GENDERS_ERR_READ;
      goto cleanup;
    }
    else if (ret == 1)
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

int parse_line(genders_t handle, char *line, int line_num, FILE *stream) {
  char *linebuf, *attrbuf, *temp;
  char *line_token = NULL;
  char *attr = NULL;
  char *val = NULL;
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
      if (insert_node_listnode(handle,line_token) == -1)
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

    /* parse value out of attribute */
    if (strchr(line_token,'=') == NULL) {
      attr = line_token;
      val = NULL;
    }
    else {
      attr = strtok_r(line_token,"=",&attrbuf);
      val = strtok_r(NULL,"\0",&attrbuf);
    }

    if (insert_attrval_listnode(handle, attr, val) == -1)
      return -1;
    
    if (strlen(attr) > handle->maxattrlen)
      handle->maxattrlen = strlen(attr);
      
    if ((val != NULL) && (strlen(val) > handle->maxvallen))
      handle->maxvallen = strlen(val);

    if ((ret = insert_attr_listnode(handle, attr)) == -1)
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

int insert_node_listnode(genders_t handle, char *name) {
  struct node_listnode *nlist;
  int size = sizeof(struct node_listnode);

  /* create new node at the end of the list */
  if ((nlist = (struct node_listnode *)malloc(size)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }
  
  if (handle->nodes_tail == NULL) {
    handle->nodes_head = nlist;
    handle->nodes_tail = handle->nodes_head;
  }
  else {
    (handle->nodes_tail)->next = nlist;
    handle->nodes_tail = (handle->nodes_tail)->next;
  }
  memset(nlist->name, '\0', MAXHOSTNAMELEN+1);

  /* previously asserted that name is < MAXHOSTNAMELEN+1 */
  strcpy(nlist->name, name);
  nlist->next = NULL;
  nlist->attrvals_head = NULL;
  nlist->attrvals_tail = NULL;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int insert_attrval_listnode(genders_t handle, char *attr, char *val) {
  struct node_listnode *nlist;
  struct attrval_listnode *avlist;
  int size = sizeof(struct attrval_listnode);

  /* create new node at the end of the list */
  if ((avlist = (struct attrval_listnode *)malloc(size)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  nlist = handle->nodes_tail;
  if (nlist->attrvals_tail == NULL) {
    nlist->attrvals_head = avlist;
    nlist->attrvals_tail = nlist->attrvals_head;
  }
  else {
    (nlist->attrvals_tail)->next = avlist;
    nlist->attrvals_tail = (nlist->attrvals_tail)->next;
  }

  /* store attribute */
  if ((avlist->name = (char *)malloc(strlen(attr)+1)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }
  strcpy(avlist->name,attr);

  /* store value */
  if (val != NULL) {
    if ((avlist->val = (char *)malloc(strlen(val)+1)) == NULL) {
      handle->errnum = GENDERS_ERR_OUTMEM;
      return -1;
    }
    strcpy(avlist->val,val);
  }
  else 
    avlist->val = NULL; 

  avlist->next = NULL;
 
  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int insert_attr_listnode(genders_t handle, char *attr) {
  struct attr_listnode *alist;
  int size = sizeof(struct attr_listnode);

  /* is attribute already in the list? */
  alist = handle->attrs_head;
  while (alist != NULL && strcmp(alist->name, attr) != 0)
    alist = alist->next;

  if (alist != NULL)
    return 0;

  /* create new node at the end of the list */
  if ((alist = (struct attr_listnode *)malloc(size)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }

  if (handle->attrs_tail == NULL) {
    handle->attrs_head = alist;
    handle->attrs_tail = handle->attrs_head;
  }
  else {
    (handle->attrs_tail)->next = alist;
    handle->attrs_tail = (handle->attrs_tail)->next;
  }

  /* store attribute */
  if ((alist->name = (char *)malloc(strlen(attr) + 1)) == NULL) {
    handle->errnum = GENDERS_ERR_OUTMEM;
    return -1;
  }
  strcpy(alist->name, attr);
  alist->next = NULL;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 1;
}

void free_handle_cache(genders_t handle) {
  struct node_listnode *nlist;
  struct attr_listnode *alist;
  
  /* free node list and each node's attributes */
  nlist = handle->nodes_head;
  while (nlist != NULL) {
    struct node_listnode *temp_nlist = nlist->next;
    struct attrval_listnode *avlist = nlist->attrvals_head;
 
    while (avlist != NULL) {
      struct attrval_listnode * temp_avlist = avlist->next;
      free(avlist->name);
      free(avlist->val);
      free(avlist);
      avlist = temp_avlist;
    }
    
    free(nlist);
    nlist = temp_nlist;
  }

  /* free unique attribute list */
  alist = handle->attrs_head;
  while (alist != NULL) {
    struct attr_listnode *temp_alist = alist->next;
    free(alist->name);
    free(alist);
    alist = temp_alist;
  }
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

int genders_handle_dump(genders_t handle, FILE *stream) {
  struct node_listnode *nlist;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (stream == NULL)
    stream = stdout;

  nlist = handle->nodes_head;
  while (nlist != NULL) {
    struct attrval_listnode *avlist = nlist->attrvals_head;

    fprintf(stream,"%s\t", nlist->name);
    while (avlist != NULL) {
      if (avlist->val != NULL)
        fprintf(stream,"%s=%s",avlist->name,avlist->val);
      else 
        fprintf(stream,"%s", avlist->name);

      if (avlist->next != NULL)
        fprintf(stream,",");

      avlist = avlist->next;
    }

    fprintf(stream,"\n");
    nlist = nlist->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return 0;
}

int genders_getnumnodes(genders_t handle) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numnodes;
}

int genders_getnumattrs(genders_t handle) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->numattrs;
}

int genders_getmaxattrs(genders_t handle) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrs;
}

int genders_getmaxnodelen(genders_t handle) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxnodelen;
}

int genders_getmaxattrlen(genders_t handle) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxattrlen;
}

int genders_getmaxvallen(genders_t handle) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return handle->maxvallen;
}

int genders_nodelist_create(genders_t handle, char ***nodelist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_create(handle, nodelist, handle->numnodes, handle->maxnodelen+1);
}

int genders_nodelist_clear(genders_t handle, char **nodelist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_clear(handle, nodelist, handle->numnodes, handle->maxnodelen+1);
}

int genders_nodelist_destroy(genders_t handle, char **nodelist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_destroy(handle, nodelist, handle->numnodes);
}

int genders_attrlist_create(genders_t handle, char ***attrlist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_create(handle, attrlist, handle->numattrs, handle->maxattrlen+1);
}

int genders_attrlist_clear(genders_t handle, char **attrlist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_clear(handle, attrlist, handle->numattrs, handle->maxattrlen+1);
}

int genders_attrlist_destroy(genders_t handle, char **attrlist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;
  
  return list_destroy(handle, attrlist, handle->numattrs);
}

int genders_vallist_create(genders_t handle, char ***vallist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_create(handle, vallist, handle->numattrs, handle->maxvallen+1);
}

int genders_vallist_clear(genders_t handle, char **vallist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_clear(handle, vallist, handle->numattrs, handle->maxvallen+1);
}

int genders_vallist_destroy(genders_t handle, char **vallist) {

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  return list_destroy(handle, vallist, handle->numattrs);
}

int list_create(genders_t handle, char ***list, int len, int entry_len) {
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

int list_clear(genders_t handle, char **list, int len, int entry_len) {
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

int list_destroy(genders_t handle, char **list, int len) {
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

int genders_getnodename(genders_t handle, char *node, int len) {
  
  if (loaded_handle_err_check(handle) == -1)
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
  struct node_listnode *nlist;
  struct attrval_listnode *avlist;
  int count = 0;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (nodes == NULL || len <= 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  /* search nodes */
  nlist = handle->nodes_head;
  while (nlist != NULL) {

    /* if attr is NULL, or if node has attribute=value, save node */
    if (attr == NULL ||
        (has_attr(handle, attr, nlist, &avlist) == 1 &&
         (val == NULL || (avlist->val != NULL && 
                          strcmp(avlist->val, val) == 0)))) {
      if (save_in_list(handle, nlist->name, nodes, count++, len) == -1)
        return -1;
    }

    nlist = nlist->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return count;
}

int genders_getattr(genders_t handle, 
                    char *attrs[], 
                    char *vals[], 
                    int len, 
                    const char *node) {
  struct node_listnode *nlist;
  struct attrval_listnode *avlist;
  int count = 0;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (attrs == NULL || len <= 0) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (has_node(handle, node, &nlist) == 0) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  /* store attributes */
  avlist = nlist->attrvals_head;
  while (avlist != NULL) {
    if (save_in_list(handle, avlist->name, attrs, count, len) == -1)
      return -1;
    
    if (vals != NULL && avlist->val != NULL) {
      if (save_in_list(handle, avlist->val, vals, count, len) == -1)
        return -1;
    }
    count++;

    avlist = avlist->next;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return count;  
}

int genders_getattr_all(genders_t handle, char *attrs[], int len) {
  struct attr_listnode *alist;
  int count = 0;

  if (loaded_handle_err_check(handle) == -1)
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
  alist = handle->attrs_head;
  while (alist != NULL) {
    if (save_in_list(handle, alist->name, attrs, count++, len) == -1)
      return -1;

    alist = alist->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return count;  
}

int save_in_list(genders_t handle, char *str, char **list, int count, int len) {
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
  struct node_listnode *nlist;
  struct attrval_listnode *avlist;
  int retval = 0;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (attr == NULL || (val != NULL && len <= 0)) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (has_node(handle, node, &nlist) == 0) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if (has_attr(handle, attr, nlist, &avlist) == 1) {
    if (val != NULL && avlist->val != NULL) {
      if ((strlen(avlist->val) + 1) > len) {
        handle->errnum = GENDERS_ERR_OVERFLOW;
        return -1;
      }
      strcpy(val, avlist->val);
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
  struct node_listnode *nlist;
  struct attrval_listnode *avlist;
  int retval = 0;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  if (node == NULL)
    node = handle->nodename;

  if (has_node(handle, node, &nlist) == 0) {
    handle->errnum = GENDERS_ERR_NOTFOUND;
    return -1;
  }

  if (has_attr(handle, attr, nlist, &avlist) == 1) {
    if ((val == NULL) || 
        ((avlist->val != NULL) && (strcmp(avlist->val, val) == 0)))
      retval = 1;
  }
  
  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_isnode(genders_t handle, const char *node) {
  int retval;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (node == NULL)
    node = handle->nodename;

  retval = has_node(handle, node, NULL);

  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_isattr(genders_t handle, const char *attr) {
  struct attr_listnode *alist;
  int retval = 0;

  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (attr == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }

  alist = handle->attrs_head;
  while (alist != NULL && strcmp(alist->name,attr) != 0)
    alist = alist->next;

  if (alist != NULL)
    retval = 1;

  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int genders_isattrval(genders_t handle, const char *attr, const char *val) {
  struct node_listnode *nlist;
  struct attrval_listnode *avlist;
  int retval = 0;
  
  if (loaded_handle_err_check(handle) == -1)
    return -1;

  if (attr == NULL || val == NULL) {
    handle->errnum = GENDERS_ERR_PARAMETERS;
    return -1;
  }
  
  nlist = handle->nodes_head;
  while (nlist != NULL) {
    if (has_attr(handle, attr, nlist, &avlist) == 1) {
      if (avlist->val != NULL && strcmp(avlist->val, val) == 0)
        retval = 1;
    }
   
    nlist = nlist->next;
  }

  handle->errnum = GENDERS_ERR_SUCCESS;
  return retval;
}

int has_node(genders_t handle, 
             const char *node, 
             struct node_listnode **node_ptr) {
  struct node_listnode *nlist;

  nlist = handle->nodes_head;
  while (nlist != NULL && strcmp(nlist->name,node) != 0)
    nlist = nlist->next;

  if (node_ptr != NULL)
    *node_ptr = nlist;

  if (nlist != NULL)
    return 1;

  return 0;
}

int has_attr(genders_t handle, 
             const char *attr, 
             struct node_listnode *node_ptr,
             struct attrval_listnode **attrval_ptr) {
  struct attrval_listnode *avlist;

  avlist = node_ptr->attrvals_head;
  while (avlist != NULL && strcmp(avlist->name,attr) != 0)
    avlist = avlist->next;
  
  if (avlist != NULL) {
    if (attrval_ptr != NULL)
      *attrval_ptr = avlist;
    return 1;
  }

  return 0;
}

int genders_parse(genders_t handle, const char *filename, FILE *stream) {
  int line_count = 1;
  int retval = 0;
  int fd = -1;
  int ret;
  char *line = NULL;

  if (handle_err_check(handle) == -1)
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
    if ((ret = parse_line(handle, line, line_count, stream)) == -1)
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

  if (handle_err_check(handle) == -1)
    return;

  if (errnum >= GENDERS_ERR_MIN && errnum <= GENDERS_ERR_MAX) 
    handle->errnum = errnum;
  else
    handle->errnum = GENDERS_ERR_INTERNAL;
}
