// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <https://unlicense.org>

#include "main.hpp"
#include "../res/res.h"
#include "strings.hpp"
#include "windowproc.hpp"

HINSTANCE hGameInstance;
HMENU hGameMenu;

Tiles tilesInstance;
HighScores highScores;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{

	auto hMutex = CreateMutexW(nullptr, FALSE, L"!WinTiles!");

	if (GetLastError() != 0)
		return 0;

	hGameInstance = hInstance;

	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"WinTiles Window Class";
	wc.lpfnWndProc = WindowProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON_GROUP));
	RegisterClassExW(&wc);

	WTStrings::load();

	hGameMenu = LoadMenuW(hInstance, MAKEINTRESOURCEW(IDM_WINTILES));

	HWND hGameWnd = CreateWindowExW(
		WINDOW_EXSTYLE,
		wc.lpszClassName,
		WTStrings::appName,
		WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
		nullptr, hGameMenu, wc.hInstance, nullptr
	);

	if (hGameWnd == nullptr)
	{
		DestroyMenu(hGameMenu);
		DestroyCursor(wc.hCursor);
		DestroyIcon(wc.hIcon);
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ReleaseMutex(hMutex);
		return 1;
	}

	ShowWindow(hGameWnd, SW_SHOW);

	HACCEL hAccel = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDR_ACCELERATORS));
	MSG msg = {};

	while (GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		if (TranslateAcceleratorW(hGameWnd, hAccel, &msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	DestroyMenu(hGameMenu);
	DestroyAcceleratorTable(hAccel);
	DestroyCursor(wc.hCursor);
	DestroyIcon(wc.hIcon);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
	ReleaseMutex(hMutex);

	return (int)msg.wParam;

}
