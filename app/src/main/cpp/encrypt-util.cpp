#include "encrypt-util.hpp"

//获取deviceid
jstring getDeviceId(JNIEnv *env, jobject thiz, jobject obj_Context) {
    jclass cls_Context = (env)->FindClass("android/content/Context");
    jmethodID methodID_getSystemService = (env)->GetMethodID(cls_Context,
                                                             "getSystemService",
                                                             "(Ljava/lang/String;)Ljava/lang/Object;");
    jfieldID jfieldID_TELEPHONY_SERVICE = (env)->GetStaticFieldID(cls_Context,
                                                                  "TELEPHONY_SERVICE",
                                                                  "Ljava/lang/String;");
    jobject obj_TELEPHONY_SERVICE = (env)->GetStaticObjectField(cls_Context,
                                                                jfieldID_TELEPHONY_SERVICE);
    jobject telephonymanager = (env)->CallObjectMethod(obj_Context,
                                                       methodID_getSystemService,
                                                       obj_TELEPHONY_SERVICE);
    jclass cls_TelephonyManager = (env)->FindClass("android/telephony/TelephonyManager");
    jmethodID methodID_getDeviceId = (env)->GetMethodID(cls_TelephonyManager,
                                                        "getDeviceId",
                                                        "()Ljava/lang/String;");
    jobject deviceid = (env)->CallObjectMethod(telephonymanager, methodID_getDeviceId);
    return (jstring) deviceid;
}

//获取公钥
jstring getPublicKey(JNIEnv *env, jobject thiz, jobject context) {
    jclass context_cls = env->GetObjectClass(context);
    jmethodID get_package_manager_mid = env->GetMethodID(context_cls,
                                                         "getPackageManager",
                                                         "()Landroid/content/pm/PackageManager;");
    jmethodID get_package_name_mid = env->GetMethodID(context_cls,
                                                      "getPackageName", "()Ljava/lang/String;");
    env->DeleteLocalRef(context_cls);
    jobject pm_obj = env->CallObjectMethod(context, get_package_manager_mid);
    jclass pm_cls = env->FindClass("android/content/pm/PackageManager");
    jmethodID get_package_info_mid = env->GetMethodID(pm_cls, "getPackageInfo",
                                                      "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jstring package_name = reinterpret_cast<jstring>(env->CallObjectMethod(
            context, get_package_name_mid));
    jfieldID flag_fid = env->GetStaticFieldID(pm_cls, "GET_SIGNATURES", "I");
    jint flag = env->GetStaticIntField(pm_cls, flag_fid);
    env->DeleteLocalRef(pm_cls);
    jobject pi_obj = env->CallObjectMethod(pm_obj, get_package_info_mid,
                                           package_name, flag);
    env->DeleteLocalRef(package_name);
    jclass pi_cls = env->FindClass("android/content/pm/PackageInfo");
    jfieldID signatures_fid = env->GetFieldID(pi_cls, "signatures",
                                              "[Landroid/content/pm/Signature;");
    env->DeleteLocalRef(pi_cls);
    jobject sig_obj = env->GetObjectField(pi_obj, signatures_fid);
    env->DeleteLocalRef(pi_obj);
    jobjectArray sigs = reinterpret_cast<jobjectArray>(sig_obj);

    jclass signature_cls = env->FindClass("android/content/pm/Signature");
    jmethodID to_byte_array_mid = env->GetMethodID(signature_cls, "toByteArray",
                                                   "()[B");
    jbyteArray sig_bytes = reinterpret_cast<jbyteArray>(env->CallObjectMethod(
            env->GetObjectArrayElement(sigs, 0), to_byte_array_mid));
    jclass certificate_factory_cls = env->FindClass(
            "java/security/cert/CertificateFactory");
    jmethodID get_certificate_instance_mid = env->GetStaticMethodID(
            certificate_factory_cls, "getInstance",
            "(Ljava/lang/String;)Ljava/security/cert/CertificateFactory;");
    jobject certificate_factory_obj = env->CallStaticObjectMethod(
            certificate_factory_cls, get_certificate_instance_mid,
            env->NewStringUTF("X509"));
    jmethodID generate_certificate_mid = env->GetMethodID(
            certificate_factory_cls, "generateCertificate",
            "(Ljava/io/InputStream;)Ljava/security/cert/Certificate;");
    env->DeleteLocalRef(certificate_factory_cls);
    jclass certificate_cls = env->FindClass("java/security/cert/Certificate");
    jclass byte_input_stream_cls = env->FindClass(
            "java/io/ByteArrayInputStream");
    jmethodID new_sig_bytes_is_mid = env->GetMethodID(byte_input_stream_cls,
                                                      "<init>", "([B)V");
    jobject sig_bytes_is = env->NewObject(byte_input_stream_cls,
                                          new_sig_bytes_is_mid, sig_bytes);
    env->DeleteLocalRef(sig_bytes);
    env->DeleteLocalRef(byte_input_stream_cls);
    jobject cert = env->CallObjectMethod(certificate_factory_obj,
                                         generate_certificate_mid, sig_bytes_is);
    env->DeleteLocalRef(sig_bytes_is);
    env->DeleteLocalRef(certificate_factory_obj);
    jmethodID get_pubic_key_mid = env->GetMethodID(certificate_cls,
                                                   "getPublicKey", "()Ljava/security/PublicKey;");
    env->DeleteLocalRef(certificate_cls);

    jobject publicKey = env->CallObjectMethod(cert, get_pubic_key_mid);
    jclass publicKey_cls = env->GetObjectClass(publicKey);
    jmethodID toString_mid = env->GetMethodID(publicKey_cls, "toString", "()Ljava/lang/String;");
    jstring publicKey_str = static_cast<jstring>(env->CallObjectMethod(publicKey, toString_mid));
    env->DeleteLocalRef(cert);
    env->DeleteLocalRef(publicKey_cls);
    env->DeleteLocalRef(publicKey);

    jclass string_cls = env->GetObjectClass(publicKey_str);
    jmethodID indexOf_mid = env->GetMethodID(string_cls, "indexOf", "(Ljava/lang/String;)I");
    jstring param = env->NewStringUTF("modulus");
    jint aa = env->CallIntMethod(publicKey_str, indexOf_mid, param);
    jstring param2 = env->NewStringUTF("publicExponent");
    jint bb = env->CallIntMethod(publicKey_str, indexOf_mid, param2);

    jmethodID substring_mid = env->GetMethodID(string_cls, "substring", "(II)Ljava/lang/String;");

    jstring publicKey2_str = static_cast<jstring>(env->CallObjectMethod(publicKey_str,
                                                                        substring_mid, aa + 8,
                                                                        bb - 1));

    return publicKey2_str;
}

