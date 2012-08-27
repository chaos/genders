#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <genders.h>

#include "Gendersjni.h"

static void
_throw_exception (JNIEnv *env, jobject obj, int errnum)
{
  jclass gexcept_class = NULL;

  if (errnum == GENDERS_ERR_OPEN)
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionOpen")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_OPEN));
    }
  else if (errnum == GENDERS_ERR_READ)
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionRead")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_READ));
    }
  else if (errnum == GENDERS_ERR_PARSE)
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionParse")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_PARSE));
    }
  else if (errnum == GENDERS_ERR_PARAMETERS)
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionParameters")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_PARAMETERS));
    }
  else if (errnum == GENDERS_ERR_NOTFOUND)
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionNotfound")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_NOTFOUND));
    }
  else if (errnum == GENDERS_ERR_SYNTAX)
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionSyntax")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_SYNTAX));
    }
  else
    {
      if (!(gexcept_class = (*env)->FindClass (env, "gov/llnl/lc/chaos/GendersExceptionInternal")))
	goto cleanup;

      (*env)->ThrowNew (env, gexcept_class, genders_strerror (GENDERS_ERR_INTERNAL));
    }

 cleanup:
  (*env)->DeleteLocalRef (env, gexcept_class);
}

static int
_constructor (JNIEnv *env, jobject obj, const char *filename)
{
  genders_t handle;
  jclass genders_cls = NULL;
  jfieldID gh_addr_fid;
  jint rv = -1;

  if (!(handle = genders_handle_create ()))
    {
      _throw_exception (env, obj, GENDERS_ERR_INTERNAL);
      goto cleanup;
    }

  if (genders_load_data (handle, filename) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      genders_handle_destroy (handle);
      goto cleanup;
    }

  genders_cls = (*env)->GetObjectClass (env, obj);

  if (!(gh_addr_fid = (*env)->GetFieldID (env, genders_cls, "gh_addr", "J")))
    {
      genders_handle_destroy (handle);
      goto cleanup;
    }

  (*env)->SetLongField (env, obj, gh_addr_fid, (long)handle);

  rv = 0;
 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_gov_llnl_lc_chaos_Genders_genders_1constructor (JNIEnv *env, jobject obj, jstring filename)
{
  const jbyte *filenameutf = NULL;
  jint rv = -1;

  if (filename)
    {
      if (!(filenameutf = (*env)->GetStringUTFChars(env, filename, NULL)))
	goto cleanup;
      
      rv = _constructor (env, obj, filenameutf);

    }
  else
    _constructor (env, obj, NULL);

 cleanup:
  if (filename && filenameutf)
      (*env)->ReleaseStringUTFChars(env, filename, filenameutf);
  return (rv);
}

static int
_get_handle (JNIEnv *env, jobject obj, genders_t *handle)
{
  jclass genders_cls;
  jfieldID gh_addr_fid;
  jlong jgh_addr_addr;
  int rv = -1;

  genders_cls = (*env)->GetObjectClass (env, obj);

  if (!(gh_addr_fid = (*env)->GetFieldID (env, genders_cls, "gh_addr", "J")))
    goto cleanup;
  
  jgh_addr_addr = (*env)->GetLongField (env, obj, gh_addr_fid);

  if (!jgh_addr_addr)
    {
      _throw_exception (env, obj, GENDERS_ERR_INTERNAL);
      goto cleanup;
    }

  (*handle) = (genders_t)jgh_addr_addr;

  rv = 0;
 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_gov_llnl_lc_chaos_Genders_getnumnodes (JNIEnv *env, jobject obj)
{
  genders_t handle;
  int rv = -1;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((rv = genders_getnumnodes (handle)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_gov_llnl_lc_chaos_Genders_getnumattrs (JNIEnv *env, jobject obj)
{
  genders_t handle;
  int rv = -1;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((rv = genders_getnumattrs (handle)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_gov_llnl_lc_chaos_Genders_getmaxattrs (JNIEnv *env, jobject obj)
{
  genders_t handle;
  int rv = -1;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((rv = genders_getmaxattrs (handle)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

 cleanup:
  return (rv);
}

JNIEXPORT jstring JNICALL
Java_gov_llnl_lc_chaos_Genders_getnodename (JNIEnv *env, jobject obj)
{
  genders_t handle;
  char *nodenamebuf = NULL;
  int maxnodenamelen;
  jstring jnodename = NULL;
  jstring rv = NULL;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((maxnodenamelen = genders_getmaxnodelen (handle)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (!(nodenamebuf = (char *)malloc (maxnodenamelen + 1)))
    {
      _throw_exception (env, obj, GENDERS_ERR_INTERNAL);
      goto cleanup;
    }

  memset (nodenamebuf, '\0', maxnodenamelen + 1);

  if (genders_getnodename (handle, nodenamebuf, maxnodenamelen + 1) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (!(jnodename = (*env)->NewStringUTF(env, nodenamebuf)))
    goto cleanup;

  rv = jnodename;
 cleanup:
  free (nodenamebuf);
  return (rv);
}

static  jobjectArray
_getnodes (JNIEnv *env, jobject obj, const char *attr, const char *val)
{
  genders_t handle;
  char **nodelist = NULL;
  int nodelistlen;
  jclass string_class = NULL;
  jobjectArray jnodelist = NULL;
  jobjectArray rv = NULL;
  int nodeslen;
  int i;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((nodelistlen = genders_nodelist_create (handle, &nodelist)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if ((nodeslen = genders_getnodes (handle, nodelist, nodelistlen, attr, val)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  string_class = (*env)->FindClass(env, "java/lang/String");

  if (!(jnodelist = (*env)->NewObjectArray(env, nodeslen, string_class, NULL)))
    goto cleanup;

  for (i = 0; i < nodeslen; i++)
    {
      jstring tmpstr;
      
      if (!(tmpstr = (*env)->NewStringUTF(env, nodelist[i])))
	goto cleanup;

      (*env)->SetObjectArrayElement (env, jnodelist, i, tmpstr);
      (*env)->DeleteLocalRef (env, tmpstr);
    }

  rv = jnodelist;
 cleanup:
  if (!rv && jnodelist)
    (*env)->DeleteLocalRef (env, jnodelist);
  genders_nodelist_destroy (handle, nodelist);
  (*env)->DeleteLocalRef (env, string_class);
  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_getnodes__ (JNIEnv *env, jobject obj)
{
  return (_getnodes (env, obj, NULL, NULL));
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_getnodes__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr)
{
  const jbyte *attrutf = NULL;
  jobjectArray rv = NULL;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  rv = _getnodes (env, obj, attrutf, NULL);

 cleanup:

  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);

  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_getnodes__Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr, jstring val)
{
  const jbyte *attrutf = NULL;
  const jbyte *valutf = NULL;
  jobjectArray rv = NULL;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  if (val)
    {
      if (!(valutf = (*env)->GetStringUTFChars(env, val, NULL)))
	goto cleanup;
    }

  rv = _getnodes (env, obj, attrutf, valutf);

 cleanup:

  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);

  if (val && valutf)
    (*env)->ReleaseStringUTFChars(env, val, valutf);

  return (rv);
}

static  jobjectArray
_getattr (JNIEnv *env, jobject obj, const char *node)
{
  genders_t handle;
  char **attrlist = NULL;
  int attrlistlen;
  jclass string_class = NULL;
  jobjectArray jlist = NULL;
  jobjectArray rv = NULL;
  int attrslen;
  int i;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((attrlistlen = genders_attrlist_create (handle, &attrlist)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if ((attrslen = genders_getattr (handle, attrlist, NULL, attrlistlen, node)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  string_class = (*env)->FindClass(env, "java/lang/String");

  if (!(jlist = (*env)->NewObjectArray(env, attrslen, string_class, NULL)))
    goto cleanup;

  for (i = 0; i < attrslen; i++)
    {
      jstring tmpstr;
      
      if (!(tmpstr = (*env)->NewStringUTF(env, attrlist[i])))
	goto cleanup;

      (*env)->SetObjectArrayElement (env, jlist, i, tmpstr);
      (*env)->DeleteLocalRef (env, tmpstr);
    }

  rv = jlist;
 cleanup:
  if (!rv && jlist)
    (*env)->DeleteLocalRef (env, jlist);
  genders_attrlist_destroy (handle, attrlist);
  (*env)->DeleteLocalRef (env, string_class);
  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_getattr__ (JNIEnv *env, jobject obj)
{
  return (_getattr (env, obj, NULL));
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_getattr__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring node)
{
  const jbyte *nodeutf;
  jobjectArray rv = NULL;

  if (node)
    {
      if (!(nodeutf = (*env)->GetStringUTFChars(env, node, NULL)))
	goto cleanup;
    }

  rv = _getattr (env, obj, nodeutf);

 cleanup:
  if (node && nodeutf)
    (*env)->ReleaseStringUTFChars(env, node, nodeutf);
  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_getattr_1all (JNIEnv *env, jobject obj)
{
  genders_t handle;
  char **attrlist = NULL;
  int attrlistlen;
  jclass string_class = NULL;
  jobjectArray jattrlist = NULL;
  jobjectArray rv = NULL;
  int attrslen;
  int i;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((attrlistlen = genders_attrlist_create (handle, &attrlist)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if ((attrslen = genders_getattr_all (handle, attrlist, attrlistlen)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  string_class = (*env)->FindClass(env, "java/lang/String");

  if (!(jattrlist = (*env)->NewObjectArray(env, attrslen, string_class, NULL)))
    goto cleanup;

  for (i = 0; i < attrslen; i++)
    {
      jstring tmpstr;
      
      if (!(tmpstr = (*env)->NewStringUTF(env, attrlist[i])))
	goto cleanup;

      (*env)->SetObjectArrayElement (env, jattrlist, i, tmpstr);
      (*env)->DeleteLocalRef (env, tmpstr);
    }

  rv = jattrlist;
 cleanup:
  if (!rv && jattrlist)
    (*env)->DeleteLocalRef (env, jattrlist);
  (*env)->DeleteLocalRef (env, string_class);
  return (rv);
}

static jstring
_getattrval (JNIEnv *env, jobject obj, const char *node, const char *attr)
{
  genders_t handle;
  char *valbuf = NULL;
  int maxvallen;
  jstring jval = NULL;
  jstring rv = NULL;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((maxvallen = genders_getmaxvallen (handle)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (!(valbuf = (char *)malloc (maxvallen + 1)))
    {
      _throw_exception (env, obj, GENDERS_ERR_INTERNAL);
      goto cleanup;
    }

  memset (valbuf, '\0', maxvallen + 1);

  if ((ret = genders_testattr (handle, node, attr, valbuf, maxvallen + 1)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    {
      if (!(jval = (*env)->NewStringUTF(env, valbuf)))
	goto cleanup;
    }
  
  rv = jval;
 cleanup:
  free (valbuf);
  return (rv);
}

JNIEXPORT jstring JNICALL
Java_gov_llnl_lc_chaos_Genders_getattrval__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr)
{
  const jbyte *attrutf;
  jstring rv = NULL;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  rv = _getattrval (env, obj, NULL, attrutf);

 cleanup:
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  return (rv);
}

JNIEXPORT jstring JNICALL
Java_gov_llnl_lc_chaos_Genders_getattrval__Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring node, jstring attr)
{
  const jbyte *nodeutf;
  const jbyte *attrutf;
  jstring rv = NULL;

  if (node)
    {
      if (!(nodeutf = (*env)->GetStringUTFChars(env, node, NULL)))
	goto cleanup;
    }

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  rv = _getattrval (env, obj, nodeutf, attrutf);

 cleanup:
  if (node && nodeutf)
    (*env)->ReleaseStringUTFChars(env, node, nodeutf);
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  return (rv);
}

static jboolean
_testattr (JNIEnv *env, jobject obj, const char *node, const char *attr)
{
  genders_t handle;
  jboolean rv = JNI_FALSE;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((ret = genders_testattr (handle, node, attr, NULL, 0)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    rv = JNI_TRUE;
  else
    rv = JNI_FALSE;

 cleanup:
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_testattr__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr)
{
  const jbyte *attrutf = NULL;
  jboolean rv = JNI_FALSE;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  rv = _testattr (env, obj, NULL, attrutf);

 cleanup:
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_testattr__Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring node, jstring attr)
{
  const jbyte *nodeutf = NULL;
  const jbyte *attrutf = NULL;
  jboolean rv = JNI_FALSE;

  if (node)
    {
      if (!(nodeutf = (*env)->GetStringUTFChars(env, node, NULL)))
	goto cleanup;
    }

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  rv = _testattr (env, obj, nodeutf, attrutf);

 cleanup:
  if (node && nodeutf)
    (*env)->ReleaseStringUTFChars(env, node, nodeutf);
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  return (rv);
}

static jboolean
_testattrval (JNIEnv *env, jobject obj, const char *node, const char *attr, const char *val)
{
  genders_t handle;
  jboolean rv = JNI_FALSE;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((ret = genders_testattrval (handle, node, attr, val)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    rv = JNI_TRUE;
  else
    rv = JNI_FALSE;

 cleanup:
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_testattrval__Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr, jstring val)
{
  const jbyte *attrutf = NULL;
  const jbyte *valutf = NULL;
  jboolean rv = JNI_FALSE;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  if (val)
    {
      if (!(valutf = (*env)->GetStringUTFChars(env, val, NULL)))
	goto cleanup;
    }

  rv = _testattrval (env, obj, NULL, attrutf, valutf);

 cleanup:
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  if (val && valutf)
    (*env)->ReleaseStringUTFChars(env, val, valutf);
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_testattrval__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring node, jstring attr, jstring val)
{
  const jbyte *nodeutf = NULL;
  const jbyte *attrutf = NULL;
  const jbyte *valutf = NULL;
  jboolean rv = JNI_FALSE;

  if (node)
    {
      if (!(nodeutf = (*env)->GetStringUTFChars(env, node, NULL)))
	goto cleanup;
    }

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  if (val)
    {
      if (!(valutf = (*env)->GetStringUTFChars(env, val, NULL)))
	goto cleanup;
    }

  rv = _testattrval (env, obj, nodeutf, attrutf, valutf);

 cleanup:
  if (node && nodeutf)
    (*env)->ReleaseStringUTFChars(env, node, nodeutf);
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  if (val && valutf)
    (*env)->ReleaseStringUTFChars(env, val, valutf);
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_isnode (JNIEnv *env, jobject obj, jstring node)
{
  genders_t handle;
  const jbyte *nodeutf = NULL;
  jboolean rv = JNI_FALSE;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if (node)
    {
      if (!(nodeutf = (*env)->GetStringUTFChars(env, node, NULL)))
	goto cleanup;
    }

  if ((ret = genders_isnode (handle, nodeutf)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    rv = JNI_TRUE;
  else
    rv = JNI_FALSE;

 cleanup:
  if (node && nodeutf)
    (*env)->ReleaseStringUTFChars(env, node, nodeutf);
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_isattr (JNIEnv *env, jobject obj, jstring attr)
{
  genders_t handle;
  const jbyte *attrutf = NULL;
  jboolean rv = JNI_FALSE;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  if ((ret = genders_isattr (handle, attrutf)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    rv = JNI_TRUE;
  else
    rv = JNI_FALSE;

 cleanup:
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_isattrval (JNIEnv *env, jobject obj, jstring attr, jstring val)
{
  genders_t handle;
  const jbyte *attrutf = NULL;
  const jbyte *valutf = NULL;
  jboolean rv = JNI_FALSE;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	goto cleanup;
    }

  if (val)
    {
      if (!(valutf = (*env)->GetStringUTFChars(env, val, NULL)))
	goto cleanup;
    }

  if ((ret = genders_isattrval (handle, attrutf, valutf)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    rv = JNI_TRUE;
  else
    rv = JNI_FALSE;

 cleanup:
  if (attr && attrutf)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);
  if (val && valutf)
    (*env)->ReleaseStringUTFChars(env, val, valutf);
  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_gov_llnl_lc_chaos_Genders_query (JNIEnv *env, jobject obj, jstring query)
{
  genders_t handle;
  const jbyte *queryutf = NULL;
  char **nodelist = NULL;
  int nodelistlen;
  jclass string_class = NULL;
  jobjectArray jnodelist = NULL;
  jobjectArray rv = NULL;
  int nodeslen;
  int i;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if (query)
    {
      if (!(queryutf = (*env)->GetStringUTFChars(env, query, NULL)))
	goto cleanup;
    }

  if ((nodelistlen = genders_nodelist_create (handle, &nodelist)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if ((nodeslen = genders_query (handle, nodelist, nodelistlen, queryutf)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  string_class = (*env)->FindClass(env, "java/lang/String");

  if (!(jnodelist = (*env)->NewObjectArray(env, nodeslen, string_class, NULL)))
    goto cleanup;

  for (i = 0; i < nodeslen; i++)
    {
      jstring tmpstr;
      
      if (!(tmpstr = (*env)->NewStringUTF(env, nodelist[i])))
	goto cleanup;

      (*env)->SetObjectArrayElement (env, jnodelist, i, tmpstr);
      (*env)->DeleteLocalRef (env, tmpstr);
    }

  rv = jnodelist;
 cleanup:
  if (!rv && jnodelist)
    (*env)->DeleteLocalRef (env, jnodelist);
  genders_nodelist_destroy (handle, nodelist);
  if (query && queryutf)
    (*env)->ReleaseStringUTFChars(env, query, queryutf);
  (*env)->DeleteLocalRef (env, string_class);
  return (rv);
}

static jboolean
_testquery (JNIEnv *env, jobject obj, const char *node, const char *query)
{
  genders_t handle;
  jboolean rv = JNI_FALSE;
  int ret;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((ret = genders_testquery (handle, node, query)) < 0)
    {
      _throw_exception (env, obj, genders_errnum (handle));
      goto cleanup;
    }

  if (ret)
    rv = JNI_TRUE;
  else
    rv = JNI_FALSE;

 cleanup:
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_testquery__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring query)
{
  const jbyte *queryutf = NULL;
  jboolean rv = JNI_FALSE;

  if (query)
    {
      if (!(queryutf = (*env)->GetStringUTFChars(env, query, NULL)))
	goto cleanup;
    }

  rv = _testquery (env, obj, NULL, queryutf);

 cleanup:
  if (query && queryutf)
    (*env)->ReleaseStringUTFChars(env, query, queryutf);
  return (rv);
}

JNIEXPORT jboolean JNICALL
Java_gov_llnl_lc_chaos_Genders_testquery__Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring node, jstring query)
{
  const jbyte *nodeutf = NULL;
  const jbyte *queryutf = NULL;
  jboolean rv = JNI_FALSE;

  if (node)
    {
      if (!(nodeutf = (*env)->GetStringUTFChars(env, node, NULL)))
	goto cleanup;
    }

  if (query)
    {
      if (!(queryutf = (*env)->GetStringUTFChars(env, query, NULL)))
	goto cleanup;
    }

  rv = _testquery (env, obj, nodeutf, queryutf);

 cleanup:
  if (node && nodeutf)
    (*env)->ReleaseStringUTFChars(env, node, nodeutf);
  if (query && queryutf)
    (*env)->ReleaseStringUTFChars(env, query, queryutf);
  return (rv);
}

JNIEXPORT void JNICALL
Java_gov_llnl_lc_chaos_Genders_cleanup (JNIEnv *env, jobject obj)
{
  genders_t handle;
  jclass genders_cls;
  jfieldID gh_addr_fid;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  genders_handle_destroy (handle);

  genders_cls = (*env)->GetObjectClass (env, obj);

  if (!(gh_addr_fid = (*env)->GetFieldID (env, genders_cls, "gh_addr", "J")))
    goto cleanup;

  (*env)->SetLongField (env, obj, gh_addr_fid, 0);

 cleanup:
  return;
}
