package com.masonsdk.demo;

import com.masonsdk.R;
import com.masonsdk.jni.JNIUtil;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MasonMainActivity extends Activity {
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		EditText str1 = (EditText) this.findViewById(R.id.str1);
		str1.setText(JNIUtil.getPWD(this));

		// try {
		// PackageInfo info =
		// getPackageManager().getPackageInfo(getPackageName(), 64);
		// //签名就是用私钥加密后的字符串
		// Signature sign = info.signatures[0];
		//
		// Log.e("test", "hashCode : " + sign.hashCode());
		// Log.e("test", "hashCodestring : " + sign.toCharsString());
		// Toast.makeText(this,sign.toCharsString(), Toast.LENGTH_LONG).show();
		// } catch (NameNotFoundException e) {
		// e.printStackTrace();
		// }

	}

	
}
