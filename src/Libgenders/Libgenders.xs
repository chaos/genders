#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <stdarg.h>
#include <string.h>

#include "genders.h"

static int
not_here(char *s)
{
    croak("%s not implemented on this architecture", s);
    return -1;
}

static double
constant(char *name, int len, int arg)
{
    errno = EINVAL;
    return 0;
}

MODULE = Libgenders             PACKAGE = Libgenders            

double
constant(sv,arg)
    PREINIT:
        STRLEN          len;
    INPUT:
        SV *            sv
        char *          s = SvPV(sv, len);
        int             arg
    CODE:
        RETVAL = constant(s,len,arg);
    OUTPUT:
        RETVAL

int
errnum(str)
    char *str
    CODE:
        if (strcmp(str, "GENDERS_ERR_SUCCESS") == 0)
            RETVAL = GENDERS_ERR_SUCCESS;
        else if (strcmp(str, "GENDERS_ERR_NULLHANDLE") == 0)
            RETVAL = GENDERS_ERR_NULLHANDLE;
        else if (strcmp(str, "GENDERS_ERR_OPEN") == 0)
            RETVAL = GENDERS_ERR_OPEN;
        else if (strcmp(str, "GENDERS_ERR_READ") == 0)
            RETVAL = GENDERS_ERR_READ;
        else if (strcmp(str, "GENDERS_ERR_PARSE") == 0)
            RETVAL = GENDERS_ERR_PARSE;
        else if (strcmp(str, "GENDERS_ERR_NOTLOADED") == 0)
            RETVAL = GENDERS_ERR_NOTLOADED;
        else if (strcmp(str, "GENDERS_ERR_ISLOADED") == 0)
            RETVAL = GENDERS_ERR_ISLOADED;
        else if (strcmp(str, "GENDERS_ERR_OVERFLOW") == 0)
            RETVAL = GENDERS_ERR_OVERFLOW;
        else if (strcmp(str, "GENDERS_ERR_PARAMETERS") == 0)
            RETVAL = GENDERS_ERR_PARAMETERS;
        else if (strcmp(str, "GENDERS_ERR_NULLPTR") == 0)
            RETVAL = GENDERS_ERR_NULLPTR;
        else if (strcmp(str, "GENDERS_ERR_NOTFOUND") == 0)
            RETVAL = GENDERS_ERR_NOTFOUND;
        else if (strcmp(str, "GENDERS_ERR_OUTMEM") == 0)
            RETVAL = GENDERS_ERR_OUTMEM;
        else if (strcmp(str, "GENDERS_ERR_MAGIC") == 0)
            RETVAL = GENDERS_ERR_MAGIC;
        else if (strcmp(str, "GENDERS_ERR_INTERNAL") == 0)
            RETVAL = GENDERS_ERR_INTERNAL;
        else if (strcmp(str, "GENDERS_ERR_ERRNUMRANGE") == 0)
            RETVAL = GENDERS_ERR_ERRNUMRANGE;
        else
            RETVAL = -1;
    OUTPUT:
        RETVAL


SV *
string_constant(str)
    char *str
    CODE:
        if (strcmp(str, "DEFAULT_GENDERS_FILE") == 0)
            RETVAL = newSVpv(DEFAULT_GENDERS_FILE, 0);      
        else
            XSRETURN_UNDEF;
    OUTPUT:
        RETVAL

genders_t
genders_handle_create()
    INIT: 
        genders_t handle;
    CODE:
        handle = genders_handle_create();
        if (handle != NULL) 
            RETVAL = handle;
        else
            XSRETURN_UNDEF;
    OUTPUT:
        RETVAL

int
genders_handle_destroy(handle)
    genders_t handle
    CODE:
        RETVAL = genders_handle_destroy(handle);
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

SV *
genders_strerror(errnum)
    int errnum
    PREINIT:
        char *ptr;
    CODE:
        ptr = genders_strerror(errnum);
        RETVAL = newSVpv(ptr, 0);       
    OUTPUT:
        RETVAL

SV *
genders_errormsg(handle)
    genders_t handle
    PREINIT:
       char *ptr;
    CODE:
        ptr = genders_errormsg(handle);
        RETVAL = newSVpv(ptr, 0);       
    OUTPUT:
        RETVAL

void
genders_perror(handle, msg=NULL)
    genders_t handle
    char *msg
    CODE:
        genders_perror(handle, msg);

int
genders_handle_dump(handle)
    genders_t handle
    CODE:
        RETVAL = genders_handle_dump(handle, NULL);
    OUTPUT:
        RETVAL

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
        int maxnodelen;
        char *buf = NULL;
    CODE:
        if ((maxnodelen = genders_getmaxnodelen(handle)) == -1) 
            goto handle_error;

        if ((buf = (char *)malloc(maxnodelen+1)) == NULL) 
            goto handle_error;

        memset(buf, '\0', maxnodelen+1);

        if (genders_getnodename(handle, buf, maxnodelen) == -1) 
            goto handle_error;

        RETVAL = newSVpv(buf, 0);
        free(buf);
        goto the_end;

        handle_error:

            if (buf != NULL) 
                free(buf);
                
            RETVAL = newSVpv("", 0);

        the_end:
    OUTPUT:
        RETVAL