//获取签名
jstring getSignatures(JNIEnv *env, jobject thizz,
                      jobject thiz) {

    jclass native_clazz = env->GetObjectClass(thiz);

    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_func = env->GetMethodID(native_clazz,
                                               "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");

    // 获得应用包的管理器
    jobject package_manager = env->CallObjectMethod(thiz, methodID_func);

    // 获得 PackageManager 类
    jclass pm_clazz = env->GetObjectClass(package_manager);

    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = env->GetMethodID(pm_clazz, "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    //获取包名
    jmethodID methodID_packagename = env->GetMethodID(native_clazz,
                                                      "getPackageName", "()Ljava/lang/String;");
    jstring name_str = static_cast<jstring>(env->CallObjectMethod(thiz,
                                                                  methodID_packagename));

    // 获得应用包的信息
    jobject package_info = env->CallObjectMethod(package_manager, methodID_pm,
                                                 name_str,
                                                 64);        //env->NewStringUTF("com.example.contasdf")

    // 获得 PackageInfo 类
    jclass pi_clazz = env->GetObjectClass(package_info);

    // 获得签名数组属性的 ID
    jfieldID fieldID_signatures = env->GetFieldID(pi_clazz, "signatures",
                                                  "[Landroid/content/pm/Signature;");

    // 得到签名数组，待修改
    jobject signatur = env->GetObjectField(package_info, fieldID_signatures);
    jobjectArray signatures = reinterpret_cast<jobjectArray>(signatur);
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

    jstring sign = env->NewStringUTF(str);

    return sign;
}

jstring getPackageName(JNIEnv *env, jobject thizz, jobject thiz) {

    jclass native_clazz = env->GetObjectClass(thiz);

    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_func = env->GetMethodID(native_clazz,
                                               "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");

    // 获得应用包的管理器
    jobject package_manager = env->CallObjectMethod(thiz, methodID_func);

    // 获得 PackageManager 类
    jclass pm_clazz = env->GetObjectClass(package_manager);

    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = env->GetMethodID(pm_clazz, "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    //获取包名
    jmethodID methodID_packagename = env->GetMethodID(native_clazz,
                                                      "getPackageName", "()Ljava/lang/String;");
    jstring name_str = static_cast<jstring>(env->CallObjectMethod(thiz,
                                                                  methodID_packagename));

    return name_str;
}


//获取SerialNumber
jstring getSerialNumber(JNIEnv *env, jobject thiz, jobject mContext) {
    jclass cls_tm = (env)->FindClass("android/os/SystemProperties");
    if (cls_tm == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jmethodID getDeviceId = (env)->GetStaticMethodID(cls_tm, "get",
                                                     "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if (getDeviceId == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jstring param1 = (env)->NewStringUTF("ro.serialno");
    jstring param2 = (env)->NewStringUTF("unknown");
    jobject deviceid = (env)->CallStaticObjectMethod(cls_tm, getDeviceId,
                                                     param1, param2);
    return (jstring) deviceid;
}

jstring jint2jstring(JNIEnv *env, jint first) {
    char buf[64]; // assumed large enough to cope with result
    sprintf(buf, "%d", first);  // error checking omitted
    return env->NewStringUTF(buf);
}

//char* to jstring
jstring toString(JNIEnv *env, jbyteArray byteArray) {
    jclass string_cls = env->FindClass("java/lang/String");
    jmethodID new_string_mid = env->GetMethodID(string_cls, "<init>",
                                                "([BLjava/lang/String;)V");
    return reinterpret_cast<jstring>(env->NewObject(string_cls, new_string_mid,
                                                    byteArray, env->NewStringUTF(UTF_8)));
}

jbyteArray toBytes(JNIEnv *env, const char *bytes) {
    jclass string_cls = env->FindClass("java/lang/String");
    jmethodID get_bytes_mid = env->GetMethodID(string_cls, "getBytes",
                                               "(Ljava/lang/String;)[B");
    return reinterpret_cast<jbyteArray>(env->CallObjectMethod(
            env->NewStringUTF(bytes), get_bytes_mid, env->NewStringUTF(UTF_8)));
}

jbyteArray toBytes(JNIEnv *env, jstring string) {
    jclass string_cls = env->FindClass("java/lang/String");
    jmethodID get_bytes_mid = env->GetMethodID(string_cls, "getBytes",
                                               "(Ljava/lang/String;)[B");
    return reinterpret_cast<jbyteArray>(env->CallObjectMethod(string,
                                                              get_bytes_mid,
                                                              env->NewStringUTF(UTF_8)));
}

jbyteArray getDigestedBytes(JNIEnv *env, jbyteArray complex_bytes) {
    static jobject satic_message_digest_obj = __null;
    jclass message_digest_cls = env->FindClass("java/security/MessageDigest");
    jmethodID get_instance_mid = env->GetStaticMethodID(message_digest_cls,
                                                        "getInstance",
                                                        "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    if (satic_message_digest_obj == __null) {
        jobject local_message_digest_obj = env->CallStaticObjectMethod(
                message_digest_cls, get_instance_mid, env->NewStringUTF("MD5"));
        satic_message_digest_obj = env->NewGlobalRef(local_message_digest_obj);
        env->DeleteLocalRef(local_message_digest_obj);
    }
    jmethodID digest_mid = env->GetMethodID(message_digest_cls, "digest",
                                            "([B)[B");
    env->DeleteLocalRef(message_digest_cls);
    return reinterpret_cast<jbyteArray>(env->CallObjectMethod(
            satic_message_digest_obj, digest_mid, complex_bytes));
}


jstring getMD5(JNIEnv *env, jstring jInfo) {
    jbyteArray digested_bytes = getDigestedBytes(env, toBytes(env, jInfo));
    return toHex(env, digested_bytes);
}

jstring toHex(JNIEnv *env, jbyteArray digested_bytes) {
    jclass big_integer_cls = env->FindClass("java/math/BigInteger");
    jmethodID new_big_integer_mid = env->GetMethodID(big_integer_cls, "<init>",
                                                     "(I[B)V");
    jobject big_integer_obj = env->NewObject(big_integer_cls,
                                             new_big_integer_mid, 1, digested_bytes);
    env->DeleteLocalRef(digested_bytes);
    jmethodID to_String_mid = env->GetMethodID(big_integer_cls, "toString",
                                               "(I)Ljava/lang/String;");
    env->DeleteLocalRef(big_integer_cls);
    return reinterpret_cast<jstring>(env->CallObjectMethod(big_integer_obj,
                                                           to_String_mid, 16));
}

jstring getAppendedString(JNIEnv *env, jobject thiz, jstring s1, jstring s2) {

    const char *s1x = (env)->GetStringUTFChars(s1, NULL);
    const char *s2x = (env)->GetStringUTFChars(s2, NULL);

    char *sall = new char[strlen(s1x) + strlen(s2x) + 1];
    strcpy(sall, s1x);
    strcat(sall, s2x);

    jstring retval = (env)->NewStringUTF(sall);

    (env)->ReleaseStringUTFChars(s1, s1x);
    (env)->ReleaseStringUTFChars(s2, s2x);
    free(sall);

    return retval;
}

jobject getInstance(JNIEnv *env, jclass obj_class) {

    jmethodID construction_id = env->GetMethodID(obj_class, "<init>", "()V");

    jobject obj = env->NewObject(obj_class, construction_id);

    return obj;

}