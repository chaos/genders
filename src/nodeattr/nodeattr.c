/*
 *  * $Id: nodeattr.c,v 1.2 2003-03-18 00:18:57 achu Exp $
 *  * $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/nodeattr/nodeattr.c,v $
 *    
 */

#include <stdio.h>
#include <getopt.h>
#include <assert.h>

#include "genders.h"
#include "hostlist.h"

#define HAVE_GETOPT_LONG 1

#if HAVE_GETOPT_LONG
#define GETOPT(ac,av,opt,lopt) getopt_long(ac,av,opt,lopt,NULL)
#else
#define GETOPT(ac,av,opt,lopt) getopt(ac,av,opt)
#endif

#define OPTIONS "cnsqvlCrf:"
static struct option longopts[] = {
    { "querycomma", 0, 0, 'c' },
    { "querynl", 0, 0, 'n' },
    { "queryspace", 0, 0, 'n' },
    { "query", 0, 0, 'q' },
    { "value", 0, 0, 'v' },
    { "cluster", 0, 0, 'C' },
    { "listattr", 0, 0, 'l' },
    { "altnames", 0, 0, 'r' },
    { "filename", 1, 0, 'f' },
    { 0,0,0,0 },
};

typedef enum { FMT_COMMA, FMT_NL, FMT_SPACE, FMT_HOSTLIST } fmt_t;

static int test_attr(genders_t gp, char *node, char *attr, int vopt);
static void list_nodes(genders_t gp, char *attr, fmt_t fmt, int ropt);
static void list_attrs(genders_t gp, char *node);
static void usage(void);

/* Utility functions */
static int _gend_error_exit(genders_t gp, char *msg);
static void *_safe_malloc(size_t size);
static void *_rangestr(hostlist_t hl, fmt_t fmt);
static char *_val_create(genders_t gp);
static char *_to_altname(genders_t gp, char *node);
#if 0
static char *_to_gendname(genders_t gp, char *val);
static char *_node_create(genders_t gp);
static char *_attr_create(genders_t gp);
#endif

int
main(int argc, char *argv[])
{
    int c;
    int ropt = 0, lopt = 0, qopt = 0, Copt = 0, vopt = 0;
    char *filename = DEFAULT_GENDERS_FILE;
    fmt_t qfmt = FMT_HOSTLIST;
    genders_t gp;

    while ((c = GETOPT(argc, argv, OPTIONS, longopts)) != EOF) {
        switch (c) {
        case 'c':   /* --querycomma */
            qfmt = FMT_COMMA;
            qopt = 1;
            break;
        case 'n':   /* --querynl */
            qfmt = FMT_NL;
            qopt = 1;
            break;
        case 's':   /* --queryspace */
            qfmt = FMT_SPACE;
            qopt = 1;
            break;
        case 'q':   /* --query */
            qfmt = FMT_HOSTLIST;
            qopt = 1;
            break;
        case 'v':   /* --value */
            vopt = 1;
            break;
        case 'l':   /* --listattr */
            lopt = 1;
            break;
        case 'C':   /* --cluster */
            Copt = 1;
            break;
        case 'r':   /* --altnames */
            ropt = 1;
            break;
        case 'f':   /* --filename */
            filename = optarg;
            break;
        default:
            usage();
            break;
        }
    }

    if (optind == argc && !Copt && !lopt)
        usage();

    /* Initialize genders package. */
    gp = genders_handle_create();
    if (!gp) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }
    if (genders_open(gp, filename) < 0)
        _gend_error_exit(gp, filename);

    /* Usage 1: list nodes with specified attribute. */
    if (qopt) {
        char *attr;

        if (vopt || Copt || lopt)
            usage();
        if (optind != argc - 1)
            usage();

        attr = argv[optind++];
        list_nodes(gp, attr, qfmt, ropt);

        exit(0);
    }

    /* Usage 2:  does node have attribute? */
    if (!Copt && !lopt) {
        char *node = NULL, *attr = NULL;
        int result;

        if (optind == argc - 2) {
            node = argv[optind++];
            attr = argv[optind++];
        } else if (optind == argc - 1) {
            node = NULL;
            attr = argv[optind++];
        } else
            usage();

        result = test_attr(gp, node, attr, vopt);
        exit(result ? 0 : 1);
    }

    /* Usage 3:  list attributes */
    if (lopt) {
        char *node = NULL;

        if (optind == argc - 1)
            node = argv[optind++];
        else if (optind != argc)
            usage();

        list_attrs(gp, node);
    }

    /* Usage 4:  list cluster name */
    if (Copt) {
        char *node = NULL;

        if (optind == argc - 1)
            node = argv[optind++];
        else if (optind != argc)
            usage();

        test_attr(gp, node, "cluster", 1);
        exit(0);
    }

    /*NOTREACHED*/
    exit(0);
}

