#include <jni.h>
#include "md5.c"
#define UTF_8 "UTF-8"
#ifdef __cplusplus

extern "C" {

jstring Java_com_masonsdk_jni_JNIUtil_getPwdOriginal(JNIEnv *, jobject,jobject);

}

#endif
