/*
 * $Id: libgenders_test.c,v 1.2 2003-03-13 22:04:38 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/testsuite/libgenders_test.c,v $
 */

#include "genders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgenders_testcases.h"

struct test_env {
  genders_t open_handle;
  genders_t not_open_handle;
  char **nodelist;
  char **attrlist;
  char **vallist;
  int nodelist_len;
  int attrlist_len;
  int vallist_len;
  int maxnodelen;
  int maxattrlen;
  int maxvallen;
};

/* create an array of strings by mallocing a 2D array of chars */ 
int create_array_of_strings(char ***list, int list_len, int string_len) {
  int i;

  if ((*list = (char **)malloc(sizeof(char *)*list_len)) == NULL) {
    printf("malloc() error\n");
    return -1;
  }
  
  for (i = 0; i < list_len; i++) {
    if (((*list)[i] = (char *)malloc(string_len + 1)) == NULL) {
      printf("malloc() error\n");
      return -1;
    }
    memset((*list)[i], '\0', string_len + 1);
  }

  return 0;
}

/* free a previously allocated array of strings */
void free_array_of_strings(char **list, int list_len) {
  int i;
  for (i = 0; i < list_len; i++) {
    free(list[i]);
  }
  free(list);
  return;
}

/* initialize the test environment */
int initialize_test_env(struct test_env *test_env) {
  test_env->open_handle = NULL;
  test_env->not_open_handle = NULL;
  test_env->nodelist = NULL;
  test_env->attrlist = NULL;
  test_env->vallist = NULL;

  if ((test_env->open_handle = genders_handle_create()) == NULL) {
    printf("genders_handle_create() error\n");
    return -1;
  }

  if (genders_open(test_env->open_handle, GENDERS_FILE_GOOD) == -1) {
    printf("genders_open() error\n");
    return -1;
  }

  if ((test_env->not_open_handle = genders_handle_create()) == NULL) {
    printf("genders_handle_create() error\n");
    return -1;
  }

  test_env->nodelist_len = GENDERS_FILE_GOOD_NUMNODES;
  test_env->attrlist_len = GENDERS_FILE_GOOD_NUMATTRS;
  test_env->vallist_len = GENDERS_FILE_GOOD_MAXATTRS;
  test_env->maxnodelen = GENDERS_FILE_GOOD_MAXNODELEN;
  test_env->maxattrlen = GENDERS_FILE_GOOD_MAXATTRLEN;
  test_env->maxvallen = GENDERS_FILE_GOOD_MAXVALLEN;

  /* create lists manually, so we don't encounter errant "NOTFREE" or
   * "ISFREE" errors during testing
   */

  if (create_array_of_strings(&test_env->nodelist, 
			      test_env->nodelist_len, 
			      test_env->maxnodelen) == -1) {
    printf("create_array_of_strings() error\n");
    return -1;
  }
  if (create_array_of_strings(&test_env->attrlist, 
			      test_env->attrlist_len, 
			      test_env->maxattrlen) == -1) {
    printf("create_array_of_strings() error\n");
    return -1;
  }
  if (create_array_of_strings(&test_env->vallist, 
			      test_env->vallist_len, 
			      test_env->maxvallen) == -1) {
    printf("create_array_of_strings() error\n");
    return -1;
  }

  return 0;
}

/* cleanup the test environment */
void cleanup_test_env(struct test_env *test_env) {
  if (test_env->open_handle != NULL) {
    (void)genders_handle_destroy(test_env->open_handle);
  }
  if (test_env->not_open_handle != NULL) {
    (void)genders_handle_destroy(test_env->not_open_handle);
  }
  if (test_env->nodelist != NULL) {
    free_array_of_strings(test_env->nodelist, test_env->nodelist_len);
  }
  if (test_env->attrlist != NULL) {
    free_array_of_strings(test_env->attrlist, test_env->attrlist_len);
  }
  if (test_env->vallist != NULL) {
    free_array_of_strings(test_env->vallist, test_env->vallist_len);
  }
  return;
}

