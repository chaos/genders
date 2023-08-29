/*****************************************************************************\
 *  $Id: nodeattr.c,v 1.42 2010-02-02 00:04:34 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2001-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Jim Garlick <garlick@llnl.gov> and Albert Chu <chu11@llnl.gov>.
 *  UCRL-CODE-2003-004.
 *
 *  This file is part of Genders, a cluster configuration database.
 *  For details, see <https://github.com/chaos/genders>.
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
 *  with Genders.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if HAVE_GETOPT_H
#include <getopt.h>
#endif /* HAVE_GETOPT_H */
#include <errno.h>

#include "genders.h"
#include "hash.h"
#include "hostlist.h"
#include "list.h"

#if HAVE_GETOPT_LONG
#define GETOPT(ac,av,opt,lopt) getopt_long(ac,av,opt,lopt,NULL)
#else
#define GETOPT(ac,av,opt,lopt) getopt(ac,av,opt)
#endif

#define OPTIONS "cnsqX:AvQVUlf:kd:eCH"

/* an impossible attribute */
#define NOATTRSFLAG "=,,=,,=,,=,,="

#if HAVE_GETOPT_LONG
static struct option longopts[] = {
    { "querycomma", 0, 0, 'c' },
    { "querynl", 0, 0, 'n' },
    { "queryspace", 0, 0, 'n' },
    { "query", 0, 0, 'q' },
    { "excludequery", 1, 0, 'X'},
    { "allnodes", 0, 0, 'A' },
    { "value", 0, 0, 'v' },
    { "testquery", 0, 0, 'Q' },
    { "values", 0, 0, 'V' },
    { "unique", 0, 0, 'U' },
    { "listattr", 0, 0, 'l' },
    { "filename", 1, 0, 'f' },
    { "parse-check", 0, 0, 'k'},
    { "diff", 1, 0, 'd'},
    { "expand", 0, 0, 'e'},
    { "compress", 0, 0, 'C'},
    { "compress-attrs", 0, 0, 'C'},
    { "compress-hosts", 0, 0, 'H'},
    { 0,0,0,0 },
};
#endif

typedef enum { FMT_COMMA, FMT_NL, FMT_SPACE, FMT_HOSTLIST } fmt_t;

static int test_attr(genders_t gp, char *node, char *attr, int vopt);
static int test_query(genders_t gp, char *node, char *query);
static void list_attr_val(genders_t gp, char *attr, int Uopt);
static void list_nodes(genders_t gp, char *attr, char *excludequery, fmt_t fmt);
static void list_attrs(genders_t gp, char *node);
static void usage(void);
static void diff_genders(char *db1, char *db2);
static void expand(genders_t gp);
static void compress(genders_t gp);
static void compress_hosts(genders_t gp);

/* Utility functions */
static int _gend_error_exit(genders_t gp, char *msg);
static void *_safe_malloc(size_t size);
static void *_rangestr(hostlist_t hl, fmt_t fmt);
static char *_val_create(genders_t gp);
#if 0
static char *_to_gendname(genders_t gp, char *val);
static char *_node_create(genders_t gp);
static char *_attr_create(genders_t gp);
#endif

#define HOSTLIST_BUFLEN 1024

int
main(int argc, char *argv[])
{
    int c, errors;
    int Aopt = 0, lopt = 0, qopt = 0, Xopt = 0, vopt = 0, Qopt = 0,
      Vopt = 0, Uopt = 0, kopt = 0, dopt = 0, eopt = 0, Copt = 0, Hopt = 0;
    char *filename = GENDERS_DEFAULT_FILE;
    char *dfilename = NULL;
    char *excludequery = NULL;
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
        case 'X':   /* --excludequery */
            excludequery = optarg;
            Xopt = 1;
            break;
        case 'A':   /* --allnodes */
            Aopt = 1;
            break;
        case 'v':   /* --value */
            vopt = 1;
            break;
        case 'Q':   /* --testquery */
            Qopt = 1;
            break;
        case 'V':   /* --values */
            Vopt = 1;
            break;
        case 'U':   /* --unique */
            Uopt = 1;
            break;
        case 'l':   /* --listattr */
            lopt = 1;
            break;
        case 'f':   /* --filename */
            filename = optarg;
            break;
        case 'k':   /* --check */
            kopt = 1;
            break;
        case 'd':   /* --diff */
            dopt = 1;
            dfilename = optarg;
            break;
        case 'e':   /* --expand */
            eopt = 1;
            break;
        case 'C':   /* --compress or --compress-attrs */
            Copt = 1;
            break;
        case 'H':   /* --compress-hosts */
            Hopt = 1;
            break;
        default:
            usage();
            break;
        }
    }

    /* check parameter inputs */

    /* specify correct option combinations */
    if ((qopt + Qopt + Vopt + lopt + kopt + dopt + eopt + Copt + Hopt) > 1)
        usage();

    if ((qopt
         || Qopt
         || Vopt
         || lopt
         || kopt
         || dopt
         || eopt
         || Copt
         || Hopt)
        && vopt)
        usage();

    if (Aopt && !qopt) {
        qfmt = FMT_HOSTLIST;
        qopt = 1;
    }

    if (!qopt && Xopt)
        usage();

    if (!Vopt && Uopt)
        usage();

    /* specified correctly number of arguments */
    if ((qopt
         && ((!Aopt && optind != (argc - 1))
             || (Aopt && optind != argc)))
        || (!qopt
            && !Qopt
            && !Vopt
            && !lopt
            && !kopt
            && !dopt
            && !eopt
            && !Copt
            && !Hopt
            && (optind != (argc - 1) && optind != (argc - 2)))
        || (Qopt && (optind != (argc - 1) && optind != (argc - 2)))
        || (Vopt && optind != (argc - 1))
        || (lopt && (optind != argc && optind != (argc - 1)))
        || (kopt && optind != argc)
        || (dopt && optind != argc)
        || (eopt && optind != argc)
        || (Copt && optind != argc)
        || (Hopt && optind != argc))
        usage();

    /* genders database diff */
    if (dopt) {
        diff_genders(filename, dfilename);
        exit(0);
    }

    /* Initialize genders package. */
    gp = genders_handle_create();
    if (!gp) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }

    /* parse check */
    if (kopt) {
        errors = genders_parse(gp, filename, NULL);
        if (errors == -1 && genders_errnum(gp) != GENDERS_ERR_PARSE)
            _gend_error_exit(gp, "genders_parse");
        if (errors >= 0)
            fprintf(stderr, "nodeattr: %d parse errors discovered\n", errors);
        exit(errors);
    }

    if (genders_load_data(gp, filename) < 0)
        _gend_error_exit(gp, filename);

    /* expand */
    if (eopt) {
        expand(gp);
        exit(0);
    }

    /* compress or compress-attrs */
    if (Copt) {
        compress(gp);
        exit(0);
    }

    /* compress-hosts */
    if (Hopt) {
        compress_hosts(gp);
        exit(0);
    }

    /* Usage 1: list nodes with specified attribute, or all nodes */
    if (qopt) {
        char *query;

        if (Aopt)
            list_nodes(gp, NULL, excludequery, qfmt);
        else {
            query = argv[optind++];
            list_nodes(gp, query, excludequery, qfmt);
        }

        exit(0);
    }

    /* Usage 2:  does node have attribute? */
    if (!qopt && !Qopt && !Vopt && !lopt && !kopt && !dopt) {
        char *node = NULL, *attr = NULL;
        int result;

        if (optind == argc - 2) {
            node = argv[optind++];
            attr = argv[optind++];
        } else {
            node = NULL;
            attr = argv[optind++];
        }

        result = test_attr(gp, node, attr, vopt);
        exit(result ? 0 : 1);
    }

    /* Usage 3:  does node meet query conditions */
    if (Qopt) {
        char *node = NULL, *query = NULL;
        int result;

        if (optind == argc - 2) {
            node = argv[optind++];
            query = argv[optind++];
        } else {
            node = NULL;
            query = argv[optind++];
        }

        result = test_query(gp, node, query);
        exit(result ? 0 : 1);
    }

    /* Usage 4:  output all attribute values */
    if (Vopt) {
        char *attr = NULL;

        attr = argv[optind++];

        if (strchr(attr, '='))  /* attr cannot be "attr=val" */
            usage();

        list_attr_val(gp, attr, Uopt);
    }

    /* Usage 5:  list attributes */
    if (lopt) {
        char *node = NULL;

        if (optind == argc - 1)
            node = argv[optind++];

        list_attrs(gp, node);
    }

    /*NOTREACHED*/
    exit(0);
}

