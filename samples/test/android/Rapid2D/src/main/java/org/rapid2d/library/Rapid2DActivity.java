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

import android.opengl.GLSurfaceView;
import android.os.Bundle;


public class Rapid2DActivity extends android.app.Activity {
    public int screenWidth;
    public int screenHeight;

    // Jni import and method define
    static {
        System.loadLibrary("Rapid2D");
    }
    public native String stringFromJNI();
    public native void init(int width, int height);
    public native void mainLoop();
    public native void setFilesDir(String dir);
    public native void setAssetManager(android.content.res.AssetManager ass);
    public native void sendTouchs(String type, int count, int ids[], float xs[], float ys[]);
    public native void sendKeyEvent(String type, int code);
    // end of jni

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // init OpenGL ES 2.0
        GLSurfaceView glSurfaceView = new OpenGLSurfaceView(this);
        glSurfaceView.setRenderer(new OpenGLRender(this));
        setContentView(glSurfaceView);

        // get jni build type
        android.util.Log.v("Rapid2D_LOG", stringFromJNI());
    }

    @Override
    protected void onResume() {
        super.onResume();

        // TODO engine resume
    }

    @Override
    protected void onPause() {
        super.onPause();

        // TODO engine pause
    }
}