/* run a single test */
void run_a_parameter_test(struct test_env *test_env,
			  int index,
			  int function,
			  int param1,
			  int param2,
			  int param3, 
			  int param4,
			  int return_value,
			  int return_errnum) {
  int int_result;
  char *char_result;
  char *nodename = NULL;
  char *str1 = NULL;
  char *str2 = NULL;
  char **list = NULL;
  char ***list_create = NULL;
  genders_t handle = NULL;
  
  /* setup necessary test parameters */
  if (param1 == IS_NOT_NULL_NOT_OPEN) {
    handle = test_env->not_open_handle;
  }
  else if (param1 == IS_NOT_NULL_OPEN) {
    handle = test_env->open_handle;
  }

  if (function & GENDERS_CREATE && param2 == IS_NOT_NULL) {
      list_create = &list; 
  }

  if (function & GENDERS_CLEAR || function & GENDERS_DESTROY) {
    if (param2 == IS_NOT_NULL) {
      /* can be any list for parameter tests */
      list = test_env->nodelist;
    }
  }

  if (function == GENDERS_GETNODES || 
      function == GENDERS_GETATTR ||
      function == GENDERS_GETATTR_ALL) {
    if (param2 == IS_NOT_NULL) {
      /* can be any list for parameter tests */
      list = test_env->nodelist;
    }
  }

  if (function == GENDERS_GETNODENAME && param2 == IS_NOT_NULL) {
      /* grab a string from a list */
      str1 = test_env->nodelist[0];
  }

  if ((function == GENDERS_TESTATTR || function == GENDERS_TESTATTRVAL) && 
      param3 == IS_NOT_NULL) {
      /* grab a string from a list */
      str1 = test_env->nodelist[0];
  }

  if (function == GENDERS_TESTATTRVAL && param4 == IS_NOT_NULL) {
    /* grab a string from a list */
    str2 = test_env->nodelist[1];
  }

  if (function == GENDERS_GETATTR && param4 == GOOD_NODENAME) {
    nodename = GENDERS_GOOD_NODENAME;
  } 
  else if (function == GENDERS_GETATTR && param4 == BAD_NODENAME) {
    nodename = GENDERS_BAD_NODENAME;
  }

  if ((function == GENDERS_TESTATTR || function == GENDERS_TESTATTRVAL) && 
      param2 == GOOD_NODENAME) {
    nodename = GENDERS_GOOD_NODENAME;
  }
  else if ((function == GENDERS_TESTATTR || function == GENDERS_TESTATTRVAL) && 
      param2 == BAD_NODENAME) {
    nodename = GENDERS_BAD_NODENAME;
  } 

  if (function == GENDERS_HANDLE_DESTROY) {
    int_result = genders_handle_destroy(handle);
  }
  else if (function == GENDERS_OPEN) {
  }
  else if (function == GENDERS_CLOSE) {
    int_result = genders_close(handle);
  }
  else if (function == GENDERS_ERRNUM) {
    int_result = genders_errnum(handle);
  }
  else if (function == GENDERS_STRERROR) {
    char_result = genders_strerror(param1);
  }
  else if (function == GENDERS_ERRORMSG) {
    char_result = genders_errormsg(handle);
  }
  else if (function == GENDERS_GETNUMNODES) {
    int_result = genders_getnumnodes(handle);
  }
  else if (function == GENDERS_GETNUMATTRS) {
    int_result = genders_getnumattrs(handle);
  }
  else if (function == GENDERS_GETMAXATTRS) {
    int_result = genders_getmaxattrs(handle);
  }
  else if (function == GENDERS_GETMAXNODELEN) {
    int_result = genders_getmaxnodelen(handle);
  }
  else if (function == GENDERS_GETMAXATTRLEN) {
    int_result = genders_getmaxattrlen(handle);
  }
  else if (function == GENDERS_GETMAXVALLEN) {
    int_result = genders_getmaxvallen(handle);
  }
  else if (function == GENDERS_NODELIST_CREATE) {
    int_result = genders_nodelist_create(handle, list_create);
  }
  else if (function == GENDERS_NODELIST_CLEAR) {
    int_result = genders_nodelist_clear(handle, list);
  }
  else if (function == GENDERS_NODELIST_DESTROY) {
    int_result = genders_nodelist_destroy(handle, list);
  }
  else if (function == GENDERS_ATTRLIST_CREATE) {
    int_result = genders_attrlist_create(handle, list_create);
  }
  else if (function == GENDERS_ATTRLIST_CLEAR) {
    int_result = genders_attrlist_clear(handle, list);
  }
  else if (function == GENDERS_ATTRLIST_DESTROY) {
    int_result = genders_attrlist_destroy(handle, list);
  }
  else if (function == GENDERS_VALLIST_CREATE) {
    int_result = genders_vallist_create(handle, list_create);
  }
  else if (function == GENDERS_VALLIST_CLEAR) {
    int_result = genders_vallist_clear(handle, list);
  }
  else if (function == GENDERS_VALLIST_DESTROY) {
    int_result = genders_vallist_destroy(handle, list);
  }
  else if (function == GENDERS_GETNODENAME) {
    int_result = genders_getnodename(handle, str1, param3);
  }
  else if (function == GENDERS_GETNODES) {
    int_result = genders_getnodes(handle, list, param3, NULL, NULL);
  }
  else if (function == GENDERS_GETATTR) {
    int_result = genders_getattr(handle, list, NULL, param3, nodename);
  }
  else if (function == GENDERS_GETATTR_ALL) {
    int_result = genders_getattr_all(handle, list, param3);
  }
  else if (function == GENDERS_TESTATTR) {
    int_result = genders_testattr(handle, nodename, str1, NULL);
  }
  else if (function == GENDERS_TESTATTRVAL) {
    int_result = genders_testattrval(handle, nodename, str1, str2);
  }
  else if (function == GENDERS_TESTNODE) {
    int_result = genders_testnode(handle, NULL);
  }
  else if (function == GENDERS_ELAPSEDTIME) {
    int_result = genders_elapsedtime(handle);
  }
  else {
    printf("Error: Incorrect function\n");
    return;
  }
  
  if (function & GENDERS_INT_RESULT) {
    if (int_result == return_value && 
	(return_errnum == NO_ERRNUM || return_errnum == genders_errnum(handle))) {
      printf("Test %d: %s: PASS\n", index, function_names[function & GENDERS_FUNCTION_MASK]);
    }
    else {
      printf("Test %d: %s: ***FAIL***, return_value = %d, return_errnum = %d\n", 
	     index, function_names[function & GENDERS_FUNCTION_MASK], 
	     int_result, genders_errnum(handle));
    }
  }
  else {
    if (((char_result == NULL && return_value == IS_NULL) || 
	 (char_result != NULL && return_value == IS_NOT_NULL)) &&
	(return_errnum == NO_ERRNUM || return_errnum == genders_errnum(handle))) {
      printf("Test %d: %s: PASS\n", index, function_names[function & GENDERS_FUNCTION_MASK]);
    } 
    else {
      printf("Test %d: %s: ***FAIL***, return_value = %p, return_errnum = %d\n", 
	     index, function_names[function & GENDERS_FUNCTION_MASK], 
	     char_result, genders_errnum(handle));
    }
  }
} 

