package com.tencent.willisdai.particlesystem;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;

/**
 * Created by willisdai on 2018/3/2.
 */

public class AndroidUtils {
    private static final String TAG = "AndroidUtils";

    public static Bitmap decodeBitmapFromFile(String filename) {
        if(!fileExists(filename)) {
            return null;

        } else {
            BitmapFactory.Options options = new BitmapFactory.Options();
//            options.inJustDecodeBounds = true;
//            BitmapFactory.decodeFile(filename, options);
//            options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);
//            options.inJustDecodeBounds = false;
            options.inPreferQualityOverSpeed = true;
            Bitmap bitmap = null;

            try {
                bitmap = BitmapFactory.decodeFile(filename, options);

            } catch (OutOfMemoryError var9) {
                options.inSampleSize <<= 1;

                try {
                    bitmap = BitmapFactory.decodeFile(filename, options);

                } catch (OutOfMemoryError var8) {
                    Log.e(TAG, var8.toString());
                }
            }

            return bitmap;
        }
    }

    public static boolean fileExists(String path) {
        return !TextUtils.isEmpty(path) && new File(path).exists();
    }
}
