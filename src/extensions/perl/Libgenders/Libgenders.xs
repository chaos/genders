/*****************************************************************************\
 *  $Id: Libgenders.xs,v 1.43 2009-06-02 18:05:21 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2001-2007 The Regents of the University of California.
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
 *  with Genders.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include <genders.h>

MODULE = Libgenders             PACKAGE = Libgenders            

PROTOTYPES: ENABLE

int
GENDERS_ERR_SUCCESS (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_SUCCESS;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_NULLHANDLE (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_NULLHANDLE;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_OPEN (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_OPEN;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_READ (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_READ;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_PARSE (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_PARSE;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_NOTLOADED (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_NOTLOADED;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_ISLOADED (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_ISLOADED;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_OVERFLOW (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_OVERFLOW;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_PARAMETERS (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_PARAMETERS;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_NULLPTR (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_NULLPTR;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_NOTFOUND (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_NOTFOUND;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_OUTMEM (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_OUTMEM;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_SYNTAX (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_SYNTAX;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_MAGIC (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_MAGIC;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_INTERNAL (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_INTERNAL;
    OUTPUT:
        RETVAL    

int
GENDERS_ERR_ERRNUMRANGE (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_ERR_ERRNUMRANGE;
    OUTPUT:
        RETVAL    

char * 
GENDERS_DEFAULT_FILE (sv=&PL_sv_undef)
    SV *sv    
    CODE:
        RETVAL = GENDERS_DEFAULT_FILE;
    OUTPUT:
        RETVAL    

void
DESTROY(handle)
    genders_t handle    
    CODE:
       (void)genders_handle_destroy(handle);

genders_t
genders_handle_create(CLASS)
        char *CLASS;
        CODE:
                RETVAL = genders_handle_create();
        OUTPUT:
                RETVAL

int
genders_load_data(handle, filename=NULL)
        genders_t handle
        char *filename
        CODE:
                RETVAL = genders_load_data(handle, filename);
        OUTPUT:
                RETVAL

int
genders_errnum(handle)
    genders_t handle
    CODE:
        RETVAL = genders_errnum(handle);
    OUTPUT:
        RETVAL

char *
genders_strerror(handle, errnum)
    genders_t handle    
    int errnum
    CODE:
        RETVAL = genders_strerror(errnum);
    OUTPUT:
        RETVAL

char *
genders_errormsg(handle)
    genders_t handle
    CODE:
        RETVAL = genders_errormsg(handle);
    OUTPUT:
        RETVAL

void
genders_perror(handle, msg=NULL)
    genders_t handle
    char *msg
    CODE:
        genders_perror(handle, msg);

int
genders_getnumnodes(handle)
    genders_t handle
    CODE:
        RETVAL = genders_getnumnodes(handle);
    OUTPUT:
        RETVAL

int
genders_getnumattrs(handle)
    genders_t handle
    CODE:
        RETVAL = genders_getnumattrs(handle);
    OUTPUT:
        RETVAL

SV *
genders_getnodename(handle) 
    genders_t handle
    PREINIT:
        int len;
        char *buf = NULL;
    CODE:
        if ((len = genders_getmaxnodelen(handle)) < 0) 
            goto handle_error;

        if (!(buf = (char *)malloc(len+1))) 
            goto handle_error;

        memset(buf, '\0', len+1);

        if (genders_getnodename(handle, buf, len+1) < 0) 
            goto handle_error;

        RETVAL = newSVpv(buf, 0);
        free(buf);
        goto the_end;

        handle_error:

            free(buf);
            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL

AV *
genders_getnodes(handle, attr=NULL, val=NULL) 
    genders_t handle
    char *attr
    char *val
    PREINIT:
        int len, num, errnum, i;
        char **nlist = NULL; 
    CODE:
        if ((len = genders_nodelist_create(handle, &nlist)) < 0) 
            goto handle_error;

        if ((num = genders_getnodes(handle, nlist, len, attr, val)) < 0)
            goto handle_error;

        RETVAL = newAV();
        for (i = 0; i < num; i++)
            av_push(RETVAL, newSVpv(nlist[i], 0));
        
        if (genders_nodelist_destroy(handle, nlist) < 0)
            goto handle_error;

        goto the_end;

        handle_error:

            errnum = genders_errnum(handle);

            (void)genders_nodelist_destroy(handle, nlist);

            genders_set_errnum(handle, errnum);

            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL    

AV *
genders_getattr(handle, node=NULL)
    genders_t handle
    char *node
    PREINIT:
        int len, num, errnum, i;
        char **alist = NULL; 
        char **vlist = NULL; 
        AV *attrs;
        AV *vals;
    CODE:
        if ((len = genders_attrlist_create(handle, &alist)) < 0) 
            goto handle_error;

        if ((len = genders_vallist_create(handle, &vlist)) < 0) 
            goto handle_error;

        if ((num = genders_getattr(handle, alist, vlist, len, node)) < 0)
            goto handle_error;

        attrs = newAV();
        for (i = 0; i < num; i++)
            av_push(attrs, newSVpv(alist[i], 0));

        vals = newAV();
        for (i = 0; i < num; i++)
            av_push(vals, newSVpv(vlist[i], 0));
        
        RETVAL = newAV();
        av_push(RETVAL, newRV_noinc((SV *)attrs));
        av_push(RETVAL, newRV_noinc((SV *)vals));
    
        if (genders_attrlist_destroy(handle, alist) < 0)
            goto handle_error;
	alist = NULL;

        if (genders_vallist_destroy(handle, vlist) < 0)
            goto handle_error;
	vlist = NULL;

        goto the_end;

        handle_error:

            errnum = genders_errnum(handle);

            (void)genders_attrlist_destroy(handle, alist);
            (void)genders_vallist_destroy(handle, vlist);

            genders_set_errnum(handle, errnum);
           
            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL

AV *
genders_getattr_all(handle)
    genders_t handle
    PREINIT:
        int len, num, errnum, i;
        char **alist = NULL; 
    CODE:
        if ((len = genders_attrlist_create(handle, &alist)) < 0) 
            goto handle_error;

        if ((num = genders_getattr_all(handle, alist, len)) < 0)
            goto handle_error;

        RETVAL = newAV();
        for (i = 0; i < num; i++)
            av_push(RETVAL, newSVpv(alist[i], 0));
        
        if (genders_attrlist_destroy(handle, alist) < 0)
            goto handle_error;

        goto the_end;
                    
        handle_error:

            errnum = genders_errnum(handle);
        
            (void)genders_attrlist_destroy(handle, alist);

            genders_set_errnum(handle, errnum);

            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL    

SV *
genders_getattrval(handle, attr, node=NULL)
    genders_t handle
    char *attr
    char *node
    PREINIT:
        int rv, len;
        char *buf = NULL;
    CODE:

        if ((len = genders_getmaxvallen(handle)) < 0) 
            goto handle_error;

        if (!(buf = (char *)malloc(len+1))) 
            goto handle_error;

        memset(buf, '\0', len+1);

        if ((rv = genders_testattr(handle, node, attr, buf, len+1)) < 0)
            goto handle_error;

        if (rv == 1 && strlen(buf) > 0) 
            RETVAL = newSVpv(buf, 0);
        else 
            RETVAL = newSVpv("", 0);

        free(buf);
        goto the_end;

        handle_error:

            free(buf);
            XSRETURN_UNDEF;
        
        the_end:
    OUTPUT:
        RETVAL

int
genders_testattr(handle, attr, node=NULL)
    genders_t handle
    char *node
    char *attr
    CODE:
        RETVAL = genders_testattrval(handle, node, attr, NULL);
    OUTPUT:
        RETVAL      

int
genders_testattrval(handle, attr, val, node=NULL)
    genders_t handle
    char *node
    char *attr
    char *val
    CODE:
        RETVAL = genders_testattrval(handle, node, attr, val);
    OUTPUT:
        RETVAL      

int
genders_isnode(handle, node=NULL)
    genders_t handle
    char *node
    CODE:
        RETVAL = genders_isnode(handle, node);
    OUTPUT:
        RETVAL

int
genders_isattr(handle, attr)
    genders_t handle
    char *attr
    CODE:
        RETVAL = genders_isattr(handle, attr);
    OUTPUT:
        RETVAL

int
genders_isattrval(handle, attr, val)
    genders_t handle
    char *attr
    char *val
    CODE:
        RETVAL = genders_isattrval(handle, attr, val);
    OUTPUT:
        RETVAL

int 
genders_index_attrvals(handle, attr)
    genders_t handle
    char *attr
    CODE:
        RETVAL = genders_index_attrvals(handle, attr);
    OUTPUT:
        RETVAL    

AV *
genders_query(handle, query=NULL)
    genders_t handle
    char *query
    PREINIT:
        int len, num, errnum, i;
        char **nlist = NULL; 
    CODE:
        if ((len = genders_nodelist_create(handle, &nlist)) < 0) 
            goto handle_error;

        if ((num = genders_query(handle, nlist, len, query)) < 0)
            goto handle_error;

        RETVAL = newAV();
        for (i = 0; i < num; i++)
            av_push(RETVAL, newSVpv(nlist[i], 0));
        
        if (genders_nodelist_destroy(handle, nlist) < 0)
            goto handle_error;

        goto the_end;

        handle_error:

            errnum = genders_errnum(handle);

            (void)genders_nodelist_destroy(handle, nlist);

            genders_set_errnum(handle, errnum);

            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL    

int
genders_testquery(handle, query, node=NULL)
    genders_t handle
    char *node
    char *query
    CODE:
        RETVAL = genders_testquery(handle, node, query);
    OUTPUT:
        RETVAL      

int
genders_parse(handle, filename=NULL)
    genders_t handle
    char *filename
    CODE:
        RETVAL = genders_parse(handle, filename, NULL);
    OUTPUT:
        RETVAL

void
genders_set_errnum(handle, errnum)
    genders_t handle
    int errnum
    CODE:
        genders_set_errnum(handle, errnum);