/* run all the parameter tests */
int run_parameter_tests(struct test_env *test_env) {
  int index = 0;
  
  printf("libgenders parameter tests                                  \n");
  printf("------------------------------------------------------------\n");
  while (parameter_tests[index].function != -1) {
    run_a_parameter_test(test_env, 
			 index,
			 parameter_tests[index].function,
			 parameter_tests[index].param1,
			 parameter_tests[index].param2,
			 parameter_tests[index].param3,
			 parameter_tests[index].param4,
			 parameter_tests[index].return_value,
			 parameter_tests[index].return_errnum);
    index++;
  }

  return 0;
}

int main(int argc, char **argv) {
 
  struct test_env *test_env = NULL;
  
  if ((test_env = (struct test_env *)malloc(sizeof(struct test_env))) == NULL) {
    printf("malloc() errorn\n");
    goto cleanup;
  }

  if (initialize_test_env(test_env) == -1) {
    printf("initialize_test_env() error\n");
    goto cleanup;
  }
  
  if (run_parameter_tests(test_env) == -1) {
    printf("run_parameter_tests() error\n");
    goto cleanup;
  }

  cleanup_test_env(test_env);
  free(test_env);
  return 0;
  
 cleanup:
  
  if (test_env != NULL) {
    cleanup_test_env(test_env);
    free(test_env);
  }
  
  return -1;
}