static void
list_nodes(genders_t gp, char *query, char *excludequery, fmt_t qfmt)
{
    char **nodes;
    int i, count;
    int len;
    hostlist_t hl;
    char *str;

    if ((len = genders_nodelist_create(gp, &nodes)) < 0)
        _gend_error_exit(gp, "genders_nodelist_create");

    if ((count = genders_query(gp, nodes, len, query)) < 0)
        _gend_error_exit(gp, query);

    /* Create a hostlist containing the list of nodes returned by the query */
    hl = hostlist_create(NULL);
    if (hl == NULL) {
        fprintf(stderr, "nodeattr: hostlist_create failed\n");
        exit(1);
    }
    for (i = 0; i < count; i++) {
        if (hostlist_push(hl, nodes[i]) == 0) {
            fprintf(stderr, "nodeattr: hostlist_push failed\n");
            exit(1);
        }
    }

    if (excludequery) {
        genders_nodelist_clear(gp, nodes);

        if ((count = genders_query(gp, nodes, len, excludequery)) < 0)
            _gend_error_exit(gp, excludequery);

        /* Do not check return code for == 0, node may not exist in hostlist */
        for (i = 0; i < count; i++)
            hostlist_delete(hl, nodes[i]);
    }

    genders_nodelist_destroy(gp, nodes);

    hostlist_sort(hl);
    str = _rangestr(hl, qfmt);
    if (strlen(str) > 0)
        printf("%s\n", str);
    free(str);
    hostlist_destroy(hl);
}

