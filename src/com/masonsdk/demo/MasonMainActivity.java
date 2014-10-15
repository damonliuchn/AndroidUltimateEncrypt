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

		 try {
			 PackageInfo info =getPackageManager().getPackageInfo(getPackageName(), 64);
			 //签名就是用私钥加密后的字符串
			 Signature sign = info.signatures[0];
			 
			 Log.e("test", "hashCode : " + sign.hashCode());
			 Log.e("test", "hashCodestring : " + sign.toCharsString());
//			 EditText str2 = (EditText) this.findViewById(R.id.str2);
//			 str2.setText(""+tm.getDeviceId());
		 } catch (NameNotFoundException e) {
			 e.printStackTrace();
		 }
		 
		 
		 TelephonyManager tm = (TelephonyManager) getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
		 
		 //必须有sim卡
		 String tmSerial = "" + tm.getSimSerialNumber();
		 //刷机或wipe会变
		 String androidId = "" + android.provider.Settings.Secure.getString(getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
		 //2.3以下没有 或者 返回unknown
		 String SerialNumber = android.os.Build.SERIAL;
		 
		 String serialnum = "";
		 try {
			 Class<?> c = Class.forName("android.os.SystemProperties");
			 Method get = c.getMethod("get", String.class, String.class );
			 serialnum = (String)( get.invoke(c, "ro.serialno", "unknown" ) );
		 }
		 catch (Exception ignored)
		 {
		 }
		 //必须开机打开过wifi
		 WifiManager manager = (WifiManager) getSystemService(Context.WIFI_SERVICE);  
		 String macAddress="";
	     if (manager != null) {  
	            macAddress = manager.getConnectionInfo().getMacAddress(); 
		 
	     }
		 TextView str3 = (TextView) this.findViewById(R.id.str3);
		 str3.setText(""+tm.getDeviceId()+"\n"+tmSerial+"\n"+androidId+"\n"+SerialNumber+"\n"+serialnum+"\n"+macAddress);
	}

	
}
