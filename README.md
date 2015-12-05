
# AndroidUltimateEncrypt



##作者：Mason Liu           天盟

## 什么是 AndroidUltimateEncrypt

AndroidUltimateEncrypt AndroidJNI Encrypt ，运用JNI读取公钥、应用包名、设备号（deviceid+seri  number），然后把得到的数据进行一系列自定义的加密（这个加密方法需要你自己写，字符替换或移位都行），最后再md5得到一个密码，可以将该密码作为AES的密钥。

用途：本项目主要适用于 某些App对存储在用户手机上的信息安全性要求比较高的情况，使用本项目后即使有人拿到用户手机也无法解密信息，因为密钥是指定的App运行时生成的。



##欢迎批评指正

博客：http://blog.csdn.net/masonblog
邮箱：MasonLiuChn@gmail.com








