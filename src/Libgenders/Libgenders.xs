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
constant_GENDERS_ERR_NO(char *name, int len, int arg)
{
    if (14 + 1 >= len ) {
	errno = EINVAL;
	return 0;
    }
    switch (name[14 + 1]) {
    case 'C':
	if (strEQ(name + 14, "TCLOSED")) {	/* GENDERS_ERR_NO removed */
#ifdef GENDERS_ERR_NOTCLOSED
	    return GENDERS_ERR_NOTCLOSED;
#else
	    goto not_there;
#endif
	}
    case 'F':
	if (strEQ(name + 14, "TFOUND")) {	/* GENDERS_ERR_NO removed */
#ifdef GENDERS_ERR_NOTFOUND
	    return GENDERS_ERR_NOTFOUND;
#else
	    goto not_there;
#endif
	}
    case 'O':
	if (strEQ(name + 14, "TOPEN")) {	/* GENDERS_ERR_NO removed */
#ifdef GENDERS_ERR_NOTOPEN
	    return GENDERS_ERR_NOTOPEN;
#else
	    goto not_there;
#endif
	}
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

static double
constant_GENDERS_ERR_N(char *name, int len, int arg)
{
    switch (name[13 + 0]) {
    case 'O':
	return constant_GENDERS_ERR_NO(name, len, arg);
    case 'U':
	if (strEQ(name + 13, "ULLPTR")) {	/* GENDERS_ERR_N removed */
#ifdef GENDERS_ERR_NULLPTR
	    return GENDERS_ERR_NULLPTR;
#else
	    goto not_there;
#endif
	}
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

static double
constant_GENDERS_ERR_O(char *name, int len, int arg)
{
    switch (name[13 + 0]) {
    case 'P':
	if (strEQ(name + 13, "PEN")) {	/* GENDERS_ERR_O removed */
#ifdef GENDERS_ERR_OPEN
	    return GENDERS_ERR_OPEN;
#else
	    goto not_there;
#endif
	}
    case 'U':
	if (strEQ(name + 13, "UTMEM")) {	/* GENDERS_ERR_O removed */
#ifdef GENDERS_ERR_OUTMEM
	    return GENDERS_ERR_OUTMEM;
#else
	    goto not_there;
#endif
	}
    case 'V':
	if (strEQ(name + 13, "VERFLOW")) {	/* GENDERS_ERR_O removed */
#ifdef GENDERS_ERR_OVERFLOW
	    return GENDERS_ERR_OVERFLOW;
#else
	    goto not_there;
#endif
	}
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

static double
constant_GENDERS_ERR_P(char *name, int len, int arg)
{
    if (13 + 2 >= len ) {
	errno = EINVAL;
	return 0;
    }
    switch (name[13 + 2]) {
    case 'A':
	if (strEQ(name + 13, "ARAMETERS")) {	/* GENDERS_ERR_P removed */
#ifdef GENDERS_ERR_PARAMETERS
	    return GENDERS_ERR_PARAMETERS;
#else
	    goto not_there;
#endif
	}
    case 'S':
	if (strEQ(name + 13, "ARSE")) {	/* GENDERS_ERR_P removed */
#ifdef GENDERS_ERR_PARSE
	    return GENDERS_ERR_PARSE;
#else
	    goto not_there;
#endif
	}
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

static double
constant_GENDERS_ERR_I(char *name, int len, int arg)
{
    switch (name[13 + 0]) {
    case 'N':
	if (strEQ(name + 13, "NTERNAL")) {	/* GENDERS_ERR_I removed */
#ifdef GENDERS_ERR_INTERNAL
	    return GENDERS_ERR_INTERNAL;
#else
	    goto not_there;
#endif
	}
    case 'S':
	if (strEQ(name + 13, "SFREE")) {	/* GENDERS_ERR_I removed */
#ifdef GENDERS_ERR_ISFREE
	    return GENDERS_ERR_ISFREE;
#else
	    goto not_there;
#endif
	}
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

static double
constant(char *name, int len, int arg)
{
    errno = 0;
    if (0 + 12 >= len ) {
	errno = EINVAL;
	return 0;
    }
    switch (name[0 + 12]) {
    case 'F':
	if (strEQ(name + 0, "GENDERS_ERR_FREEMEM")) {	/*  removed */
#ifdef GENDERS_ERR_FREEMEM
	    return GENDERS_ERR_FREEMEM;
#else
	    goto not_there;
#endif
	}
    case 'I':
	if (!strnEQ(name + 0,"GENDERS_ERR_", 12))
	    break;
	return constant_GENDERS_ERR_I(name, len, arg);
    case 'M':
	if (strEQ(name + 0, "GENDERS_ERR_MAGIC")) {	/*  removed */
#ifdef GENDERS_ERR_MAGIC
	    return GENDERS_ERR_MAGIC;
#else
	    goto not_there;
#endif
	}
    case 'N':
	if (!strnEQ(name + 0,"GENDERS_ERR_", 12))
	    break;
	return constant_GENDERS_ERR_N(name, len, arg);
    case 'O':
	if (!strnEQ(name + 0,"GENDERS_ERR_", 12))
	    break;
	return constant_GENDERS_ERR_O(name, len, arg);
    case 'P':
	if (!strnEQ(name + 0,"GENDERS_ERR_", 12))
	    break;
	return constant_GENDERS_ERR_P(name, len, arg);
    case 'R':
	if (strEQ(name + 0, "GENDERS_ERR_READ")) {	/*  removed */
#ifdef GENDERS_ERR_READ
	    return GENDERS_ERR_READ;
#else
	    goto not_there;
#endif
	}
    case 'S':
	if (strEQ(name + 0, "GENDERS_ERR_SUCCESS")) {	/*  removed */
#ifdef GENDERS_ERR_SUCCESS
	    return GENDERS_ERR_SUCCESS;
#else
	    goto not_there;
#endif
	}
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}


MODULE = Libgenders		PACKAGE = Libgenders		


double
constant(sv,arg)
    PREINIT:
	STRLEN		len;
    INPUT:
	SV *		sv
	char *		s = SvPV(sv, len);
	int		arg
    CODE:
	RETVAL = constant(s,len,arg);
    OUTPUT:
	RETVAL

SV *
string_constant(str)
	char *str
	CODE:
		if (strcmp(str, "DEFAULT_GENDERS_FILE") == 0)
			RETVAL = newSVpv(DEFAULT_GENDERS_FILE, 0);	
		else
			RETVAL = newSVpv("", 0);
	OUTPUT:
		RETVAL

genders_t
genders_handle_create()
	CODE:
		RETVAL = genders_handle_create();
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
		if (ptr != NULL)
			RETVAL = newSVpv(ptr, 0);	
		else
			RETVAL = newSVpv("", 0);
	OUTPUT:
		RETVAL

SV *
genders_errormsg(handle)
	genders_t handle
	PREINIT:
		char *ptr;
	CODE:
		ptr = genders_errormsg(handle);
		if (ptr != NULL)
			RETVAL = newSVpv(ptr, 0);	
		else
			RETVAL = newSVpv("", 0);
	OUTPUT:
		RETVAL

void
genders_perror(handle, msg)
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
		int maxvallen;
		char *buf = NULL;
	CODE:
		if ((maxvallen = genders_getmaxvallen(handle)) == -1) 
			goto handle_error;

		if ((buf = (char *)malloc(maxvallen+1)) == NULL) 
			goto handle_error;

		memset(buf, '\0', maxvallen+1);

		if (genders_getnodename(handle, buf, maxvallen) == -1) 
			goto handle_error;

		RETVAL = newSVpv(buf, 0);
		free(buf);
		goto the_end;

		handle_error:

		if (buf != NULL) {
			free(buf);
		}
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
		if ((num = genders_nodelist_create(handle, 
						   &nodelist)) == -1) 
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

		if (nodelist != NULL) {
			(void)genders_nodelist_destroy(handle, nodelist);
		}
		RETVAL = newAV();

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
		if ((num = genders_attrlist_create(handle, 
						   &attrlist)) == -1) 
			goto handle_error;

		if ((num = genders_vallist_create(handle, 
						  &vallist)) == -1) 
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

		if (attrlist != NULL) {
			(void)genders_attrlist_destroy(handle, attrlist);
		}
		if (vallist != NULL) {
			(void)genders_vallist_destroy(handle, vallist);
		}
		attrs = newAV();
		vals = newAV();
		
		RETVAL = newAV();

		av_push(RETVAL, newRV_noinc((SV *)attrs));	
		av_push(RETVAL, newRV_noinc((SV *)vals));

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
		if ((num = genders_attrlist_create(handle, 
						   &attrlist)) == -1) 
			goto handle_error;

		if ((ret = genders_getattr_all(handle, 
	                                       attrlist, 
	                                       num)) == -1)
			goto handle_error;

		RETVAL = newAV();
		for (i = 0; i < ret; i++) {
			av_push(RETVAL, newSVpv(attrlist[i], 0));
		}
		
		(void)genders_attrlist_destroy(handle, attrlist);
		goto the_end;

		handle_error:

		if (attrlist != NULL) {
			(void)genders_attrlist_destroy(handle, attrlist);
		}
		RETVAL = newAV();

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

		if ((ret = genders_getattrval(handle, 
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
		if (buf != NULL) {
			free(buf);
		}
		RETVAL = newSVpv("", 0);

		the_end:
	OUTPUT:
		RETVAL

int
genders_testattr(handle, attr, node=NULL)
	genders_t handle
	char *node
	char *attr
	CODE:
		RETVAL = genders_testattr(handle, node, attr);
	OUTPUT:
		RETVAL		

int
genders_testattrval(handle, attr, val=NULL, node=NULL)
	genders_t handle
	char *node
	char *attr
	char *val
	CODE:
		RETVAL = genders_testattrval(handle, node, attr, val);
	OUTPUT:
		RETVAL		

int
genders_testnode(handle, node=NULL)
	genders_t handle
	char *node
	CODE:
		RETVAL = genders_testnode(handle, node);
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

