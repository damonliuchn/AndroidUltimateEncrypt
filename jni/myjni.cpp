#include <string>
#include <stdio.h>
#include <wchar.h>
#include "myjni.h"

//char* to jstring

jstring getMD5(JNIEnv* env, jstring jInfo) {
//		MD5_CTX md5;
//	    MD5Init(&md5);
	    int i;
//	    unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
//	    //const char *a  = (env)->GetStringUTFChars(jInfo, NULL);
//	    //unsigned char * encrypt = (unsigned char *)a;
//	    unsigned char decrypt[16];
//	    MD5Update(&md5,encrypt,strlen((char *)encrypt));
//	    MD5Final(&md5,decrypt);
////	    printf("加密前:%s\n加密后:",encrypt);
//
//
//	    char destination[16]={0};
//	    for (i = 0; i < 16; i++) {
//	            sprintf(destination, "%s%02x", destination, decrypt[i]);
//	    }
//
//
//
//
//	    //const char* b = static_cast<const char*>( static_cast<void*>( const_cast<unsigned char*>(decrypt)));
//	    unsigned char h[16]="23445F";
//	    //		  jstring test2 = env->NewString(szDigest,wcslen(szDigest));
//	    		  //unsigned char * ucptest=L"somestring";
//	    jstring test1;
//	    std::string str(reinterpret_cast<const char*>(decrypt));
//	    test1 = env->NewStringUTF(str.c_str());
//	    return env->NewStringUTF(destination);


		  unsigned char szDigest[16];
		  char encrypt[200]="123456";
		  MD5Digest(encrypt,strlen(encrypt),szDigest);
//		  int i;
//		  for (i=0;i<16;i++)
//			  printf ("%02X",(unsigned char)szDigest[i]);

//		  unsigned char h[16]="23445F";
////		  jstring test2 = env->NewString(szDigest,wcslen(szDigest));
//		  //unsigned char * ucptest=L"somestring";
//		  jstring test1;
//		  std::string str(reinterpret_cast<const char*>(szDigest));
//		  test1 = env->NewStringUTF(str.c_str());

		  char destination[16]={0};
		  for (i = 0; i < 8; i++) {
		  	     sprintf(destination, "%s%02x", destination, szDigest[i]);
		  }
		  return env->NewStringUTF(destination);
}
jstring getAppendedString(JNIEnv* env,jobject thiz, jstring s1,jstring s2) {

    const char *s1x  = (env)->GetStringUTFChars(s1, NULL);
    const char *s2x  = (env)->GetStringUTFChars(s2, NULL);

    char *sall  = new char[strlen(s1x) + strlen(s2x) + 1];
    strcpy(sall, s1x);
    strcat(sall, s2x);

    jstring retval = (env)->NewStringUTF(sall);

    (env)->ReleaseStringUTFChars(s1, s1x);
    (env)->ReleaseStringUTFChars(s2, s2x);
    free(sall);

    return retval;
}

