package com.tencent.willisdai.particlesystem;

/**
 * Created by willisdai on 2018/3/2.
 */

public class ParticleSystemProxy {
    static {
        System.loadLibrary("native-lib");
    }

    public static void load() {
        loadNative();
    }

    public static void render() {
        renderNative();
    }

    public static void init() {
        initNative();
    }

    private static native void initNative();
    private static native void loadNative();
    private static native void renderNative();
}