static void list_nodes(genders_t gp, char *attr, fmt_t qfmt, int ropt)
{
    char **nodes;
    int i, count;
    int len = genders_nodelist_create(gp, &nodes);
    hostlist_t hl;
    char *val;
    char *str;

    if ((val = strchr(attr, '=')))  /* attr can actually be "attr=val" */
        *val++ ='\0';
    if ((count = genders_getnodes(gp, nodes, len, attr, val)) < 0)
        _gend_error_exit(gp, attr);

    /* Create a hostlist containing the list of nodes returned by the query */
    hl = hostlist_create(NULL);
    if (hl == NULL) {
        fprintf(stderr, "nodeattr: hostlist_create failed\n");
        exit(1);
    }
    for (i = 0; i < count; i++) {
        char *node = nodes[i];

        if (ropt)
            node = _to_altname(gp, nodes[i]);           
        if (hostlist_push(hl, node) == 0) {
            fprintf(stderr, "nodeattr: hostlist_push failed\n");
            exit(1);
        }
        if (ropt)
            free(node);
    }
    genders_nodelist_destroy(gp, nodes);

    str = _rangestr(hl, qfmt);
    if (strlen(str) > 0)
        printf("%s\n", str);
    free(str);
    hostlist_destroy(hl);
}

static int test_attr(genders_t gp, char *node, char *attr, int vopt)
{
    char *val = NULL;
    char *wantval;
    int res;

    if ((wantval = strchr(attr, '=')))  /* attr can actually be "attr=val" */
        *wantval++ ='\0';
   
    if (vopt || wantval)
        val = _val_create(gp); /* full of nulls initially */
    if ((res = genders_testattr(gp, node, attr, val, genders_getmaxvallen(gp))) < 0)
        _gend_error_exit(gp, "genders_testattr");
    if (vopt) {
        if (strlen(val) > 0)
            printf("%s\n", val);
    }
    if (wantval && strcmp(wantval, val) != 0)
        res = 0;
    if (vopt || wantval)
        free(val);
    return res;
}

static void list_attrs(genders_t gp, char *node)
{
    char **attrs, **vals;
    int len, vlen, count, i;

    len = genders_attrlist_create(gp, &attrs);
    vlen = genders_vallist_create(gp, &vals);
    if (node) {
        if ((count = genders_getattr(gp, attrs, vals, len, node)) < 0)
            _gend_error_exit(gp, "genders_getattr");
    } else {
        if ((count = genders_getattr_all(gp, attrs, len)) < 0)
            _gend_error_exit(gp, "genders_getattr_all");
    }
    for (i = 0; i < count; i++)
        if (node && strlen(vals[i]) > 0)
            printf("%s=%s\n", attrs[i], vals[i]);
        else
            printf("%s\n", attrs[i]);
    genders_attrlist_destroy(gp, attrs);
    genders_vallist_destroy(gp, vals);
}

static void usage(void)
{
    fprintf(stderr,
        "Usage: nodeattr [-f genders] [-q|-c|-n|-s] [-r] attr[=val]\n"
        "or     nodeattr [-f genders] [-v] [node] attr[=val]\n"
        "or     nodeattr [-f genders] -l [node]\n"
        "or     nodeattr [-f genders] -C [node]\n");
    exit(1);
}

