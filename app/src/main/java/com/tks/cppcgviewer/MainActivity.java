package com.tks.cppcgviewer;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import com.google.android.material.snackbar.Snackbar;
import com.tks.cppcgviewer.databinding.ActivityMainBinding;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding binding;
    private ScaleGestureDetector mPinchDetector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        SurfaceView surface = binding.surfaceview;;
        /* 透過設定 */
        surface.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        surface.setZOrderOnTop(true);

        /* コールバック設定 */
        surface.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Jni.surfaceCreated(holder.getSurface());
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                mHeight = height;
                Jni.surfaceChanged(width, height);
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Jni.surfaceDestroyed();
            }
        });

        /* ピンチジェスチャー定義 */
        mPinchDetector = new ScaleGestureDetector(this.getApplicationContext(), new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override
            public boolean onScaleBegin(ScaleGestureDetector detector) {
                return super.onScaleBegin(detector);
            }

            @Override
            public boolean onScale(ScaleGestureDetector detector) {
                float scale = detector.getScaleFactor();
                if(scale > 5) scale = 5;
                else if(scale < 0.2) scale = 0.2f;
                Jni.setScale(scale);
                return true;
            }

            @Override
            public void onScaleEnd(ScaleGestureDetector detector) {
                super.onScaleEnd(detector);
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();

        /* モデルindexファイルs(model-index.json)を取得 */
        HashMap<String, ModelIndex> md2modelindex = new HashMap<>();
        try {
            /* indexファイル読込み */
            InputStream fileInputStream = getAssets().open("model-index.json");
            byte[] readBytes = new byte[fileInputStream.available()];
            fileInputStream.read(readBytes);
            String readString = new String(readBytes);
            fileInputStream.close();
            Log.i("index-content:", readString);
            /* jsonパース */
            JSONObject jsonObject = new JSONObject(readString);
            /* jsonパース(md2モデル) */
            JSONArray jsonmd2array = jsonObject.getJSONArray("md2models");
            for(int lpct = 0; lpct < jsonmd2array.length(); lpct++) {
                JSONObject md2model = jsonmd2array.getJSONObject(lpct);
                ModelIndex mi = new ModelIndex() {{
                    modelname = md2model.getString("name");
                    md2filename = md2model.getString("model");
                    texfilename = md2model.getString("tex");
                    vshfilename = md2model.getString("vsh");
                    fshfilename = md2model.getString("fsh");
                    rotx = (float) md2model.getDouble("rotx");
                    roty = (float) md2model.getDouble("roty");
                    rotz = (float) md2model.getDouble("rotz");
                    posx = (float) md2model.getDouble("posx");
                    posz = (float) md2model.getDouble("posy");
                    posz = (float) md2model.getDouble("posz");
                    sclx = (float) md2model.getDouble("sclx");
                    scly = (float) md2model.getDouble("scly");
                    sclz = (float) md2model.getDouble("sclz");
                }};
                md2modelindex.put(mi.modelname, mi);
            }
            /* jsonパース(mqoモデル) */
            JSONArray jsonmqoarray = jsonObject.getJSONArray("mqomodels");
            for(int lpct = 0; lpct < jsonmqoarray.length(); lpct++) {
                JSONObject md2model = jsonmqoarray.getJSONObject(lpct);
                ModelIndex mi = new ModelIndex() {{
                    modelname  =md2model.getString("name");
                    md2filename=md2model.getString("model");
                    texfilename=md2model.getString("tex");
                    vshfilename=md2model.getString("vsh");
                    fshfilename=md2model.getString("fsh");
                    rotx=(float)md2model.getDouble("rotx");
                    roty=(float)md2model.getDouble("roty");
                    rotz=(float)md2model.getDouble("rotz");
                    posx=(float)md2model.getDouble("posx");
                    posz=(float)md2model.getDouble("posy");
                    posz=(float)md2model.getDouble("posz");
                    sclx=(float)md2model.getDouble("sclx");
                    scly=(float)md2model.getDouble("scly");
                    sclz=(float)md2model.getDouble("sclz");
                }};
                md2modelindex.put(mi.modelname, mi);
            }
        }
        catch(IOException | JSONException e) {
            e.printStackTrace();
            Snackbar.make(binding.getRoot(), "初期化に失敗しました!!", Snackbar.LENGTH_SHORT).show();
            new Handler().postDelayed(() -> MainActivity.this.finish(), 10000);
        }

        Log.d("aaaaa", "model数=" + md2modelindex.size());
        for (Map.Entry<String, ModelIndex> item : md2modelindex.entrySet())
            System.out.println(item.getKey() + " => " + item.getValue().md2filename + " : " + item.getValue().texfilename + " : " + item.getValue().vshfilename + " : " + item.getValue().fshfilename);

        /* cpp側 初期化 */
        String[] modelnames   = new String[md2modelindex.size()];
        String[] md2filenames = new String[md2modelindex.size()];
        String[] texfilenames = new String[md2modelindex.size()];
        String[] vshfilenames = new String[md2modelindex.size()];
        String[] fshfilenames = new String[md2modelindex.size()];
        float[] rotxs = new float[md2modelindex.size()];
        float[] rotys = new float[md2modelindex.size()];
        float[] rotzs = new float[md2modelindex.size()];
        float[] posxs = new float[md2modelindex.size()];
        float[] posys = new float[md2modelindex.size()];
        float[] poszs = new float[md2modelindex.size()];
        float[] sclxs = new float[md2modelindex.size()];
        float[] sclys = new float[md2modelindex.size()];
        float[] sclzs = new float[md2modelindex.size()];
        int lpct = 0;
        for(Map.Entry<String, ModelIndex> item : md2modelindex.entrySet()) {
            modelnames  [lpct] = item.getKey();
            md2filenames[lpct] = item.getValue().md2filename;
            texfilenames[lpct] = item.getValue().texfilename;
            vshfilenames[lpct] = item.getValue().vshfilename;
            fshfilenames[lpct] = item.getValue().fshfilename;
            rotxs[lpct] = item.getValue().rotx;
            rotys[lpct] = item.getValue().roty;
            rotzs[lpct] = item.getValue().rotz;
            posxs[lpct] = item.getValue().posx;
            posys[lpct] = item.getValue().posy;
            poszs[lpct] = item.getValue().posz;
            sclxs[lpct] = item.getValue().sclx;
            sclys[lpct] = item.getValue().scly;
            sclzs[lpct] = item.getValue().sclz;
            lpct++;
        }

        /* onStart */
        Jni.onStart(getResources().getAssets(), modelnames, md2filenames, texfilenames, vshfilenames, fshfilenames, rotxs, rotys, rotzs, posxs, posys, poszs, sclxs, sclys, sclzs );
    }

    @Override
    protected void onStop() {
        super.onStop();
        Jni.onStop();
    }

    private float mTouchAngleX, mTouchAngleY;
    private float mLastX,mLastY;
    private int mHeight;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if(event.getPointerCount() == 2)
            return mPinchDetector.onTouchEvent(event);

        switch(event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mLastX = event.getX();
                mLastY = event.getY();
                return true;
            case MotionEvent.ACTION_MOVE:
            {
                float factor = 100f / mHeight;
                float dx = factor * (event.getX() - mLastX);
                float dy = factor * (event.getY() - mLastY);
                mTouchAngleX = Math.max(Math.min(mTouchAngleX+dx,90f),-90f);
                mTouchAngleY += dy;
                Jni.setRotate(mTouchAngleX, mTouchAngleY);
                mLastX = event.getX();
                mLastY = event.getY();
            }
            return true;
        }
        return super.onTouchEvent(event);
    }

    static class ModelIndex {
        public String modelname;
        public String md2filename;
        public String texfilename;
        public String vshfilename;
        public String fshfilename;
        public float rotx;
        public float roty;
        public float rotz;
        public float posx;
        public float posy;
        public float posz;
        public float sclx;
        public float scly;
        public float sclz;
    }
}
