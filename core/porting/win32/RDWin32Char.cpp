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

#include "RDWin32Char.h"

WCHAR *UTF8_To_Wchar(const char *in)
{
	int strLength = MultiByteToWideChar(CP_UTF8, 0, in, -1, nullptr, 0);
	if (strLength == 0) {
		return NULL;
	}

	WCHAR *pOut = (WCHAR *)malloc((strLength + 1) * sizeof(WCHAR));
	memset(pOut, 0, (strLength + 1) * sizeof(WCHAR));

	MultiByteToWideChar(CP_UTF8, 0, in, -1, pOut, strLength);
	return pOut;
}

char *Wchar_To_Utf8(WCHAR *in)
{
	int strLength = WideCharToMultiByte(CP_UTF8, 0, in, -1, nullptr, 0, nullptr, FALSE);
	if (strLength == 0) {
		return NULL;
	}

	char *pOut = (char*)malloc(strLength + 1);
	memset(pOut, 0, strLength + 1);

	WideCharToMultiByte(CP_UTF8, 0, in, -1, pOut, strLength, nullptr, FALSE);
	return pOut;
}
