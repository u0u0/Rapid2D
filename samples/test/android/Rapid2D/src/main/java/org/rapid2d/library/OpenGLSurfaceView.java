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
import android.view.MotionEvent;
import android.view.KeyEvent;

/**
 * Created by u0u0 on 5/4/16.
 */
public class OpenGLSurfaceView extends GLSurfaceView {
    private Rapid2DActivity activity;

    public OpenGLSurfaceView(Rapid2DActivity activity) {
        super(activity);

        this.activity = activity;
        this.setEGLContextClientVersion(2);//OpenGL 2.0
        this.setFocusableInTouchMode(true);
    }

    // http://my.oschina.net/banxi/blog/56421
    @Override
    public boolean onTouchEvent(final MotionEvent pMotionEvent) {
        final int pointerNumber = pMotionEvent.getPointerCount();
        final int[] ids = new int[pointerNumber];
        final float[] xs = new float[pointerNumber];
        final float[] ys = new float[pointerNumber];
        final int pointIndex = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;

        for (int i = 0; i < pointerNumber; i++) {
            ids[i] = pMotionEvent.getPointerId(i);
            xs[i] = pMotionEvent.getX(i);
            // view space convert to OpenGL space
            ys[i] = activity.screenHeight - pMotionEvent.getY(i);
        }

        // must & mask to get ACTION_POINTER_DOWN info
        switch (pMotionEvent.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_POINTER_DOWN:
                ids[0] = ids[pointIndex];
                xs[0] = xs[pointIndex];
                ys[0] = ys[pointIndex];

                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        activity.sendTouchs("began", 1, ids, xs, ys);
                    }
                });
                break;

            case MotionEvent.ACTION_DOWN:
                // there is only one finger on the screen
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        activity.sendTouchs("began", 1, ids, xs, ys);
                    }
                });
                break;

            case MotionEvent.ACTION_MOVE:
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        activity.sendTouchs("moved", pointerNumber, ids, xs, ys);
                    }
                });
                break;

            case MotionEvent.ACTION_POINTER_UP:
                ids[0] = ids[pointIndex];
                xs[0] = xs[pointIndex];
                ys[0] = ys[pointIndex];

                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        activity.sendTouchs("ended", 1, ids, xs, ys);
                    }
                });
                break;

            case MotionEvent.ACTION_UP:
                // there is only one finger on the screen
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        activity.sendTouchs("ended", 1, ids, xs, ys);
                    }
                });
                break;

            case MotionEvent.ACTION_CANCEL:
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        activity.sendTouchs("ended", pointerNumber, ids, xs, ys);
                    }
                });
                break;
        }

        return true;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_BACK:
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        // GLFW_KEY_BACKSPACE
                        activity.sendKeyEvent("down", 259);
                    }
                });
                return true;
            default:
                return super.onKeyDown(keyCode, event);
        }
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_BACK:
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        // GLFW_KEY_BACKSPACE
                        activity.sendKeyEvent("up", 259);
                    }
                });
                return true;
            default:
                return super.onKeyUp(keyCode, event);
        }
    }
}
