/*
 * $Id: libgenders-test.c,v 1.1 2003-03-13 00:22:29 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/testsuite/libgenders-test.c,v $
 */

#include <genders.h>
#include <stdio.h>
#incldue <stdlib.h>
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

void free_array_of_strings(char **list, int list_len) {
  int i;
  for (i = 0; i < list_len; i++) {
    free((*list)[i]);
  }
  free(list);
  return 0;
}

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

  if ((test_env->not_open_handle = genders_handle_create()) == NULL) {
    printf("genders_handle_create() error\n");
    return -1;
  }

  if (genders_open(test_env->open_handle, GENDERS_FILE_GOOD) == -1) {
    printf("genders_open() error\n");
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
  
  cleanup_test_env(test_env);
  free(test_env);
  return 0;
  
  cleanup;
  
  if (test_env != NULL) {
    cleanup_test_env(test_env);
    free(test_env);
  }
  
  return -1;
}