static int
test_attr(genders_t gp, char *node, char *attr, int vopt)
{
    char *val = NULL;
    char *wantval;
    int res;

    if ((wantval = strchr(attr, '=')))  /* attr can actually be "attr=val" */
        *wantval++ ='\0';

    if (vopt || wantval)
        val = _val_create(gp); /* full of nulls initially */

    if ((res = genders_testattr(gp, node, attr, val, genders_getmaxvallen(gp) + 1)) < 0)
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

static int
test_query(genders_t gp, char *node, char *query)
{
    int res;

    if ((res = genders_testquery(gp, node, query)) < 0)
        _gend_error_exit(gp, "genders_testquery");

    return res;
}

static void
list_attr_val(genders_t gp, char *attr, int Uopt)
{
    char **nodes, **myvallist;
    char *val;
    int maxvallen, nlen, ncount, i, ret;
    unsigned int val_count = 0;

    /* achu: There is currently no library operation that offers
     * anything to easily access this information.  So we have to
     * iterate to do it.
     */

    if ((nlen = genders_nodelist_create(gp, &nodes)) < 0)
        _gend_error_exit(gp, "genders_getnodelist_create");

    if ((ncount = genders_getnodes(gp, nodes, nlen, attr, NULL)) < 0)
        _gend_error_exit(gp, "genders_getnodes");

    myvallist = (char **)_safe_malloc(ncount * sizeof(char **));
    for (i = 0; i < ncount; i++)
        myvallist[i] = _val_create(gp);

    val = _val_create(gp); /* full of nulls initially */

    if ((maxvallen = genders_getmaxvallen(gp)) < 0)
        _gend_error_exit(gp, "genders_getmaxvallen");

    for (i = 0; i < ncount; i++) {
        memset(val, '\0', maxvallen + 1);
        if ((ret = genders_testattr(gp,
                                    nodes[i],
                                    attr,
                                    val,
                                    maxvallen + 1)) < 0)
            _gend_error_exit(gp, "genders_testattr");
        if (ret && strlen(val)) {
            int j, store = 0;
            if (Uopt) {
                /* achu: I know this is inefficient.  I don't have
                   good data structures around to make things
                   better/easier.
                 */
                for (j = 0; j < val_count; j++) {
                    if (!strcmp(val, myvallist[j])) {
                        store++;
                        break;
                    }
                }
            }
            if (!store) {
                strcpy(myvallist[val_count], val);
                val_count++;
            }
        }
    }

    for (i = 0; i < val_count; i++) {
        printf("%s\n", myvallist[i]);
    }

    genders_nodelist_destroy(gp, nodes);
    for (i = 0; i < ncount; i++)
        free(myvallist[i]);
    free(myvallist);
    free(val);
}

static void
usage(void)
{
    fprintf(stderr,
        "Usage: nodeattr [-f genders] [-q|-c|-n|-s] [-X exclude_query] query\n"
        "or     nodeattr [-f genders] [-q|-c|-n|-s] -A\n"
        "or     nodeattr [-f genders] [-v] [node] attr[=val]\n"
        "or     nodeattr [-f genders] -Q [node] query\n"
        "or     nodeattr [-f genders] -V [-U] attr\n"
        "or     nodeattr [-f genders] -l [node]\n"
        "or     nodeattr [-f genders] -k\n"
        "or     nodeattr [-f genders] -d genders\n"
        "or     nodeattr [-f genders] --expand\n"
        "or     nodeattr [-f genders] --compress\n"
        "or     nodeattr [-f genders] --compress-hosts\n"
            );
    exit(1);
}

static int
_diff(genders_t gh, genders_t dgh, char *filename, char *dfilename)
{
    char **nodes = NULL, **dnodes = NULL;
    int maxnodes, dmaxnodes, numnodes, dnumnodes;
    char **attrs = NULL, **dattrs = NULL;
    int maxattrs, dmaxattrs, numattrs, dnumattrs;
    char **vals = NULL, **dvals = NULL, *dvalbuf = NULL;
    int maxvals, dmaxvals, dmaxvallen;
    int i, j, rv, errcount = 0;

    /* Test #1: Determine if nodes match */

    if ((maxnodes = genders_nodelist_create(gh, &nodes)) < 0)
        _gend_error_exit(gh, "genders_nodelist_create");

    if ((numnodes = genders_getnodes(gh, nodes, maxnodes, NULL, NULL)) < 0)
        _gend_error_exit(gh, "genders_getnodes");

    if ((dmaxnodes = genders_nodelist_create(dgh, &dnodes)) < 0)
        _gend_error_exit(gh, "genders_nodelist_create");

    if ((dnumnodes = genders_getnodes(dgh, dnodes, dmaxnodes, NULL, NULL)) < 0)
        _gend_error_exit(dgh, "genders_getnodes");

    for (i = 0; i < numnodes; i++) {
        if ((rv = genders_isnode(dgh, nodes[i])) < 0)
            _gend_error_exit(dgh, "genders_isnode");

        if (!rv) {
            fprintf(stderr, "%s: Node \"%s\" does not exist\n", dfilename, nodes[i]);
            errcount++;
        }
    }

    for (i = 0; i < dnumnodes; i++) {
        if ((rv = genders_isnode(gh, dnodes[i])) < 0)
            _gend_error_exit(gh, "genders_isnode");

        if (!rv) {
            fprintf(stderr, "%s: Contains additional node \"%s\"\n", dfilename, dnodes[i]);
            errcount++;
        }
    }

    /* Test #2: Determine if attributes match */

    if ((maxattrs = genders_attrlist_create(gh, &attrs)) < 0)
        _gend_error_exit(gh, "genders_attrlist_create");

    if ((dmaxattrs = genders_attrlist_create(dgh, &dattrs)) < 0)
        _gend_error_exit(dgh, "genders_attrlist_create");

    if ((numattrs = genders_getattr_all(gh, attrs, maxattrs)) < 0)
        _gend_error_exit(gh, "genders_getattr_all");

    if ((dnumattrs = genders_getattr_all(dgh, dattrs, dmaxattrs)) < 0)
        _gend_error_exit(dgh, "genders_getattr_all");

    for (i = 0; i < numattrs; i++) {
        if ((rv = genders_isattr(dgh, attrs[i])) < 0)
            _gend_error_exit(dgh, "genders_isattr");

        if (!rv) {
            fprintf(stderr, "%s: Attribute \"%s\" does not exist\n", dfilename, attrs[i]);
            errcount++;
        }
    }

    for (i = 0; i < dnumattrs; i++) {
        if ((rv = genders_isattr(gh, dattrs[i])) < 0)
            _gend_error_exit(gh, "genders_isattr");

        if (!rv) {
            fprintf(stderr, "%s: Contains additional attribute \"%s\"\n", dfilename, dattrs[i]);
            errcount++;
        }
    }

    /* Test #3: For each node, are the attributes and values identical */

    if ((maxvals = genders_vallist_create(gh, &vals)) < 0)
        _gend_error_exit(gh, "genders_vallist_create");

    if ((dmaxvals = genders_vallist_create(dgh, &dvals)) < 0)
        _gend_error_exit(dgh, "genders_vallist_create");

    if ((dmaxvallen = genders_getmaxvallen(dgh)) < 0)
        _gend_error_exit(dgh, "genders_maxvallen");

    if (!(dvalbuf = malloc(dmaxvallen + 1))) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }

    for (i = 0; i < numnodes; i++) {

        /* Don't bother if the node doesn't exist, this issue has been
         * output already
         */
        if ((rv = genders_isnode(dgh, nodes[i])) < 0)
            _gend_error_exit(dgh, "genders_isnode");

        if (!rv)
            continue;

        if (genders_attrlist_clear(gh, attrs) < 0)
            _gend_error_exit(gh, "genders_attrlist_clear");

        if (genders_vallist_clear(gh, vals) < 0)
            _gend_error_exit(gh, "genders_vallist_clear");

        if (genders_attrlist_clear(dgh, dattrs) < 0)
            _gend_error_exit(dgh, "genders_attrlist_clear");

        if (genders_vallist_clear(dgh, dvals) < 0)
            _gend_error_exit(dgh, "genders_vallist_clear");

        if ((numattrs = genders_getattr(gh,
                                        attrs,
                                        vals,
                                        maxattrs,
                                        nodes[i])) < 0)
          _gend_error_exit(gh, "genders_getattr");

        for (j = 0; j < numattrs; j++) {

            /* Don't bother if the attribute doesn't exist, this issue
             * has been output already
             */
            if ((rv = genders_isattr(dgh, attrs[j])) < 0)
                _gend_error_exit(dgh, "genders_isattr");

            if (!rv)
                continue;

            memset(dvalbuf, '\0', dmaxvallen + 1);

            if ((rv = genders_testattr(dgh,
                                       nodes[i],
                                       attrs[j],
                                       dvalbuf,
                                       dmaxvallen + 1)) < 0)
                _gend_error_exit(dgh, "genders_testattr");

            if (!rv) {
                fprintf(stderr, "%s: Node \"%s\" does not "
                        "contain attribute \"%s\"\n",
                        dfilename, nodes[i], attrs[j]);
                errcount++;
                continue;
            }

            if (strlen(vals[j])) {
                if (strcmp(vals[j], dvalbuf)) {
                    if (strlen(dvalbuf)) {
                        fprintf(stderr, "%s: Node \"%s\", attribute \"%s\" has "
                                "a different value \"%s\"\n",
                                dfilename, nodes[i], attrs[j], dvalbuf);
                    }
                    else {
                        fprintf(stderr, "%s: Node \"%s\", attribute \"%s\" does "
                                "not have a value\n",
                                dfilename, nodes[i], attrs[j]);
                    }
                    errcount++;
                    continue;
                }
            }
            else {
                if (strlen(dvalbuf)) {
                    fprintf(stderr, "%s: Node \"%s\", attribute \"%s\" has "
                            "a value \"%s\"\n",
                            dfilename, nodes[i], attrs[j], dvalbuf);
                    errcount++;
                    continue;
                }
            }
        }

        /* There is no need to compare attribute values for the reverse
         * case.  Only for existence of attributes.
         */

        if ((dnumattrs = genders_getattr(dgh,
                                         dattrs,
                                         dvals,
                                         dmaxattrs,
                                         nodes[i])) < 0)
          _gend_error_exit(dgh, "genders_getattr");

        for (j = 0; j < dnumattrs; j++) {

            /* Don't bother if the attribute doesn't exist, this issue
             * has been output already
             */
            if ((rv = genders_isattr(gh, dattrs[j])) < 0)
                _gend_error_exit(dgh, "genders_isattr");

            if (!rv)
                continue;

            if ((rv = genders_testattr(gh,
                                       nodes[i],
                                       dattrs[j],
                                       NULL,
                                       0)) < 0)
                _gend_error_exit(gh, "genders_testattr");

            if (!rv) {
                if (strlen(dvals[j])) {
                    fprintf(stderr, "%s: Node \"%s\" contains "
                            "an additional attribute value pair \"%s=%s\"\n",
                            dfilename, nodes[i], dattrs[j], dvals[j]);
                }
                else {
                    fprintf(stderr, "%s: Node \"%s\" contains "
                            "an additional attribute \"%s\"\n",
                            dfilename, nodes[i], dattrs[j]);
                }
                errcount++;
                continue;
            }
        }
    }

    (void)genders_nodelist_destroy(gh, nodes);
    (void)genders_nodelist_destroy(dgh, dnodes);
    (void)genders_attrlist_destroy(gh, attrs);
    (void)genders_attrlist_destroy(dgh, dattrs);
    (void)genders_vallist_destroy(gh, vals);
    (void)genders_vallist_destroy(dgh, dvals);
    free(dvalbuf);
    return errcount;
}

