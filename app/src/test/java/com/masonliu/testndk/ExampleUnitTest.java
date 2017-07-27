package com.masonliu.testndk;

import android.content.Context;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    static {
        System.loadLibrary("native-lib");//app\build\intermediates\cmake\debug
    }
    @Test
    public void addition_isCorrect() throws Exception {
        String a = MainActivity.stringFromJNI(null);
        System.out.print(a);
        assertEquals(4, 2 + 2);
    }
}