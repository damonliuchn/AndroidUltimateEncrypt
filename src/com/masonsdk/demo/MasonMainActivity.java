package com.masonsdk.demo;

import java.lang.reflect.Method;

import com.masonsdk.R;
import com.masonsdk.jni.JNIUtil;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MasonMainActivity extends Activity {
	/** Called when the activity is first created. */
	@SuppressLint("NewApi")
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		EditText str1 = (EditText) this.findViewById(R.id.str1);
		str1.setText(JNIUtil.getPwd(this));

//		 try {
//			 PackageInfo info =getPackageManager().getPackageInfo(getPackageName(), 64);
//			 Signature sign = info.signatures[0];
//			 Log.e("test", "hashCode : " + sign.hashCode());
//			 Log.e("test", "hashCodestring : " + sign.toCharsString());
//		 } catch (NameNotFoundException e) {
//			 e.printStackTrace();
//		 }
	}

	
}