static void
diff_genders(char *filename, char *dfilename)
{
    genders_t gh, dgh;

    gh = genders_handle_create();
    if (!gh) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }

    dgh = genders_handle_create();
    if (!dgh) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }

    if (genders_load_data(gh, filename) < 0)
        _gend_error_exit(gh, filename);

    if (genders_load_data(dgh, dfilename) < 0)
        _gend_error_exit(dgh, dfilename);

    if (_diff(gh, dgh, filename, dfilename) != 0)
        return;
}

static void
expand(genders_t gp)
{
    char **nodes, **attrs, **vals;
    int nodeslen, attrslen, valslen;
    int nodescount, attrscount;
    unsigned int maxnodenamelen = 0;
    hostlist_t hl = NULL;
    hostlist_iterator_t hlitr = NULL;
    char *node;
    int i, j;

    if ((nodeslen = genders_nodelist_create(gp, &nodes)) < 0)
        _gend_error_exit(gp, "genders_nodelist_create");

    if ((attrslen = genders_attrlist_create(gp, &attrs)) < 0)
        _gend_error_exit(gp, "genders_attrlist_create");

    if ((valslen = genders_vallist_create(gp, &vals)) < 0)
        _gend_error_exit(gp, "genders_vallist_create");

    if ((nodescount = genders_getnodes(gp, nodes, nodeslen, NULL, NULL)) < 0)
        _gend_error_exit(gp, "genders_getnodes");

    /* We use the hostlist as a cheap mechanism to sort the node names
     * before outputting them
     */

    if (!(hl = hostlist_create(NULL))) {
        fprintf(stderr, "hostlist_create: %s\n", strerror(errno));
        exit(1);
    }

    for (i = 0; i < nodescount; i++) {
        unsigned int tmp = strlen(nodes[i]);
        if (tmp > maxnodenamelen) {
            maxnodenamelen = tmp;
        }

        if (!hostlist_push(hl, nodes[i])) {
            fprintf(stderr, "hostlist_push: %s\n", strerror(errno));
            exit(1);
        }
    }

    hostlist_sort(hl);

    if (!(hlitr = hostlist_iterator_create(hl))) {
        fprintf(stderr, "hostlist_iterator_create: %s\n", strerror(errno));
        exit(1);
    }

    while ((node = hostlist_next(hlitr))) {
        if (genders_attrlist_clear(gp, attrs) < 0)
            _gend_error_exit(gp, "genders_attrlist_clear");

        if (genders_vallist_clear(gp, vals) < 0)
            _gend_error_exit(gp, "genders_vallist_clear");

        if ((attrscount = genders_getattr(gp, attrs, vals, attrslen, node)) < 0)
            _gend_error_exit(gp, "genders_getattr");

        printf("%s", node);
        if (attrscount) {
            unsigned int numspace = maxnodenamelen - strlen(node);
            for (j = 0; j < numspace; j++)
                printf(" ");
            printf(" ");
        }

        for (j = 0 ; j < attrscount; j++) {
            if (j)
                printf(",");

            if (strlen(vals[j]))
                printf("%s=%s", attrs[j], vals[j]);
            else
                printf("%s", attrs[j]);
        }

        printf("\n");
        free(node);
    }

    genders_nodelist_destroy(gp, nodes);
    genders_attrlist_destroy(gp, attrs);
    genders_vallist_destroy(gp, vals);
    hostlist_destroy(hl);
}

