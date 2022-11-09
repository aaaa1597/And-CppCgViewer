package com.tks.cppcgviewer;

import android.content.res.AssetManager;
import android.view.Surface;

public class Jni {
    static {
        System.loadLibrary("cppcgviewer");
    }
    public static native void onStart();
    public static native void surfaceCreated(Surface surface);
    public static native void surfaceChanged(int width, int height);
    public static native void surfaceDestroyed();
    public static native void onStop();
}
