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

#include "windowproc.hpp"
#include "../res/res.h"
#include "strings.hpp"
#include "main.hpp"
#include "paint.hpp"
#include "command.hpp"
#include "key.hpp"
#include "save.hpp"
#include "extra.hpp"
#include <windowsx.h>
#include <string>

HFONT hFont;

int tileSize = TILE_SIZE_NORMAL;

void UpdateTitle(HWND hWnd)
{

	std::wstring title = WTStrings::appName;
	title += L" - ";
	title += std::to_wstring(tilesInstance.Limit);
	SetWindowTextW(hWnd, title.c_str());

}

#define BOOL_TO_MF_CHECKED(b) ((b) ? MF_CHECKED : MF_UNCHECKED)

void UpdateMenu()
{

	for (int i = IDM_1024; i <= IDM_8192; i++)
		CheckMenuItem(hGameMenu, i, MF_UNCHECKED);

	for (int i = IDM_SMALL; i <= IDM_LARGE; i++)
		CheckMenuItem(hGameMenu, i, MF_UNCHECKED);

	CheckMenuItem(hGameMenu, IDM_WASD, BOOL_TO_MF_CHECKED(bWasd));
	CheckMenuItem(hGameMenu, IDM_1024 + lShiftOf1024(tilesInstance.Limit), MF_CHECKED);
	CheckMenuItem(hGameMenu, IDM_SMALL + (tileSize - TILE_SIZE_SMALL) / TILE_SIZE_STEP, MF_CHECKED);

}

void UpdateSize(HWND hWnd)
{

	DeleteObject(hFont);

	hFont = CreateFontW(
		(int)((float)tileSize / 2.5f), 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Tahoma"
	);

	HDC hDC = CreateCompatibleDC(nullptr);
	TEXTMETRICW tm = {};

	auto hOldFont = (HFONT)SelectObject(hDC, hFont);
	GetTextMetricsW(hDC, &tm);

	SelectObject(hDC, hOldFont);
	DeleteDC(hDC);

	RECT adjustedRect = {};
	AdjustWindowRectEx(&adjustedRect, WINDOW_STYLE, TRUE, WINDOW_EXSTYLE);

	//Window width and height includes scrollbars...even when they're not used
	const int additionalWidth =
		adjustedRect.right - adjustedRect.left + GetSystemMetrics(SM_CXVSCROLL);

	const int additionalHeight =
		adjustedRect.bottom - adjustedRect.top +
			GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYHSCROLL);

	const int
		width = 2 * PADDING + 4 * tileSize + 1,
		height = width + PADDING + tm.tmHeight;

	SetWindowPos(
		hWnd, nullptr, 0, 0,
		width + additionalWidth,
		height + additionalHeight,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS
	);

}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_CREATE:
		if (!Save::Read())
		{
			MessageBoxW(hWnd, WTStrings::loadFail, WTStrings::error, MB_ICONWARNING);
			tilesInstance.Start();
		}

		UpdateSize(hWnd);
		UpdateMenu();
		UpdateTitle(hWnd);

		break;

	case WM_PAINT:
		HANDLE_WM_PAINT(hWnd, wParam, lParam, OnPaint);
		break;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_COMMAND:
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, OnCommand);
		break;

	case WM_KEYDOWN:
		HANDLE_WM_KEYDOWN(hWnd, wParam, lParam, OnKeyDown);
		break;

	case WM_KEYUP:
	case WM_SETFOCUS:
		bKeyLock = false;
		break;

	case WM_DESTROY:
	writeAgain:
		if (!Save::Save())
		{
			int ret =
				MessageBoxW(hWnd, WTStrings::saveFail, WTStrings::error, MB_ICONERROR | MB_RETRYCANCEL);

			if (ret == IDRETRY)
				goto writeAgain;
		}

		DeleteObject(hBackground);
		DeleteObject(hBoard);
		DeleteObject(hFont);

		for (const auto& pair : tileColor)
			DeleteObject(pair.second.brush);

		tileColor.clear();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);

	}

	return 0;

}