/**
 ** Utility functions
 **/

static int _gend_error_exit(genders_t gp, char *msg)
{
    fprintf(stderr, "nodeattr: %s: %s\n", 
        msg, genders_strerror(genders_errnum(gp)));
    exit(1);
}

static void *_safe_malloc(size_t size)
{
    void *obj = malloc(size);

    if (obj == NULL) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }
    memset(obj, 0, size);
    return obj;
}

/* Create a host range string.  Caller must free result. */
static void *_rangestr(hostlist_t hl, fmt_t qfmt)
{
    int size = 65536;
    char *str = _safe_malloc(size);

    /* FIXME: hostlist functions are supposed to return -1 on truncation.
     * This doesn't seem to be working, so make initial size big enough.
     */
    if (qfmt == FMT_HOSTLIST) {
        while (hostlist_ranged_string(hl, size, str) < 0) {
            free(str); 
            size += size;
            str = (char *)_safe_malloc(size);
        }
    } else {
        char sep = qfmt == FMT_SPACE ? ' ' : qfmt == FMT_COMMA ? ',' : '\n';
        char *p;

        while (hostlist_deranged_string(hl, size, str) < 0) {
            free(str); 
            size += size;
            str = (char *)_safe_malloc(size);
        }
        for (p = str; p != NULL; ) {
            if ((p = strchr(p, ',')))
                *p++ = sep;
        }
    }
    return str;
}

/* Create a value string.  Caller must free result. */
static char *_val_create(genders_t gp)
{
    int maxvallen;
    char *val;

    if ((maxvallen = genders_getmaxvallen(gp)) < 0) 
        _gend_error_exit(gp, "genders_getmaxvallen");
    val = (char *)_safe_malloc(maxvallen + 1);
    return val;
}

/* Convert "altname" to "gendname".  Caller must free result. */
static char *_to_altname(genders_t gp, char *node)
{
    char *val = _val_create(gp); /* full of nulls initially */
   
    if (genders_testattr(gp, node, "altname", val, genders_getmaxvallen(gp)) < 0)
        _gend_error_exit(gp, "genders_testattr");
    if (strlen(val) == 0) {
        fprintf(stderr, "nodeattr: %s: conversion to alternate hostname failed\n", node);
        exit(1);
    }
    return val;
}


#if 0
/* Create a node string.  Caller must free result. */
static char *_node_create(genders_t gp)
{
    int maxnodelen;
    char *node;

    if ((maxnodelen = genders_getmaxnodelen(gp)) < 0) 
        _gend_error_exit(gp, "genders_getmaxnodelen");
    node = (char *)_safe_malloc(maxnodelen + 1);
    return node;
}

/* Create an attribute string.  Caller must free result. */
static char *_attr_create(genders_t gp)
{
    int maxattrlen;
    char *attr;

    if ((maxattrlen = genders_getmaxattrlen(gp)) < 0) 
        _gend_error_exit(gp, "genders_getmaxattrlen");
    attr = (char *)_safe_malloc(maxattrlen + 1);
    return attr;
}

/* Convert "altname" to "gendname". Caller must free result. */
static char *_to_gendname(genders_t gp, char *val)
{
    char **nodes;
    int count;
    int len = genders_nodelist_create(gp, &nodes);
    char *node = NULL;

    if ((count = genders_getnodes(gp, nodes, len, "altname", val)) < 0) {
        genders_nodelist_destroy(gp, nodes);
        _gend_error_exit(gp, val);
    } 
    if (count > 1)
        fprintf(stderr, "nodeattr: altname=%s appears more than once!\n", val);

    if (count == 1) {
        node = _node_create(gp);
        strcpy(node, nodes[0]);
    }
    genders_nodelist_destroy(gp, nodes);

    return node;
}
#endif

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
