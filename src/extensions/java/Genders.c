#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <genders.h>

#include "Genders.h"

/*
 * Class:     Genders
 * Method:    genders_handle_setup
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_Genders_genders_1handle_1setup (JNIEnv *env, jobject obj)
{
  genders_t handle;
  jclass genders_cls;
  jfieldID gh_fid;
  jint rv = -1;

  if (!(handle = genders_handle_create()))
    {
      fprintf (stderr, "genders_handle_create: %s\n", strerror (errno));
      goto cleanup;
    }

  fprintf (stderr, "In C - handle = %p\n", handle);

  genders_cls = (*env)->GetObjectClass(env, obj);

  gh_fid = (*env)->GetFieldID(env, genders_cls, "gh", "J");
  if (!gh_fid)
    {
      fprintf (stderr, "gh_fid GetFieldID error\n");
      goto cleanup;
    } 

  (*env)->SetLongField(env, obj, gh_fid, (long)handle);

  rv = 0;
 cleanup:
  return rv;
}
