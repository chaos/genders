/*
 * $Id: genders_example.c,v 1.5 2003-03-18 00:19:41 achu Exp $
 * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/libgenders/genders_example.c,v $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include "genders.h"
#include <mcheck.h>
#include <string.h>

int numnodes = 0;
int numattrs = 0;
int maxattrs = 0;
int maxnodelen = 0;
int maxattrlen = 0;
int maxvallen = 0;
char **nodes = NULL;
char **attrs = NULL;
char **vals = NULL;
int nodeslen = 0;
int attrslen = 0;
int valslen = 0;

void usage() {
  printf("Usage: genders_example <genders_file>\n");
  printf("       <genders_file> - genders file to be parsed\n");
  exit(0);
}

void cleanup_and_exit(genders_t handle, int status) {

  if (nodes != NULL && genders_nodelist_destroy(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
  }

  if (attrs != NULL && genders_attrlist_destroy(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
  }

  if (vals != NULL && genders_vallist_destroy(handle, vals) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
  }
  
  exit(status);
}

void dump_genders_file(genders_t handle) {

  printf("Dumping contents of the gender file\n");
  printf("-----------------------------------\n");

  if (genders_handle_dump(handle, NULL) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }
  printf("\n");
}

void get_this_node_name(genders_t handle) {
  char nodename[MAXHOSTNAMELEN+1];
  memset(nodename,'\0', MAXHOSTNAMELEN+1);

  if ((genders_getnodename(handle, nodename, MAXHOSTNAMELEN+1)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("Node Information\n");
  printf("----------------\n");
  printf("This Node's Name Is: %s\n", nodename);
  printf("\n");

  return;
}

void get_gender_file_information(genders_t handle) {

  if ((numnodes = genders_getnumnodes(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((numattrs = genders_getnumattrs(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((maxattrs = genders_getmaxattrs(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((maxnodelen = genders_getmaxnodelen(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((maxattrlen = genders_getmaxattrlen(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((maxvallen = genders_getmaxvallen(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("Genders File Information\n");
  printf("------------------------\n");
  printf("Max number of nodes: %d\n", numnodes);
  printf("Max number of attributes: %d\n", numattrs);
  printf("Max number of attributes at any one node: %d\n", maxattrs);
  printf("Max node name length: %d\n", maxnodelen);
  printf("Max attribute name length: %d\n", maxattrlen);
  printf("Max value name length: %d\n", maxvallen);
  printf("\n");

  return;
}

void get_nodes_in_gender_file(genders_t handle) {
  int ret, i;

  if (numnodes == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }
    
  if (ret > 0) {
    printf("Nodes in the genders file\n");
    printf("-------------------------\n");
    for (i = 0; i < ret-1; i++) {
      printf("%s, ", nodes[i]);
    }
    printf("%s\n\n", nodes[ret - 1]);
  }

  return;
}

void get_attributes_for_all_nodes(genders_t handle) {
  int ret, i, j;

  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (ret == 0)
    return;

  printf("Attribute Information\n");
  printf("---------------------\n");
  for (i = 0; i < numnodes; i++) {
    if (genders_attrlist_clear(handle, attrs) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }

    if ((ret = genders_getattr(handle, attrs, NULL, attrslen, nodes[i])) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }    

    if (ret > 0) {
      printf("Attributes for node \"%s\": ", nodes[i]);
      for (j = 0; j < ret - 1; j++) {
	printf("%s, ", attrs[j]);
      }
      printf("%s\n", attrs[ret-1]);
    }
  }
  printf("\n");

  return;
  
}

void get_attributes_and_values_for_all_nodes(genders_t handle) {
  int ret, i, j;

  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (ret == 0)
    return;

  printf("Attribute and Value Information\n");
  printf("-------------------------------\n");
  for (i = 0; i < numnodes; i++) {

    if (genders_attrlist_clear(handle, attrs) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }

    if (genders_vallist_clear(handle, vals) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }

    if ((ret = genders_getattr(handle, attrs, vals, attrslen, nodes[i])) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }    

    if (ret > 0) {
      printf("Attributes and values for node \"%s\": ", nodes[i]);
      for (j = 0; j < ret - 1; j++) {
	if (strlen(vals[j]) > 0) {
	  printf("%s=%s, ", attrs[j], vals[j]);
	}
	else {
	  printf("%s, ", attrs[j]);
	}
      }
      if (strlen(vals[ret - 1]) > 0) {
	printf("%s=%s\n", attrs[ret-1], vals[ret-1]);
      }
      else {
	printf("%s\n", attrs[ret-1]);
      }
    }

  }
  printf("\n");

  return;
}

void get_all_attributes(genders_t handle) {
  int ret, i;

  if (numnodes == 0 || numattrs == 0) {
    return;
  }

  if (genders_attrlist_clear(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getattr_all(handle, attrs, attrslen)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("All attributes listed in the genders file\n");
  printf("-----------------------------------------\n");
  if (ret > 0) {
    for (i = 0; i < ret - 1; i++) {
      printf("%s, ", attrs[i]);
    }
    printf("%s\n", attrs[ret-1]);
  }
  printf("\n");
}

void get_all_nodes_with_a_certain_attribute(genders_t handle) {
  int num_attributes, ret, i, j;

  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_attrlist_clear(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((num_attributes = genders_getattr_all(handle, attrs, attrslen)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (num_attributes == 0)
    return;

  printf("Nodes with the same attributes\n");
  printf("------------------------------\n");
  for (i = 0; i < num_attributes; i++) {

    if (genders_nodelist_clear(handle, nodes) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }

    if ((ret = genders_getnodes(handle, nodes, nodeslen, attrs[i], NULL)) == -1) {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }    

    if (ret > 0) {
      printf("Nodes with attribute \"%s\": ", attrs[i]);
      for (j = 0; j < ret - 1; j++) {
	  printf("%s, ", nodes[j]);
      }
      printf("%s\n", nodes[ret-1]);
    }
  }
  printf("\n");

  return;

}

void get_all_nodes_with_a_certain_attribute_and_value(genders_t handle) {
  int num_attributes, ret, i, j;

  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  } 

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (ret == 0)
    return;

  if (genders_attrlist_clear(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_vallist_clear(handle, vals) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((num_attributes = genders_getattr(handle, attrs, vals, attrslen, nodes[0])) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (num_attributes == 0)
    return;

  printf("Nodes with the same attribute and values as the first node entry\n");
  printf("----------------------------------------------------------------\n");
  for (i = 0; i < num_attributes; i++) {

    if (strlen(vals[i]) > 0) {

      if (genders_nodelist_clear(handle, nodes) == -1) {
	printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
	cleanup_and_exit(handle,1);
      }

      if ((ret = genders_getnodes(handle, nodes, nodeslen, attrs[i], vals[i])) == -1) {
	printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
	cleanup_and_exit(handle,1);
      }    
    
      if (ret > 0) {
	printf("Nodes with attribute \"%s\"=\"%s\": ", attrs[i], vals[i]);
	for (j = 0; j < ret - 1; j++) {
	  printf("%s, ", nodes[j]);
	}
	printf("%s\n", nodes[ret-1]);
      }
    }
  }
  printf("\n");

  return;
}

void test_if_node_has_attribute(genders_t handle) {
  int num_attributes, ret, i;
  
  if (numnodes == 0 || numattrs == 0)
    return;
  
  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_attrlist_clear(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((num_attributes = genders_getattr(handle, attrs, NULL, attrslen, nodes[0])) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (num_attributes == 0)
    return;

  printf("Does the first node entry have the following attributes?\n");
  printf("--------------------------------------------------------\n");

  for (i = 0; i < num_attributes; i++) {
    printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], attrs[i]);

    ret = genders_testattr(handle, nodes[0], attrs[i], NULL, 0);
    if (ret == 1) {
      printf("Yes\n");
    }
    else if (ret == 0) {
      printf("No\n");
    }
    else {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }
  }

  printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], "foobar");
  ret = genders_testattr(handle, nodes[0], "foobar", NULL, 0);
  if (ret == 1) {
    printf("Yes\n");
  }
  else if (ret == 0) {
    printf("No\n");
  }
  else {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("\n");

  return;
}

void test_if_node_has_attribute_and_return_the_attribute_value(genders_t handle) {
  int num_attributes, ret, i;
  char buffer[maxvallen+1];
  
  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_attrlist_clear(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  } 

  if ((num_attributes = genders_getattr(handle, attrs, NULL, attrslen, nodes[0])) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (num_attributes == 0)
    return;

  printf("Does the first node entry have the following attributes, and what are the values?\n");
  printf("--------------------------------------------------------------------------------\n");

  for (i = 0; i < num_attributes ; i++) {
    printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], attrs[i]);

    memset(buffer, '\0', maxvallen + 1);
    ret = genders_testattr(handle, nodes[0], attrs[i], buffer, maxvallen+1);
    if (ret == 1) {
      if (strlen(buffer) > 0) {
	printf("Yes, and it has value \"%s\"\n", buffer);
      }
      else {
	printf("Yes, but it has no value\n");
      }
    }
    else if (ret == 0) {
      printf("No\n");
    }
    else {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }
  }

  printf("Does node \"%s\" have attribute \"%s\"?: ", nodes[0], "foobar");

  memset(buffer, '\0', maxvallen + 1);
  ret = genders_testattr(handle, nodes[0], "foobar", buffer, maxvallen+1);
  if (ret == 1) {
    if (strlen(buffer) > 0) {
      printf("Yes, and it has value \"%s\"\n", buffer);
    }
    else {
      printf("Yes, but it has no value\n");
    }
  }
  else if (ret == 0) {
    printf("No\n");
  }
  else {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("\n");

  return;
}

void test_if_node_has_attribute_equal_to_a_value(genders_t handle) {
  int num_attributes, ret, i;
  
  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_attrlist_clear(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  } 

  if (genders_vallist_clear(handle, vals) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  } 

  if ((num_attributes = genders_getattr(handle, attrs, vals, attrslen, nodes[0])) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (num_attributes == 0)
    return;

  printf("Does the first node entry have the following attributes=value?\n");
  printf("--------------------------------------------------------------\n");

  for (i = 0; i < num_attributes ; i++) {
    if (strlen(vals[i]) > 0) {
      printf("Does node \"%s\" have attribute \"%s\"=\"%s\"?: ", nodes[0], attrs[i], vals[i]);

      ret = genders_testattrval(handle, nodes[0], attrs[i], vals[i]);
      if (ret == 1) {
	printf("Yes\n");
      }
      else if (ret == 0) {
	printf("No\n");
      }
      else {
	printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
	cleanup_and_exit(handle,1);
      }

      printf("Does node \"%s\" have attribute \"%s\"=\"%s\"?: ", nodes[0], attrs[i], "foo");
      ret = genders_testattrval(handle, nodes[0], attrs[i], "foo");
      if (ret == 1) {
	printf("Yes\n");
      }
      else if (ret == 0) {
	printf("No\n");
      }
      else {
	printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
	cleanup_and_exit(handle,1);
      }
    }
  }

  printf("Does node \"%s\" have attribute \"%s\"=\"%s\"?: ", nodes[0], "foobar", "foo");

  ret = genders_testattrval(handle, nodes[0], "foobar", "foo");
  if (ret == 1) {
    printf("Yes\n");
  }
  else if (ret == 0) {
    printf("No\n");
  }
  else {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("\n");

  return;
}

void test_if_genders_file_has_certain_nodes(genders_t handle) {
  int num_attributes, ret, i;
  
  if (numnodes == 0 || numattrs == 0)
    return;

  if (genders_nodelist_clear(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((ret = genders_getnodes(handle, nodes, nodeslen, NULL, NULL)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("Does the genders file have the following nodes?               \n");
  printf("--------------------------------------------------------------\n");

  for (i = 0; i < numnodes ; i++) {
    printf("Does the genders file have node \"%s\"? ", nodes[i]);
    
    ret = genders_testnode(handle, nodes[i]);
    if (ret == 1) {
      printf("Yes\n");
    }
    else if (ret == 0) {
      printf("No\n");
    }
    else {
      printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
      cleanup_and_exit(handle,1);
    }
  }
  
  printf("Does the genders file have node \"%s\"? ", "foo");
    
  ret = genders_testnode(handle, "foo");
  if (ret == 1) {
    printf("Yes\n");
  }
  else if (ret == 0) {
    printf("No\n");
  }
  else {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("\n");

  return;
}

void time_this_example_genders_took_to_run(genders_t handle) {
  int seconds;

  if ((seconds = genders_elapsedtime(handle)) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  printf("Time this executable took to run\n");
  printf("--------------------------------\n");
  printf("Time: %d\n", seconds);
  printf("\n");
}

void example_using_genders_perror_and_errormsg(genders_t handle) {
  /* Intentionally force errors to test and illustate genders_perror */
  printf("Intentional errors to test and illustrate genders error routines()\n");
  printf("------------------------------------------------------------------\n");

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) == -1) {
    genders_perror(handle, "Passing in Invalid Parameter, With messages");
  }

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) == -1) {
    genders_perror(handle, NULL);
  }

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) == -1) {
    printf("%s\n",genders_errormsg(handle));
  }

  if (genders_getnodes(handle, NULL, 0, NULL, NULL) == -1) {
    printf("%s\n",genders_errormsg(handle));
  }

  printf("\n");

}

int main(int argc, char **argv) {
  genders_t handle;
  char *filename;
mtrace();
  if (argc != 2) {
    usage();
  }
  else {
    filename = argv[1];
  }

  if ((handle = genders_handle_create()) == NULL) {
    printf("Error: Cannot create handle.\n");
    cleanup_and_exit(handle,1);
  }

  if (genders_open(handle,filename) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((nodeslen = genders_nodelist_create(handle, &nodes)) <= 0) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((attrslen = genders_attrlist_create(handle, &attrs)) <= 0) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if ((valslen = genders_vallist_create(handle, &vals)) <= 0) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  dump_genders_file(handle);

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

  time_this_example_genders_took_to_run(handle);

  example_using_genders_perror_and_errormsg(handle);

  if (genders_nodelist_destroy(handle, nodes) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_attrlist_destroy(handle, attrs) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_vallist_destroy(handle, vals) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_close(handle) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  if (genders_handle_destroy(handle) == -1) {
    printf("Error: %s\n", genders_strerror(genders_errnum(handle)));
    cleanup_and_exit(handle,1);
  }

  exit(0);
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