AV *
genders_getnodes(handle, attr=NULL, val=NULL) 
    genders_t handle
    char *attr
    char *val
    PREINIT:
        int num, ret, i;
        char **nodelist = NULL; 
    CODE:
        if ((num = genders_nodelist_create(handle, &nodelist)) == -1) 
            goto handle_error;

        if ((ret = genders_getnodes(handle, 
                                    nodelist, 
                                    num, 
                                    attr, 
                                    val)) == -1)
            goto handle_error;

        RETVAL = newAV();
        for (i = 0; i < ret; i++) {
            av_push(RETVAL, newSVpv(nodelist[i], 0));
        }
        
        (void)genders_nodelist_destroy(handle, nodelist);
        goto the_end;

        handle_error:

            /* manually destroy lists, to preserve error value */
            if (nodelist != NULL) {
                for (i = 0; i < num; i++) {
                    free(nodelist[i]);
                } 
                free(nodelist);
            }

            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL    

AV *
genders_getattr(handle, node=NULL)
    genders_t handle
    char *node
    PREINIT:
        int num, ret, i;
        char **attrlist = NULL; 
        char **vallist = NULL; 
        AV *attrs;
        AV *vals;
    CODE:
        if ((num = genders_attrlist_create(handle, &attrlist)) == -1) 
            goto handle_error;

        if ((num = genders_vallist_create(handle, &vallist)) == -1) 
            goto handle_error;

        if ((ret = genders_getattr(handle, 
                                   attrlist, 
                                   vallist, 
                                   num, 
                                   node)) == -1)
            goto handle_error;

        attrs = newAV();
        for (i = 0; i < ret; i++) {
            av_push(attrs, newSVpv(attrlist[i], 0));
        }

        vals = newAV();
        for (i = 0; i < ret; i++) {
            av_push(vals, newSVpv(vallist[i], 0));
        }
        
        RETVAL = newAV();
        av_push(RETVAL, newRV_noinc((SV *)attrs));
        av_push(RETVAL, newRV_noinc((SV *)vals));
    
        (void)genders_attrlist_destroy(handle, attrlist);
        (void)genders_vallist_destroy(handle, vallist);
        goto the_end;

        handle_error:

            /* manually destroy lists, to preserve error value */
            if (attrlist != NULL) {
                for (i = 0; i < num; i++) {
                    free(attrlist[i]);
                } 
                free(attrlist);
            }

            if (vallist != NULL) {
                for (i = 0; i < num; i++) {
                    free(vallist[i]);
                } 
                free(vallist);
            }

            XSRETURN_UNDEF;

        the_end:
    OUTPUT:
        RETVAL

AV *
genders_getattr_all(handle)
    genders_t handle
    PREINIT:
        int num, ret, i;
        char **attrlist = NULL; 
    CODE:
        if ((num = genders_attrlist_create(handle, &attrlist)) == -1) 
            goto handle_error;

        if ((ret = genders_getattr_all(handle, attrlist, num)) == -1)
            goto handle_error;

        RETVAL = newAV();
        for (i = 0; i < ret; i++) {
            av_push(RETVAL, newSVpv(attrlist[i], 0));
        }
        
        (void)genders_attrlist_destroy(handle, attrlist);
        goto the_end;
                    
        handle_error:

            /* manually destroy lists, to preserve error value */
            if (attrlist != NULL) {
                for (i = 0; i < num; i++) {
                    free(attrlist[i]);
                } 
                free(attrlist);
            }

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
        int ret, maxvallen;
        char *buf = NULL;
    CODE:

        if ((maxvallen = genders_getmaxvallen(handle)) == -1) 
            goto handle_error;

        if ((buf = (char *)malloc(maxvallen+1)) == NULL) 
            goto handle_error;

        memset(buf, '\0', maxvallen+1);

        if ((ret = genders_testattr(handle, 
                                    node, 
                                    attr, 
                                    buf, 
                                    maxvallen+1)) == -1) {
            goto handle_error;
        }

        if (ret == 1 && strlen(buf) > 0) 
            RETVAL = newSVpv(buf, 0);
        else 
            RETVAL = newSVpv("", 0);

        free(buf);
        goto the_end;

        handle_error:

            if (buf != NULL)
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
genders_parse(handle, filename=NULL)
    genders_t handle
    char *filename
    CODE:
        RETVAL = genders_parse(handle, filename, NULL);
    OUTPUT:
        RETVAL