struct hosts_data {
    char *key;
    hostlist_t hl;
};

struct attr_list {
    char *hostrange;
    List l;
};

struct store_hostrange_data {
    List hlist;
    unsigned int maxhostrangelen;
};

static void
_hosts_data_del(void *data)
{
    struct hosts_data *hd = (struct hosts_data *)data;

    free(hd->key);
    hostlist_destroy(hd->hl);
    free(hd);
}

static int
_print_key(void *data, const void *key, void *arg)
{
    printf("%s\n", (char *)key);
    return(0);
}

static void
_hash_attrval(hash_t hattr, char *node, char *attr, char *val)
{
    struct hosts_data *hd = NULL;
    char *hashkey = NULL;
    int keylen, attrlen, vallen;

    assert(hattr && node && attr && val);

    attrlen = strlen(attr);
    vallen = strlen(val);
    keylen = attrlen + vallen;

    /* for equal sign */
    if (vallen)
        keylen++;

    /* for NUL char */
    keylen++;

    if (!(hashkey = (char *)malloc(keylen))) {
        fprintf(stderr, "malloc: %s\n", strerror(errno));
        exit(1);
    }

    if (vallen)
        snprintf(hashkey, keylen, "%s=%s", attr, val);
    else
        snprintf(hashkey, keylen, "%s", attr);

    if (!(hd = hash_find(hattr, hashkey))) {
        if (!(hd = (struct hosts_data *)malloc(sizeof(struct hosts_data)))) {
            fprintf(stderr, "malloc: %s\n", strerror(errno));
            exit(1);
        }

        hd->key = hashkey;
        if (!(hd->hl = hostlist_create(NULL))) {
            fprintf(stderr, "hostlist_create: %s\n", strerror(errno));
            exit(1);
        }

        if (!hash_insert(hattr, hd->key, hd)) {
            fprintf(stderr, "hash_insert: %s\n", strerror(errno));
            exit(1);
        }
    }
    else
        free(hashkey);

    if (!hostlist_push(hd->hl, node)) {
        fprintf(stderr, "hostlist_push: %s\n", strerror(errno));
        exit(1);
    }
}

static void
list_attrs(genders_t gp, char *node)
{
    char **attrs, **vals;
    char *anode = NULL;
    int numattrs, len, vlen, count, i;
    hash_t hattrval = NULL;
    hostlist_t nodelist;

    if ((len = genders_attrlist_create(gp, &attrs)) < 0)
        _gend_error_exit(gp, "genders_attrlist_create");
    if ((vlen = genders_vallist_create(gp, &vals)) < 0)
        _gend_error_exit(gp, "genders_vallist_create");
    if ((numattrs = genders_getnumattrs(gp)) < 0)
        _gend_error_exit(gp, "genders_getnumattrs");

    /* numattrs + 1, in case numattrs == 0
     *
     * (numattrs + 1) * 4, is an estimate on attribute=value pair
     * types, b/c we are keying off attr=val pairs, not just the
     * attribute name.
     */
    if (!(hattrval = hash_create((numattrs + 1)*4,
                            (hash_key_f)hash_key_string,
                            (hash_cmp_f)strcmp,
                            _hosts_data_del))) {
        fprintf(stderr, "hash_create: %s\n", strerror(errno));
        exit(1);
    }

    /* a single node or a list of nodes */
    if (node) {
        nodelist = hostlist_create(node);
        while ((anode = hostlist_pop(nodelist))) {
            if ((count = genders_getattr(gp, attrs, vals, len, anode)) < 0)
                _gend_error_exit(gp, "genders_getattr");
            for (i = 0; i < count; i++)
                _hash_attrval(hattrval, anode, attrs[i], vals[i]);
            free(anode);
        }

        if (hash_for_each(hattrval, _print_key, NULL) < 0) {
            fprintf(stderr, "hash_for_each: %s\n", strerror(errno));
            exit(1);
        }
        hostlist_destroy(nodelist);
    /* all nodes */
    } else {
        if ((count = genders_getattr_all(gp, attrs, len)) < 0)
            _gend_error_exit(gp, "genders_getattr_all");
        for (i = 0; i < count; i++)
            printf("%s\n", attrs[i]);
    }
    genders_attrlist_destroy(gp, attrs);
    genders_vallist_destroy(gp, vals);
    hash_destroy(hattrval);
}

static void
_attr_list_del(void *data)
{
    struct attr_list *al = (struct attr_list *)data;

    free(al->hostrange);
    list_destroy(al->l);
    free(al);
}

