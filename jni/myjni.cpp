#include <myjni.h>
#include <string.h>
#include <stdio.h>


jobject getInstance(JNIEnv* env, jclass obj_class){

    jmethodID construction_id = env->GetMethodID(obj_class, "<init>", "()V");

    jobject obj = env->NewObject(obj_class, construction_id);

    return obj;

}

jstring Java_com_masonsdk_jni_JNIUtil_getPWD(JNIEnv* env, jobject thizz
		,jobject thiz
		){


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
	    return env->NewStringUTF(str);




}

