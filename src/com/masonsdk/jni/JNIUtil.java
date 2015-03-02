package com.masonsdk.jni;

import android.content.Context;

public class JNIUtil {
	static {
		System.loadLibrary("masonsdkjni");
	}
	private static String pwd;
	public static String getPwd(Context con){
		if (pwd==null) {
			pwd=getPwdOriginal(con).substring(8, 24);
		}
		return pwd;
	}
	
	private static native String getPwdOriginal(Context con);

}
