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
  jfieldID gh_fid;
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

  gh_fid = (*env)->GetFieldID (env, genders_cls, "gh", "J");
  if (!gh_fid)
    {
      fprintf (stderr, "gh_fid GetFieldID error\n");
      goto cleanup;
    } 

  (*env)->SetLongField (env, obj, gh_fid, (long)handle);

  rv = 0;
 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_Genders_genders_1constructor_1default (JNIEnv *env, jobject obj)
{
  return (_constructor (env, obj, NULL));
}

JNIEXPORT jint JNICALL
Java_Genders_genders_1constructor_1filename (JNIEnv *env, jobject obj, jstring filename)
{
  const jbyte *filenameutf;
  jint rv = -1;

  if (filename)
    {
      filenameutf = (*env)->GetStringUTFChars(env, filename, NULL);
      if (!filenameutf)
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
  jfieldID gh_fid;
  jlong jgh_addr;
  int rv = -1;

  genders_cls = (*env)->GetObjectClass (env, obj);

  gh_fid = (*env)->GetFieldID (env, genders_cls, "gh", "J");
  if (!gh_fid)
    {
      fprintf (stderr, "gh_fid GetFieldID error\n");
      goto cleanup;
    }
  
  jgh_addr = (*env)->GetLongField (env, obj, gh_fid);
  (*handle) = (genders_t)jgh_addr;

  rv = 0;
 cleanup:
  return (rv);
}

JNIEXPORT jint JNICALL
Java_Genders_getnumnodes (JNIEnv *env, jobject obj)
{
  genders_t handle;
  long gh_addr;
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
  long gh_addr;
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
  long gh_addr;
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
