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

#include "key.hpp"
#include "../res/res.h"
#include "strings.hpp"
#include "dlgproc.hpp"
#include "main.hpp"
#include "tiles/tiles.hpp"
#include <windows.h>

bool bKeyLock = false,
	bWasd = false;

void OnKeyDown(HWND hWnd, UINT vk, BOOL, int, UINT)
{

	if (vk != VK_PAUSE && bKeyLock)
		return;

	Tiles::STATUS status = Tiles::update;

	switch (vk)
	{

	case 'A':
		if (!bWasd) return;
	case VK_LEFT:
		status = tilesInstance.MoveLeft();
		break;

	case 'D':
		if (!bWasd) return;
	case VK_RIGHT:
		status = tilesInstance.MoveRight();
		break;

	case 'W':
		if (!bWasd) return;
	case VK_UP:
		status = tilesInstance.MoveUp();
		break;

	case 'S':
		if (!bWasd) return;
	case VK_DOWN:
		status = tilesInstance.MoveDown();
		break;

	case 'B':
		if (!bWasd) return;
	case VK_DELETE:
		tilesInstance.GoBack();
		break;

	default:
		return;

	}

	bKeyLock = true;

	if (status == Tiles::kept)
		return;

	InvalidateRect(hWnd, nullptr, FALSE);
	UpdateWindow(hWnd);

	if (status == Tiles::update)
	{
		if (!tilesInstance.MovementPossible() && vk != VK_DELETE && vk != 'B')
		{
			bKeyLock = false;
			MessageBoxW(hWnd, WTStrings::over, WTStrings::gameOver, MB_ICONINFORMATION);
		}

		return;
	}

	MessageBoxW(hWnd, WTStrings::limit, WTStrings::gameOver, MB_ICONINFORMATION);

	if (!highScores.Broken(tilesInstance.Score))
		return;

	const auto ret =
		DialogBoxParamW(hGameInstance, MAKEINTRESOURCEW(IDD_NAME), hWnd, NameProc, (LPARAM)tilesInstance.Score);

	if (ret == IDOK)
		DialogBoxW(hGameInstance, MAKEINTRESOURCEW(IDD_HIGHSCORES), hWnd, ScoresProc);

}