static int
_hash_hostrange(void *data, const void *key, void *arg)
{
    struct hosts_data *hd = (struct hosts_data *)data;
    hash_t *hrange = (hash_t *)arg;
    char hostrange[HOSTLIST_BUFLEN + 1];
    struct attr_list *al;

    memset(hostrange, '\0', HOSTLIST_BUFLEN + 1);

    hostlist_sort(hd->hl);

    if (hostlist_ranged_string(hd->hl, HOSTLIST_BUFLEN, hostrange) < 0) {
        fprintf(stderr, "hostlist_ranged_string: %s\n", strerror(errno));
        exit(1);
    }

    if (!(al = hash_find(*hrange, hostrange))) {
        if (!(al = (struct attr_list *)malloc(sizeof(struct attr_list)))) {
            fprintf(stderr, "malloc: %s\n", strerror(errno));
            exit(1);
        }

        if (!(al->hostrange = strdup(hostrange))) {
            fprintf(stderr, "strdup: %s\n", strerror(errno));
            exit(1);
        }

        if (!(al->l = list_create(NULL))) {
            fprintf(stderr, "list_create: %s\n", strerror(errno));
            exit(1);
        }

        if (!hash_insert(*hrange, al->hostrange, al)) {
            fprintf(stderr, "hash_insert: %s\n", strerror(errno));
            exit(1);
        }
    }

    if (!list_append(al->l, hd->key)) {
        fprintf(stderr, "list_append: %s\n", strerror(errno));
        exit(1);
    }

    return 0;
}

static int
_store_hostrange(void *data, const void *key, void *arg)
{
    struct attr_list *al = (struct attr_list *)data;
    struct store_hostrange_data *shd = (struct store_hostrange_data *)arg;
    unsigned int len;

    if (!list_append(shd->hlist, al)) {
        fprintf(stderr, "list_append: %s\n", strerror(errno));
        exit(1);
    }

    len = strlen(al->hostrange);
    if (len > shd->maxhostrangelen)
        shd->maxhostrangelen = len;

    return 0;
}

static int
_hostrange_cmp(void *x, void *y)
{
    struct attr_list *al1 = (struct attr_list *)x;
    struct attr_list *al2 = (struct attr_list *)y;

    if (strlen(al1->hostrange) < strlen(al2->hostrange))
        return 1;
    else if (strlen(al1->hostrange) > strlen(al2->hostrange))
        return -1;
    else
        return 0;
}

static int
_output_hostrange(void *x, void *arg)
{
    struct attr_list *al = (struct attr_list *)x;
    unsigned int maxhostrangelen = *(unsigned int *)arg;
    char *attrval;
    ListIterator litr;
    int lcount, count = 0;
    unsigned int numspace;
    int i;

    printf("%s", al->hostrange);
    numspace = maxhostrangelen - strlen(al->hostrange);
    for (i = 0; i < numspace; i++)
        printf(" ");
    printf(" ");

    lcount = list_count(al->l);

    if (!(litr = list_iterator_create(al->l))) {
        fprintf(stderr, "list_iterator_create: %s\n", strerror(errno));
        exit(1);
    }

    while ((attrval = list_next(litr))) {

        if (!strcmp(attrval, NOATTRSFLAG))
            continue;

        printf("%s", attrval);
        count++;
        if (lcount != count)
            printf(",");
    }

    printf("\n");

    list_iterator_destroy(litr);
    return 0;
}

static void
compress(genders_t gp)
{
    char **nodes, **attrs, **vals;
    int nodeslen, attrslen, valslen;
    int nodescount, attrscount;
    int numnodes, numattrs;
    hash_t hattr = NULL;
    hash_t hrange = NULL;
    List hlist = NULL;
    struct store_hostrange_data shd;
    int i, j;

    /* The basic idea behind this algorithm is that we will find every
     * host that contains an attr or attr=val combination.
     *
     * Then, we will find every attr or attr=val combination with the
     * same sets of hosts, than output a compressed hostrange output
     * for those hosts with every appropriate attr/attr=val.
     */

    /* need to treat values w/ raw inputs in order to compress */
    if (genders_set_flags(gp, GENDERS_FLAG_RAW_VALUES) < 0)
        _gend_error_exit(gp, "genders_set_flags");

    if ((numnodes = genders_getnumnodes(gp)) < 0)
        _gend_error_exit(gp, "genders_getnumnodes");

    if ((numattrs = genders_getnumattrs(gp)) < 0)
        _gend_error_exit(gp, "genders_getnumattrs");

    /* numattrs + 1, in case numattrs == 0
     *
     * (numattrs + 1) * 4, is an estimate on attribute=value pair
     * types, b/c we are keying off attr=val pairs, not just the
     * attribute name.
     */
    if (!(hattr = hash_create((numattrs + 1)*4,
                          (hash_key_f)hash_key_string,
                          (hash_cmp_f)strcmp,
                          _hosts_data_del))) {
        fprintf(stderr, "hash_create: %s\n", strerror(errno));
        exit(1);
    }

    if ((nodeslen = genders_nodelist_create(gp, &nodes)) < 0)
        _gend_error_exit(gp, "genders_nodelist_create");

    if ((attrslen = genders_attrlist_create(gp, &attrs)) < 0)
        _gend_error_exit(gp, "genders_attrlist_create");

    if ((valslen = genders_vallist_create(gp, &vals)) < 0)
        _gend_error_exit(gp, "genders_vallist_create");

    if ((nodescount = genders_getnodes(gp, nodes, nodeslen, NULL, NULL)) < 0)
        _gend_error_exit(gp, "genders_getnodes");

    for (i = 0; i < nodescount; i++) {
        if (genders_attrlist_clear(gp, attrs) < 0)
            _gend_error_exit(gp, "genders_attrlist_clear");

        if (genders_vallist_clear(gp, vals) < 0)
            _gend_error_exit(gp, "genders_vallist_clear");

        if ((attrscount = genders_getattr(gp, attrs, vals, attrslen, nodes[i])) < 0)
            _gend_error_exit(gp, "genders_getattr");

        if (!attrscount) {
            _hash_attrval(hattr, nodes[i], NOATTRSFLAG, "");
            continue;
        }

        for (j = 0 ; j < attrscount; j++)
            _hash_attrval(hattr, nodes[i], attrs[j], vals[j]);
    }

    /* Now, find all the common attributes for a particular hostrange */

    if (!(hrange = hash_create(numnodes,
                              (hash_key_f)hash_key_string,
                              (hash_cmp_f)strcmp,
                              _attr_list_del))) {
        fprintf(stderr, "hash_create: %s\n", strerror(errno));
        exit(1);
    }

    if (hash_for_each(hattr, _hash_hostrange, &hrange) < 0) {
        fprintf(stderr, "hash_for_each: %s\n", strerror(errno));
        exit(1);
    }

    if (!(hlist = list_create(NULL))) {
        fprintf(stderr, "list_create: %s\n", strerror(errno));
        exit(1);
    }

    shd.hlist = hlist;
    shd.maxhostrangelen = 0;

    if (hash_for_each(hrange, _store_hostrange, &shd) < 0) {
        fprintf(stderr, "hash_for_each: %s\n", strerror(errno));
        exit(1);
    }

    list_sort(hlist, _hostrange_cmp);

    if (list_for_each(hlist, _output_hostrange, &shd.maxhostrangelen) < 0) {
        fprintf(stderr, "list_for_each: %s\n", strerror(errno));
        exit(1);
    }

    genders_nodelist_destroy(gp, nodes);
    genders_attrlist_destroy(gp, attrs);
    genders_vallist_destroy(gp, vals);
    hash_destroy(hattr);
    hash_destroy(hrange);
    list_destroy(hlist);
}