jobject getInstance(JNIEnv* env, jclass obj_class){

    jmethodID construction_id = env->GetMethodID(obj_class, "<init>", "()V");

    jobject obj = env->NewObject(obj_class, construction_id);

    return obj;

}
//获取deviceid
jstring getDeviceID(JNIEnv *env,jobject thiz,jobject mContext)
{
	jclass cls_context = (env)->FindClass("android/content/Context");
	if(cls_context == 0){
		return (env)->NewStringUTF("unknown");
	}
	jmethodID getSystemService = (env)->GetMethodID(cls_context, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	if(getSystemService == 0){
		return (env)->NewStringUTF("unknown");
	}
	jfieldID TELEPHONY_SERVICE = (env)->GetStaticFieldID( cls_context, "TELEPHONY_SERVICE", "Ljava/lang/String;");
	if(TELEPHONY_SERVICE == 0){
		return (env)->NewStringUTF("unknown");
	}
	jobject str = (env)->GetStaticObjectField( cls_context, TELEPHONY_SERVICE);
	jobject telephonymanager = (env)->CallObjectMethod( mContext, getSystemService, str);
	if(telephonymanager == 0){
		return (env)->NewStringUTF("unknown");
	}
	jclass cls_tm = (env)->FindClass( "android/telephony/TelephonyManager");
	if(cls_tm == 0){
		return (env)->NewStringUTF("unknown");
	}
	jmethodID getDeviceId = (env)->GetMethodID( cls_tm, "getDeviceId", "()Ljava/lang/String;");
	if(getDeviceId == 0){
		return (env)->NewStringUTF("unknown");
	}
	jobject deviceid = (env)->CallObjectMethod( telephonymanager, getDeviceId);
	return (jstring)deviceid;
}
//获取SerialNumber
jstring getSerialNumber(JNIEnv *env,jobject thiz,jobject mContext)
{
	jclass cls_tm = (env)->FindClass( "android/os/SystemProperties");
	if(cls_tm == 0){
			return (env)->NewStringUTF("unknown");
	}
	jmethodID getDeviceId = (env)->GetStaticMethodID( cls_tm, "get", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	if(getDeviceId == 0){
		return (env)->NewStringUTF("unknown");
	}
	jstring param1 = (env)->NewStringUTF("ro.serialno");
	jstring param2 = (env)->NewStringUTF("unknown");
	jobject deviceid = (env)->CallStaticObjectMethod(cls_tm, getDeviceId,param1, param2);
	return (jstring)deviceid;
}
jstring Java_com_masonsdk_jni_JNIUtil_getPWD(JNIEnv* env, jobject thizz,jobject thiz){


		jclass native_clazz = env->GetObjectClass(thiz);

	    // 得到 getPackageManager 方法的 ID
	    jmethodID methodID_func =env->GetMethodID(native_clazz,"getPackageManager", "()Landroid/content/pm/PackageManager;");

	    // 获得应用包的管理器
	    jobject package_manager = env->CallObjectMethod(thiz, methodID_func);

	    // 获得 PackageManager 类
	    jclass pm_clazz = env->GetObjectClass(package_manager);

	    // 得到 getPackageInfo 方法的 ID
	    jmethodID methodID_pm = env->GetMethodID( pm_clazz,
	            "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

	    //获取包名
	    jmethodID methodID_packagename =env->GetMethodID(native_clazz,"getPackageName", "()Ljava/lang/String;");
	    jstring name_str = static_cast<jstring>(env->CallObjectMethod(thiz, methodID_packagename));



	    // 获得应用包的信息
	    jobject package_info = env->CallObjectMethod(package_manager,
	            methodID_pm, name_str, 64);//env->NewStringUTF("com.example.contasdf")

	    // 获得 PackageInfo 类
	    jclass pi_clazz = env->GetObjectClass( package_info);

	    // 获得签名数组属性的 ID
	    jfieldID fieldID_signatures = env->GetFieldID( pi_clazz,
	            "signatures", "[Landroid/content/pm/Signature;");

	    // 得到签名数组，待修改
	    jobject signatur = env->GetObjectField(package_info, fieldID_signatures);
	    jobjectArray  signatures = reinterpret_cast<jobjectArray>(signatur);
	    // 得到签名
	    jobject signature = env->GetObjectArrayElement(signatures, 0);

	    // 获得 Signature 类，待修改
	    jclass s_clazz = env->GetObjectClass(signature);

	    // 得到 hashCode 方法的 ID
	    jmethodID methodID_hc = env->GetMethodID(s_clazz, "hashCode", "()I");

	    // 获得应用包的管理器，待修改
	    int hash_code = env->CallIntMethod(signature, methodID_hc);

	    char str[100];
	    sprintf(str, "%u", hash_code);

	    jstring imei=getAppendedString(env, thizz,getDeviceID(env, thizz,thiz),getSerialNumber(env, thizz,thiz));
	    jstring sign=env->NewStringUTF(str);
	    jstring imei_sign=getAppendedString(env, thizz,imei,sign);
	    jstring imei_sign_package=getAppendedString(env,thizz,imei_sign,name_str);
	    return getMD5(env,imei_sign_package);
	    //return imei_sign_package;
}
