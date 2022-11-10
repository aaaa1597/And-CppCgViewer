package com.tks.cppcgviewer;

import android.content.res.AssetManager;
import android.view.Surface;

public class Jni {
    static {
        System.loadLibrary("cppcgviewer");
    }
    public static native boolean onStart(AssetManager assets, String[] modelnames, String[] md2filenames, String[] texfilenames, String[] vshfilenames, String[] fshfilenames, float[] rotxs, float[] rotys, float[] rotzs, float[] posxs, float[] posys, float[] poszs, float[] sclxs, float[] sclys, float[] sclzs);
    public static native void surfaceCreated(Surface surface);
    public static native void surfaceChanged(int width, int height);
    public static native void surfaceDestroyed();
    public static native void onStop();
    public static native void setScale(float scale);
    public static native void setRotate(float mTouchAngleX, float mTouchAngleY);
}
