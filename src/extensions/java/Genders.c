#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <genders.h>

#include "Genders.h"

int
_constructor (JNIEnv *env, jobject obj, const char *filename)
{
  genders_t handle;
  jclass genders_cls;
  jfieldID gh_addr_fid;
  jint rv = -1;

  if (!(handle = genders_handle_create ()))
    {
      fprintf (stderr, "genders_handle_create: %s\n", strerror (errno));
      goto cleanup;
    }

  if (genders_load_data (handle, filename) < 0)
    {
      fprintf (stderr, "genders_load_data: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

  genders_cls = (*env)->GetObjectClass (env, obj);

  if (!(gh_addr_fid = (*env)->GetFieldID (env, genders_cls, "gh_addr", "J")))
    {
      fprintf (stderr, "gh_addr_fid GetFieldID error\n");
      goto cleanup;
    } 

  (*env)->SetLongField (env, obj, gh_addr_fid, (long)handle);

  rv = 0;
 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_Genders_genders_1constructor (JNIEnv *env, jobject obj, jstring filename)
{
  const jbyte *filenameutf;
  jint rv = -1;

  if (filename)
    {
      if (!(filenameutf = (*env)->GetStringUTFChars(env, filename, NULL)))
	{
	  fprintf (stderr, "GetStringUTFChars error\n");
	  goto cleanup;
	}
      
      rv = _constructor (env, obj, filenameutf);

      (*env)->ReleaseStringUTFChars(env, filename, filenameutf);
    }
  else
    _constructor (env, obj, NULL);

 cleanup:
  return (rv);
}

int
_get_handle (JNIEnv *env, jobject obj, genders_t *handle)
{
  jclass genders_cls;
  jfieldID gh_addr_fid;
  jlong jgh_addr_addr;
  int rv = -1;

  genders_cls = (*env)->GetObjectClass (env, obj);

  if (!(gh_addr_fid = (*env)->GetFieldID (env, genders_cls, "gh_addr", "J")))
    {
      fprintf (stderr, "gh_addr_fid GetFieldID error\n");
      goto cleanup;
    }
  
  jgh_addr_addr = (*env)->GetLongField (env, obj, gh_addr_fid);
  (*handle) = (genders_t)jgh_addr_addr;

  rv = 0;
 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_Genders_getnumnodes (JNIEnv *env, jobject obj)
{
  genders_t handle;
  int rv = -1;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((rv = genders_getnumnodes (handle)) < 0)
    {
      fprintf (stderr, "genders_getnumnodes: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_Genders_getnumattrs (JNIEnv *env, jobject obj)
{
  genders_t handle;
  int rv = -1;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((rv = genders_getnumattrs (handle)) < 0)
    {
      fprintf (stderr, "genders_getnumattrs: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_Genders_getmaxattrs (JNIEnv *env, jobject obj)
{
  genders_t handle;
  int rv = -1;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((rv = genders_getmaxattrs (handle)) < 0)
    {
      fprintf (stderr, "genders_getmaxattrs: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

 cleanup:
  return (rv);
}

JNIEXPORT jstring JNICALL
Java_Genders_getnodename (JNIEnv *env, jobject obj)
{
  genders_t handle;
  char *nodename_buf = NULL;
  int maxnodenamelen;
  jstring rv = NULL;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((maxnodenamelen = genders_getmaxnodelen (handle)) < 0)
    goto cleanup;

  if (!(nodename_buf = (char *)malloc (maxnodenamelen + 1)))
    {
      fprintf (stderr, "malloc: %s\n", strerror (errno));
      goto cleanup;
    }

  memset (nodename_buf, '\0', maxnodenamelen + 1);

  if (genders_getnodename (handle, nodename_buf, maxnodenamelen + 1) < 0)
    {
      fprintf (stderr, "genders_getnodename: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

  rv = (*env)->NewStringUTF(env, nodename_buf);
 cleanup:
  free (nodename_buf);
  return (rv);
}

jobjectArray
_getnodes (JNIEnv *env, jobject obj, const char *attr, const char *val)
{
  genders_t handle;
  char **nodelist = NULL;
  int nodelistlen;
  jclass string_class;
  jobjectArray rv = NULL;
  int nodeslen;
  int i;

  if (_get_handle (env, obj, &handle) < 0)
    goto cleanup;

  if ((nodelistlen = genders_nodelist_create (handle, &nodelist)) < 0)
    {
      fprintf (stderr, "genders_nodelist_create: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

  if ((nodeslen = genders_getnodes (handle, nodelist, nodelistlen, attr, val)) < 0)
    {
      fprintf (stderr, "genders_getnodes: %s\n", genders_errormsg (handle));
      goto cleanup;
    }

  string_class = (*env)->FindClass(env, "java/lang/String");

  if (!(rv = (*env)->NewObjectArray(env, nodeslen, string_class, NULL)))
    {
      fprintf (stderr, "NewObjectArray\n");
      goto cleanup;
    }

  for (i = 0; i < nodeslen; i++)
    {
      jstring tmpstr;
      
      if (!(tmpstr = (*env)->NewStringUTF(env, nodelist[i])))
	{
	  fprintf (stderr, "NewStringUTF\n");
	  goto cleanup;
	}

      (*env)->SetObjectArrayElement (env, rv, i, tmpstr);
    }

 cleanup:
  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_Genders_getnodes__ (JNIEnv *env, jobject obj)
{
  return (_getnodes (env, obj, NULL, NULL));
}

JNIEXPORT jobjectArray JNICALL
Java_Genders_getnodes__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr)
{
  const jbyte *attrutf = NULL;
  jobjectArray rv = NULL;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	{
	  fprintf (stderr, "GetStringUTFChars error\n");
	  goto cleanup;
	}
    }

  rv = _getnodes (env, obj, attrutf, NULL);

  if (attr)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);

 cleanup:
  return (rv);
}

JNIEXPORT jobjectArray JNICALL
Java_Genders_getnodes__Ljava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring attr, jstring val)
{
  const jbyte *attrutf = NULL;
  const jbyte *valutf = NULL;
  jobjectArray rv = NULL;

  if (attr)
    {
      if (!(attrutf = (*env)->GetStringUTFChars(env, attr, NULL)))
	{
	  fprintf (stderr, "GetStringUTFChars error\n");
	  goto cleanup;
	}
    }

  if (val)
    {
      if (!(valutf = (*env)->GetStringUTFChars(env, val, NULL)))
	{
	  fprintf (stderr, "GetStringUTFChars error\n");
	  goto cleanup;
	}
    }

  rv = _getnodes (env, obj, attrutf, valutf);

  if (attr)
    (*env)->ReleaseStringUTFChars(env, attr, attrutf);

  if (val)
    (*env)->ReleaseStringUTFChars(env, val, valutf);

 cleanup:
  return (rv);
}
