// Copyright 2016 KeNan Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package org.rapid2d.library;

import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.view.Display;
import java.io.File;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by u0u0 on 2/26/16.
 */

class OpenGLRender implements GLSurfaceView.Renderer {
    private Rapid2DActivity activity;

    public OpenGLRender(Rapid2DActivity activity){
        this.activity = activity;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // get Screen Size
        int width;
        int height;
        Display display = this.activity.getWindowManager().getDefaultDisplay();
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.HONEYCOMB) {
            Point size = new Point();
            display.getSize(size);
            width = size.x;
            height = size.y;
        } else {
            width = display.getWidth();
            height = display.getHeight();
        }

        // 1. set Writable path to ndk.
        File filesDir = this.activity.getApplicationContext().getFilesDir();
        this.activity.setFilesDir(filesDir.toString());
        // 2. set assertManager to ndk.
        this.activity.setAssetManager(this.activity.getResources().getAssets());
        // 3. init engine
        this.activity.init(width, height);
        this.activity.screenWidth = width;
        this.activity.screenHeight = height;
    }

    @Override
    public void onDrawFrame(GL10 gl){
        this.activity.mainLoop();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height){
        //android.util.Log.v("Rapid2D", width + "x" + height);
    }
}