static void
_hash_allattrvals(hash_t hattr, const char *node, const char *attrsvals)
{
    struct hosts_data *hd = NULL;
    char *hashkey = NULL;

    /* Insert NODE into the hostlist of HATTR's key ATTRSVALS,
       creating the entry if needed.  */

    /* An empty attr will come in as NOATTRSFLAG, attrsvals will never be
       empty.  */
    assert(hattr && node && attrsvals);

    if (!(hd = hash_find(hattr, attrsvals))) {
        if (!(hd = (struct hosts_data *)malloc(sizeof(struct hosts_data)))) {
            fprintf(stderr, "malloc: %s\n", strerror(errno));
            exit(1);
        }

        /* Copy the attrsvals string for a new key.  */
        if (!(hashkey = (char *) malloc(strlen(attrsvals) + 1))) {
            fprintf(stderr, "malloc: %s\n", strerror(errno));
            exit(1);
        }
        memcpy(hashkey, attrsvals, strlen(attrsvals) + 1);
        hd->key = hashkey;

        if (!(hd->hl = hostlist_create(NULL))) {
            fprintf(stderr, "hostlist_create: %s\n", strerror(errno));
            exit(1);
        }

        if (!hash_insert(hattr, hd->key, hd)) {
            fprintf(stderr, "hash_insert: %s\n", strerror(errno));
            exit(1);
        }
    }

    if (!hostlist_push(hd->hl, node)) {
        fprintf(stderr, "hostlist_push: %s\n", strerror(errno));
        exit(1);
    }
    /* hd and hashkey are destroyed with hash_destroy(hattr).  */
}

static int
_flatten_attrsvals(genders_t gp, const char *node, char **attrs, char **vals,
                   const int len, char *output_buffer)
{
    int i;
    int flatstringpos;
    int maxavlen;
    int attrlen;
    char *avstring = NULL;
    List avlist = NULL;
    ListIterator attrvallist_itr = NULL;

    /* Store ATTRS and VALS as sorted pairs in output_buffer.  */

    assert(gp && node && output_buffer);

    maxavlen = genders_getmaxattrlen(gp) + 2 + genders_getmaxvallen(gp);
    avlist = list_create(free);

    /* Get each attrval pair and store in avlist. */
    for (i = 0; i < len; i++) {
        if (!(avstring = (char *) malloc(sizeof(char)*maxavlen))) {
            fprintf(stderr, "malloc: %s\n", strerror(errno));
            goto cleanup;
        }

        attrlen = strlen(attrs[i]);
        memcpy(avstring, attrs[i], attrlen + 1);

        if (*vals[i]) {
            *(avstring + attrlen) = '=';
            memcpy(avstring + attrlen + 1, vals[i], strlen(vals[i]) + 1);
        }

        if (!list_append(avlist, avstring)) {
            fprintf(stderr, "list_insert: %s\n", strerror(errno));
            goto cleanup;
        }
    }

    list_sort(avlist, (ListCmpF) strcmp);

    flatstringpos = 0;
    if (!(attrvallist_itr = list_iterator_create(avlist))) {
        fprintf(stderr, "list_iterator_create: %s\n", strerror(errno));
        goto cleanup;
    }
    while ((avstring = list_next(attrvallist_itr)))
    {
        memcpy( output_buffer + flatstringpos, avstring, strlen(avstring));
        flatstringpos += strlen(avstring);

        /* multiple empty attrs will result in multiple commas,
         * I don't think that's worse than hiding them */
        *(output_buffer + flatstringpos) = ',';
        flatstringpos++;
    }
    list_iterator_destroy(attrvallist_itr);

    /* kill that last comma or make an empty string*/
    if (len)
        *(output_buffer + flatstringpos - 1) = '\0';
    else
        *output_buffer = '\0';

    return(0);

    cleanup:
    if (avlist)
        list_destroy(avlist);
    if (attrvallist_itr)
        list_iterator_destroy(attrvallist_itr);
    if (avstring)
        free(avstring);
    return(1);
}

