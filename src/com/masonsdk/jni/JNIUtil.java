package com.masonsdk.jni;

import android.content.Context;

public class JNIUtil {
	static {
		System.loadLibrary("masonsdkjni");
	}

	public static native String getPWD(Context con);

	public native String stringFromJNI2(String str);

}
