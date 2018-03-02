package com.tencent.willisdai.particlesystem;

import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.support.v4.media.session.ParcelableVolumeInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity implements GLSurfaceView.Renderer {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private GLSurfaceView glview;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        glview = findViewById(R.id.glview);
        glview.setEGLContextClientVersion(3);
        glview.setRenderer(this);

        ParticleSystemProxy.init();
//        loadBitmap();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        stringFromJNI();
        ParticleSystemProxy.load();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glview.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
//        GLES30.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
//        glview.requestRender();
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);
        ParticleSystemProxy.render();
    }
}
