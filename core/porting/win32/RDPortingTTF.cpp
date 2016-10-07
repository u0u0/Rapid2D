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

#include <windows.h>
#include <stdio.h>
#include "RDPortingTTF.h"
#include "RDWin32Char.h"

bool RDPortingTTF_getBitmap32(const char *utf8_text,
	const char *fontName,
	float fontSize,
	RDColor *fontColor,// 0~255
	int align,// 0 left, 1 center, 2 right
	RD_Bitmap32 *outMap)
{
	// create font
	HFONT m_hFont;
	HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONTA logFont = { 0 };
	GetObjectA(hDefFont, sizeof(logFont), &logFont);
	logFont.lfCharSet = DEFAULT_CHARSET;
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, fontName);
	logFont.lfHeight = -fontSize;
	m_hFont = CreateFontIndirectA(&logFont);
	if (NULL == m_hFont) {
		printf("Error: RDPortingTTF_getBitmap32 create font fail!\n");
		return false;
	}

	// convert text to utf16
	WCHAR *wText = UTF8_To_Wchar(utf8_text);

	// get hdc
	HDC hdc = GetDC(NULL);
	HDC m_hDC = CreateCompatibleDC(hdc);
	ReleaseDC(NULL, hdc);

	// use current font to measure text extent
	RECT rc = { 0, 0, 0, 0 };
	DWORD dwCalcFmt = DT_CALCRECT;
	SelectObject(m_hDC, m_hFont);
	DrawTextW(m_hDC, wText, -1, &rc, dwCalcFmt);

	// create bitmap
	HBITMAP m_hBmp = CreateBitmap(rc.right, rc.bottom, 1, 32, NULL);
	SelectObject(m_hDC, m_hBmp);

	SetBkMode(m_hDC, TRANSPARENT);
	SetTextColor(m_hDC, RGB(255, 255, 255)); //white color to make post color fill happy

	// draw text
	dwCalcFmt = DT_WORDBREAK;
	switch (align)
	{
	case 0: // left
		dwCalcFmt |= DT_LEFT;
		break;
	case 1: // center
		dwCalcFmt |= DT_CENTER;
		break;
	case 2: // right
		dwCalcFmt |= DT_RIGHT;
		break;
	}
	DrawTextW(m_hDC, wText, -1, &rc, dwCalcFmt);

	struct
	{
		BITMAPINFOHEADER bmiHeader;
		int mask[4];
	} bi = { 0 };
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	GetDIBits(m_hDC, m_hBmp, 0, rc.bottom, NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);

	size_t bufferSize = rc.right * rc.bottom * 4;
	if (bufferSize != bi.bmiHeader.biSizeImage)
	{
		printf("Error: RDPortingTTF_getBitmap32 create font fail!\n");
	}
	unsigned char *m_pData = (unsigned char *)malloc(bufferSize);
	memset(m_pData, 0, bufferSize);

	GetDIBits(m_hDC, m_hBmp, 0, rc.bottom, m_pData, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
	// fill fontColor
	COLORREF textColor = (fontColor->b << 16 | fontColor->g << 8 | fontColor->r) & 0x00ffffff;
	COLORREF *pPixel = NULL;
	unsigned char alpha = 0;
	for (int y = 0; y < rc.bottom; ++y)
	{
		pPixel = (COLORREF *)m_pData + y * rc.right;
		for (int x = 0; x < rc.right; ++x)
		{
			COLORREF &clr = *pPixel;
			// GetRValue(clr) is original draw text alpha
			alpha = GetRValue(clr) * fontColor->a / 255.0;
			clr = (alpha << 24) | textColor;
			++pPixel;
		}
	}

	outMap->buffer = m_pData;
	outMap->width = rc.right;
	outMap->height = rc.bottom;
	outMap->isPremultipliedAlpha = false;//must set false to rendering

	free(wText);
	DeleteObject(m_hBmp);
	DeleteObject(m_hFont);
	DeleteDC(m_hDC);
	return true;
}