static void
compress_hosts(genders_t gp)
{
    char **nodes;
    char *attrsvals;
    char **attrs, **vals;
    int attrslen, valslen;
    int attrscount;
    int numattrs;
    int nodeslen;
    int nodescount;
    int numnodes;
    int maxattrsvalslen;
    hash_t hattr = NULL;
    hash_t hrange = NULL;
    List hlist = NULL;
    struct store_hostrange_data shd;
    int i;

    /* Similar to compress.  For each host, gather attrs & vals as a set,
       compress hostlists of hosts with common sets.  The result is that each
       node only appears only on one line.  */

    /* need to treat values w/ raw inputs in order to compress */
    if (genders_set_flags(gp, GENDERS_FLAG_RAW_VALUES) < 0)
        _gend_error_exit(gp, "genders_set_flags");

    if ((numnodes = genders_getnumnodes(gp)) < 0)
        _gend_error_exit(gp, "genders_getnumnodes");

    if ((numattrs = genders_getnumattrs(gp)) < 0)
        _gend_error_exit(gp, "genders_getnumattrs");

    if ((attrslen = genders_attrlist_create(gp, &attrs)) < 0)
        _gend_error_exit(gp, "genders_attrlist_create");

    if ((valslen = genders_vallist_create(gp, &vals)) < 0)
        _gend_error_exit(gp, "genders_vallist_create");

    /* casses: I found that there are usually around numattrs/10 unique attrval
     * sets in my examples, so numattrs should be plenty of bins.
     * numattrs + 1, in case numattrs == 0
     */
    if (!(hattr = hash_create((numattrs + 1),
                              (hash_key_f)hash_key_string,
                              (hash_cmp_f)strcmp,
                              _hosts_data_del))) {
        fprintf(stderr, "hash_create: %s\n", strerror(errno));
        exit(1);
    }

    if ((nodeslen = genders_nodelist_create(gp, &nodes)) < 0)
        _gend_error_exit(gp, "genders_nodelist_create");

    if ((nodescount = genders_getnodes(gp, nodes, nodeslen, NULL, NULL)) < 0)
        _gend_error_exit(gp, "genders_getnodes");

    /* Assume that each attr and val is max possible length, leave room for '='
     * and (',' or terminating '\0').  */
    /* Potential integer overflow here, but a genders file that big would
     * have other problems, not checking.*/
    maxattrsvalslen = (genders_getmaxattrlen(gp) + 2
                       + genders_getmaxvallen(gp)) * numattrs;
    if (!(attrsvals = (char *) malloc(maxattrsvalslen * sizeof(char)))) {
        fprintf(stderr, "memory: %s\n", strerror(errno));
        exit(1);
    }

    for (i = 0; i < nodescount; i++) {
        if (genders_attrlist_clear(gp, attrs) < 0)
            _gend_error_exit(gp, "genders_attrlist_clear");

        if (genders_vallist_clear(gp, vals) < 0)
            _gend_error_exit(gp, "genders_vallist_clear");

        if ((attrscount = genders_getattr(gp,
                                          attrs,
                                          vals,
                                          attrslen,
                                          nodes[i])) < 0)
            _gend_error_exit(gp, "genders_getattr");

        if (attrscount) {
                if (_flatten_attrsvals(gp,
                                    nodes[i],
                                    attrs,
                                    vals,
                                    attrscount,
                                    attrsvals) < 0)
                    _gend_error_exit(gp, "_flatten_attrsvals");
                _hash_allattrvals(hattr, nodes[i], attrsvals);
            } else
                _hash_allattrvals(hattr, nodes[i], NOATTRSFLAG);
    }

    /* Now, find all the common attributes for a particular hostrange
     * For compress_nodes: Leaving this even though they're already grouped
     * because it also compresses the hostrange and reverses the hash
     */
    if (!(hrange = hash_create(numnodes,
                              (hash_key_f)hash_key_string,
                              (hash_cmp_f)strcmp,
                              _attr_list_del))) {
        fprintf(stderr, "hash_create: %s\n", strerror(errno));
        exit(1);
    }

    if (hash_for_each(hattr, _hash_hostrange, &hrange) < 0) {
        fprintf(stderr, "hash_for_each: %s\n", strerror(errno));
        exit(1);
    }

    if (!(hlist = list_create(NULL))) {
        fprintf(stderr, "list_create: %s\n", strerror(errno));
        exit(1);
    }

    shd.hlist = hlist;
    shd.maxhostrangelen = 0;

    if (hash_for_each(hrange, _store_hostrange, &shd) < 0) {
        fprintf(stderr, "hash_for_each: %s\n", strerror(errno));
        exit(1);
    }

    list_sort(hlist, _hostrange_cmp);

    if (list_for_each(hlist, _output_hostrange, &shd.maxhostrangelen) < 0) {
        fprintf(stderr, "list_for_each: %s\n", strerror(errno));
        exit(1);
    }

    free(attrsvals);
    genders_nodelist_destroy(gp, nodes);
    hash_destroy(hattr);
    hash_destroy(hrange);
    list_destroy(hlist);
}

/**
 ** Utility functions
 **/

static int
_gend_error_exit(genders_t gp, char *msg)
{
    fprintf(stderr, "nodeattr: %s: %s\n",
        msg, genders_strerror(genders_errnum(gp)));
    if (genders_errnum(gp) == GENDERS_ERR_PARSE) {
#if HAVE_GETOPT_LONG
        fprintf(stderr, "nodeattr: use --parse-check to debug errors\n");
#else
        fprintf(stderr, "nodeattr: use -k to debug errors\n");
#endif
    }
    exit(1);
}

static void *
_safe_malloc(size_t size)
{
    void *obj = (void *)malloc(size);

    if (obj == NULL) {
        fprintf(stderr, "nodeattr: out of memory\n");
        exit(1);
    }
    memset(obj, 0, size);
    return obj;
}

/* Create a host range string.  Caller must free result. */
static void *
_rangestr(hostlist_t hl, fmt_t qfmt)
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
static char *
_val_create(genders_t gp)
{
    int maxvallen;
    char *val;

    if ((maxvallen = genders_getmaxvallen(gp)) < 0)
        _gend_error_exit(gp, "genders_getmaxvallen");
    val = (char *)_safe_malloc(maxvallen + 1);

    return val;
}

#if 0
/* Create a node string.  Caller must free result. */
static char *
_node_create(genders_t gp)
{
    int maxnodelen;
    char *node;

    if ((maxnodelen = genders_getmaxnodelen(gp)) < 0)
        _gend_error_exit(gp, "genders_getmaxnodelen");
    node = (char *)_safe_malloc(maxnodelen + 1);
    return node;
}

/* Create an attribute string.  Caller must free result. */
static char *
_attr_create(genders_t gp)
{
    int maxattrlen;
    char *attr;

    if ((maxattrlen = genders_getmaxattrlen(gp)) < 0)
        _gend_error_exit(gp, "genders_getmaxattrlen");
    attr = (char *)_safe_malloc(maxattrlen + 1);
    return attr;
}

/* Convert "altname" to "gendname". Caller must free result. */
static char *
_to_gendname(genders_t gp, char *val)
{
    char **nodes;
    int count;
    int len;
    char *node = NULL;

    if ((len = genders_nodelist_create(gp, &nodes)) < 0)
        _gend_error_exit(gp, "genders_nodelist_create");

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
