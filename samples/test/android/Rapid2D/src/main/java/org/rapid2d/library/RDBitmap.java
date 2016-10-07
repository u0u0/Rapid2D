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

import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.Paint.Align;
import android.graphics.Paint.FontMetrics;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by u0u0 on 3/30/16.
 */
public class RDBitmap {
    public static native void initNativeBMP(int width,
                                      int height,
                                      byte[] pixels);
    public static void createTextBitmap(String text,
                                        String fontName,
                                        float fontSize,
                                        int colorR,
                                        int colorG,
                                        int colorB,
                                        int colorA,
                                        int align)
    {
        // get Paint and set param
        final Paint paint = new Paint();
        paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
        paint.setARGB(colorA, colorR, colorG, colorB);
        paint.setTextSize(fontSize);
        paint.setAntiAlias(true);

        switch (align) {
            case 0://left
                paint.setTextAlign(Align.LEFT);
                break;
            case 1://center
                paint.setTextAlign(Align.CENTER);
                break;
            case 2://right
                paint.setTextAlign(Align.RIGHT);
                break;
            default:
                android.util.Log.v("Rapid2D_LOG", "Error in java, createTextBitmap wrong align");
                return;
        }

        // get font height
        FontMetrics fontMetrics = paint.getFontMetrics();
        float fontHeight = fontMetrics.bottom - fontMetrics.top;

        // Android not deal '\n', do it by myself
        final String lines[] = text.split("\n");

        // get text frame size
        float textWidth = 0;
        float textHeight = 0;
        for (final String line : lines) {
            float lineWidth = paint.measureText(line);
            textWidth = lineWidth > textWidth ? lineWidth : textWidth;
            textHeight += fontHeight;
        }

        // draw text
        Bitmap bitmap = Bitmap.createBitmap((int)Math.ceil(textWidth),
                (int)Math.ceil(textHeight),
                Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);

        float drawX = 0.0f;
        switch (align) {
            case 0:
                drawX = 0;
                break;
            case 1:
                drawX = bitmap.getWidth() / 2.0f;
                break;
            case 2:
                drawX = bitmap.getWidth();
                break;
            default:
                drawX = 0;
                break;
        }
        float drawY = -fontMetrics.top;
        // draw lines
        for (final String line : lines) {
            canvas.drawText(line, drawX, drawY, paint);
            drawY += fontHeight;
        }

        final byte[] pixels = new byte[bitmap.getWidth() * bitmap.getHeight() * 4];
        final ByteBuffer buf = ByteBuffer.wrap(pixels);
        buf.order(ByteOrder.nativeOrder());
        bitmap.copyPixelsToBuffer(buf);
        initNativeBMP(bitmap.getWidth(), bitmap.getHeight(), pixels);
    }
}