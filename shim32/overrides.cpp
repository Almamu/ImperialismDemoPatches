#include "framework.h"

extern "C"
{
	int WINAPI I_LoadStringA(
		HINSTANCE hInstance,
		UINT      uID,
		LPSTR     lpBuffer,
		int       cchBufferMax
	) {
		int copiedBytes = LoadStringA(hInstance, uID, lpBuffer, cchBufferMax);

		if (uID == 1006001) {
			DebugBreak();
		}

		printf("Loading string (%d) : % s\n", uID, lpBuffer);

		return copiedBytes;
	}
}