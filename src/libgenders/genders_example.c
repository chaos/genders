/*
 * $Id: genders_example.c,v 1.13 2003-11-03 17:36:39 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/libgenders/genders_example.c,v $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include "genders.h"
#include <string.h>

char **nodes = NULL;
char **attrs = NULL;
char **vals = NULL;
int nodeslen = 0;
int attrslen = 0;
int valslen = 0;

#define output_error_and_return                                           \ 
  do {                                                                    \
    printf("Error %s(): %s\n\n", __FUNCTION__, genders_errormsg(handle)); \
    return;                                                               \
  } while (0)

#define output_error_and_exit
  do {                                                                    \
    printf("Error %s(): %s\n\n", __FUNCTION__, genders_errormsg(handle)); \
    exit(1);                                                              \
  } while (0)

void usage() {
  printf("Usage: genders_example <genders_file>\n");
  printf("       <genders_file> - genders file to be parsed\n");
  exit(0);
}

void get_this_node_name(genders_t handle) {
  char nodename[MAXHOSTNAMELEN+1];
  memset(nodename,'\0', MAXHOSTNAMELEN+1);

  printf("Node Information\n");
  printf("----------------\n");

  if ((genders_getnodename(handle, nodename, MAXHOSTNAMELEN+1)) < 0)
    output_error_and_return;

  printf("This Node's Name Is: %s\n\n", nodename);
}

void get_gender_file_information(genders_t handle) {
  int numnodes = 0;
  int numattrs = 0;
  int maxattrs = 0;
  int maxnodelen = 0;
  int maxattrlen = 0;
  int maxvallen = 0;

  printf("Genders File Information\n");
  printf("------------------------\n");

  if ((numnodes = genders_getnumnodes(handle)) < 0)
    output_error_and_return;

  if ((numattrs = genders_getnumattrs(handle)) < 0)
    output_error_and_return;

  if ((maxattrs = genders_getmaxattrs(handle)) < 0)
    output_error_and_return;

  if ((maxnodelen = genders_getmaxnodelen(handle)) < 0)
    output_error_and_return;

  if ((maxattrlen = genders_getmaxattrlen(handle)) < 0)
    output_error_and_return;

  if ((maxvallen = genders_getmaxvallen(handle)) < 0)
    output_error_and_return;

  printf("Max number of nodes: %d\n", numnodes);
  printf("Max number of attributes: %d\n", numattrs);
  printf("Max number of attributes at any one node: %d\n", maxattrs);
  printf("Max node name length: %d\n", maxnodelen);
  printf("Max attribute name length: %d\n", maxattrlen);
  printf("Max value name length: %d\n", maxvallen);
  printf("\n");
}

void get_nodes_in_gender_file(genders_t handle) {
  int numnodes, i;

  printf("Nodes in the genders file\n");
  printf("-------------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0)
    output_error_and_return;

  if ((numnodes = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;
    
  if (numnodes > 0) {
    for (i = 0; i < numnodes-1; i++)
      printf("%s, ", nodes[i]);
    printf("%s\n", nodes[numnodes - 1]);
  }
  printf("\n");
}

void get_attributes_for_all_nodes(genders_t handle) {
  int numnodes, numattrs, i, j;

  printf("Attribute Information\n");
  printf("---------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0)
    output_error_and_return;

  if ((numnodes = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;

  for (i = 0; i < numnodes; i++) {
    if (genders_attrlist_clear(handle, attrs) < 0)
      output_error_and_return;

    if ((numattrs = genders_getattr(handle, attrs, NULL, attrslen, nodes[i])) < 0)
      output_error_and_return;

    if (numattrs > 0) {
      printf("Attributes for node \"%s\": ", nodes[i]);
      for (j = 0; j < numattrs - 1; j++)
        printf("%s, ", attrs[j]);
      printf("%s\n", attrs[numattrs-1]);
    }
  }
  printf("\n");
}

void get_attributes_and_values_for_all_nodes(genders_t handle) {
  int numnodes, numattrs, i, j;

  printf("Attribute and Value Information\n");
  printf("-------------------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0)
      output_error_and_return;

  if ((numnodes = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
      output_error_and_return;

  for (i = 0; i < numnodes; i++) {

    if (genders_attrlist_clear(handle, attrs) < 0)
      output_error_and_return;

    if (genders_vallist_clear(handle, vals) < 0)
      output_error_and_return;

    if ((numattrs = genders_getattr(handle, attrs, vals, attrslen, nodes[i])) < 0)
      output_error_and_return;

    if (numattrs > 0) {
      printf("Attributes and values for node \"%s\": ", nodes[i]);
      for (j = 0; j < numattrs - 1; j++) {
        if (strlen(vals[j]) > 0)
          printf("%s=%s, ", attrs[j], vals[j]);
        else
          printf("%s, ", attrs[j]);
      }
      if (strlen(vals[numattrs - 1]) > 0)
        printf("%s=%s\n", attrs[numattrs-1], vals[numattrs-1]);
      else
        printf("%s\n", attrs[numattrs-1]);
    }

  }
  printf("\n");
}

void get_all_attributes(genders_t handle) {
  int numattrs, i;

  printf("All attributes listed in the genders file\n");
  printf("-----------------------------------------\n");

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr_all(handle, attrs, attrslen)) < 0)
    output_error_and_return;

  if (numattrs > 0) {
    for (i = 0; i < numattrs - 1; i++)
      printf("%s, ", attrs[i]);
    printf("%s\n", attrs[numattrs-1]);
  }
  printf("\n");
}

void get_all_nodes_with_a_certain_attribute(genders_t handle) {
  int numattrs, ret, i, j;

  printf("Nodes with the same attributes\n");
  printf("------------------------------\n");

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr_all(handle, attrs, attrslen)) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs; i++) {
    int numnodes;

    if (genders_nodelist_clear(handle, nodes) < 0)
      output_error_and_return;

    if ((numnodes = genders_getnodes(handle, nodes, nodeslen, attrs[i], NULL)) < 0)
      output_error_and_return;

    if (numnodes > 0) {
      printf("Nodes with attribute \"%s\": ", attrs[i]);
      for (j = 0; j < numnodes - 1; j++)
	printf("%s, ", nodes[j]);
      printf("%s\n", nodes[numnodes-1]);
    }
  }
  printf("\n");
}

void get_all_nodes_with_a_certain_attribute_and_value(genders_t handle) {
  int numattrs, numnodes, i, j;

  printf("Nodes with the same attribute and values as the first node\n");
  printf("----------------------------------------------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0) 
    output_error_and_return;

  if ((numnodes = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;

  if (numnodes == 0)
    return;

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if (genders_vallist_clear(handle, vals) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr(handle, attrs, vals, attrslen, nodes[0])) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs; i++) {

    if (strlen(vals[i]) > 0) {
      if (genders_nodelist_clear(handle, nodes) < 0)
	output_error_and_return;

      if ((numnodes = genders_getnodes(handle, nodes, nodeslen, attrs[i], vals[i])) < 0)
	output_error_and_return;
    
      if (numnodes > 0) {
        printf("Nodes with attribute \"%s\"=\"%s\": ", attrs[i], vals[i]);
        for (j = 0; j < numnodes - 1; j++)
          printf("%s, ", nodes[j]);
        printf("%s\n", nodes[numnodes-1]);
      }
    }
  }
  printf("\n");
}

void test_if_node_has_attribute(genders_t handle) {
  int numattrs, numnodes, ret, i;
  
  printf("Does the first node entry have the following attributes?\n");
  printf("--------------------------------------------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0)
    output_error_and_return;

  if ((numnodes = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr(handle, attrs, NULL, attrslen, nodes[0])) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs; i++) {
    printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], attrs[i]);

    ret = genders_testattr(handle, nodes[0], attrs[i], NULL, 0);
    if (ret == 1)
      printf("Yes\n");
    else if (ret == 0)
      printf("No\n");
    else
      output_error_and_return;
  }

  printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], "foobar");
  ret = genders_testattr(handle, nodes[0], "foobar", NULL, 0);
  if (ret == 1)
    printf("Yes\n");
  else if (ret == 0)
    printf("No\n");
  else
    output_error_and_return;

  printf("\n");
}

void test_if_node_has_attribute_and_return_the_attribute_value(genders_t handle) {
  int numattrs, ret, i;
  char buffer[maxvallen+1];
  
  printf("Does the first node entry have the following attributes and values?\n");
  printf("-------------------------------------------------------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0)
    output_error_and_return;

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr(handle, attrs, NULL, attrslen, nodes[0])) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs ; i++) {
    printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], attrs[i]);
    memset(buffer, '\0', maxvallen + 1);
    ret = genders_testattr(handle, nodes[0], attrs[i], buffer, maxvallen+1);
    if (ret == 1) {
      if (strlen(buffer) > 0)
        printf("Yes, and it has value \"%s\"\n", buffer);
      else
        printf("Yes, but it has no value\n");
    }
    else if (ret == 0)
      printf("No\n");
    else
      output_error_and_return;
  }

  printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], "foobar");

  memset(buffer, '\0', maxvallen + 1);
  ret = genders_testattr(handle, nodes[0], "foobar", buffer, maxvallen+1);
  if (ret == 1) {
    if (strlen(buffer) > 0)
      printf("Yes, and it has value \"%s\"\n", buffer);
    else
      printf("Yes, but it has no value\n");
  }
  else if (ret == 0)
    printf("No\n");
  else
    output_error_and_return;

  printf("\n");
}

void test_if_node_has_attribute_equal_to_a_value(genders_t handle) {
  int numattrs, ret, i;
  
  printf("Does the first node entry have the following attributes=value?\n");
  printf("--------------------------------------------------------------\n");

  if (genders_nodelist_clear(handle, nodes) < 0)
    output_error_and_return;

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if (genders_vallist_clear(handle, vals) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr(handle, attrs, vals, attrslen, nodes[0])) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs ; i++) {
    if (strlen(vals[i]) > 0) {
      printf("Does node \"%s\" have attribute \"%s\"=\"%s\"?: ", nodes[0], attrs[i], vals[i]);

      ret = genders_testattrval(handle, nodes[0], attrs[i], vals[i]);
      if (ret == 1)
        printf("Yes\n");
      else if (ret == 0)
        printf("No\n");
      else
	output_error_and_return;

      printf("Does node \"%s\" have attribute \"%s\"=\"%s\"?: ", nodes[0], attrs[i], "foo");
      ret = genders_testattrval(handle, nodes[0], attrs[i], "foo");
      if (ret == 1)
        printf("Yes\n");
      else if (ret == 0)
        printf("No\n");
      else
	output_error_and_return;
    }
  }

  printf("Does node \"%s\" have attribute \"%s\"=\"%s\"?: ", nodes[0], "foobar", "foo");

  ret = genders_testattrval(handle, nodes[0], "foobar", "foo");
  if (ret == 1)
    printf("Yes\n");
  else if (ret == 0)
    printf("No\n");
  else
    output_error_and_return;

  printf("\n");
}

void test_if_genders_file_has_certain_nodes(genders_t handle) {
  int ret, i;

  printf("Does the genders file have the following nodes?               \n");
  printf("--------------------------------------------------------------\n");
  
  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) < 0)
    output_error_and_return;

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) < 0)
    output_error_and_return;

  for (i = 0; i < numnodes ; i++) {
    printf("Does the genders file have node \"%s\"? ", nodes[i]);
    
    ret = genders_isnode(handle, nodes[i]);
    if (ret == 1)
      printf("Yes\n");
    else if (ret == 0)
      printf("No\n");
    else
      output_error_and_return;
  }

  printf("Does the genders file have node \"%s\"? ", "foo");

  ret = genders_isnode(handle, "foo");
  if (ret == 1)
    printf("Yes\n");
  else if (ret == 0)
    printf("No\n");
  else
    output_error_and_return;

  printf("\n");
}


void test_if_genders_file_has_certain_attrs(genders_t handle) {
  int ret, numattrs, i;
  
  printf("Does the genders file have the following attrs?               \n");
  printf("--------------------------------------------------------------\n");

  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr(handle, attrs, NULL, attrslen, NULL)) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs; i++) {
    printf("Does the genders file have attr \"%s\"? ", attrs[i]);
    
    ret = genders_isattr(handle, attrs[i]);
    if (ret == 1)
      printf("Yes\n");
    else if (ret == 0)
      printf("No\n");
    else
      output_error_and_return;
  }
  
  printf("Does the genders file have attr \"%s\"? ", "foo");
    
  ret = genders_isattr(handle, "foo");
  if (ret == 1)
    printf("Yes\n");
  else if (ret == 0)
    printf("No\n");
  else
    output_error_and_return;

  printf("\n");
}

void test_if_genders_file_has_certain_attrvals(genders_t handle) {
  int ret, numattrs, i;
  
  printf("Does the genders file have the following attrvals?            \n");
  printf("--------------------------------------------------------------\n");

  if (genders_attrlist_clear(handle, attrs) < 0)
    output_error_and_return;

  if (genders_vallist_clear(handle, vals) < 0)
    output_error_and_return;

  if ((numattrs = genders_getattr(handle, attrs, vals, attrslen, NULL)) < 0)
    output_error_and_return;

  for (i = 0; i < numattrs; i++) {
    printf("Does the genders file have \"%s=%s\"? ", attrs[i], vals[i]);
    
    ret = genders_isattrval(handle, attrs[i], vals[i]);
    if (ret == 1)
      printf("Yes\n");
    else if (ret == 0)
      printf("No\n");
    else
      output_error_and_return;

    printf("Does the genders file have \"%s=%s\"? ", attrs[i], "foo");
    
    ret = genders_isattrval(handle, attrs[i], "foo");
    if (ret == 1)
      printf("Yes\n");
    else if (ret == 0)
      printf("No\n");
    else
      output_error_and_return;
  }
 
  printf("\n");
}

void example_using_genders_perror_and_errormsg(genders_t handle) {
  printf("Intentional errors to illustrate genders error routines()\n");
  printf("---------------------------------------------------------\n");

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) < 0)
    genders_perror(handle, "Passing in Invalid Parameter, With messages");

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) < 0)
    genders_perror(handle, NULL);

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) < 0)
    printf("%s\n",genders_errormsg(handle));

  printf("\n");

}

int main(int argc, char **argv) {
  genders_t handle;

  if (argc != 2)
    usage();
  else
    filename = argv[1];

  if ((handle = genders_handle_create()) == NULL)
    output_error_and_exit;
    
  if (genders_load_data(handle,argv[1]) < 0)
    output_error_and_exit;

  if ((nodeslen = genders_nodelist_create(handle, &nodes)) < 0)
    output_error_and_exit;

  if ((attrslen = genders_attrlist_create(handle, &attrs)) < 0)
    output_error_and_exit;

  if ((valslen = genders_vallist_create(handle, &vals)) < 0)
    output_error_and_exit;

  get_this_node_name(handle);
  get_gender_file_information(handle);
  get_nodes_in_gender_file(handle);
  get_attributes_for_all_nodes(handle);
  get_attributes_and_values_for_all_nodes(handle);
  get_all_attributes(handle);
  get_all_nodes_with_a_certain_attribute(handle);
  get_all_nodes_with_a_certain_attribute_and_value(handle);
  test_if_node_has_attribute(handle);
  test_if_node_has_attribute_and_return_the_attribute_value(handle);
  test_if_node_has_attribute_equal_to_a_value(handle);
  test_if_genders_file_has_certain_nodes(handle);
  test_if_genders_file_has_certain_attrs(handle);
  test_if_genders_file_has_certain_attrvals(handle);
  example_using_genders_perror_and_errormsg(handle);

  if (genders_nodelist_destroy(handle, nodes) < 0)
    output_error_and_exit;

  if (genders_attrlist_destroy(handle, attrs) < 0)
    output_error_and_exit;

  if (genders_vallist_destroy(handle, vals) < 0)
    output_error_and_exit;

  if (genders_handle_destroy(handle) < 0)
    output_error_and_exit;
}
