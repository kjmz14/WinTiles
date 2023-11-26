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

#include "command.hpp"
#include "../res/res.h"
#include "strings.hpp"
#include "main.hpp"
#include "windowproc.hpp"
#include "dlgproc.hpp"
#include "key.hpp"

void OnCommand(HWND hWnd, int id, HWND, UINT)
{

	const auto newGameAsk = [hWnd]() -> bool
	{

	  return !tilesInstance.MovementMade || !tilesInstance.MovementPossible() ||
		  MessageBoxW(hWnd,
			  WTStrings::newGame, WTStrings::confirm,
			  MB_ICONQUESTION | MB_YESNO) == IDYES;

	};

	switch (id)
	{

	case IDM_NEW:
		if (!newGameAsk())
			return;

		tilesInstance.Start();
		break;

	case IDM_UNDO:
		tilesInstance.GoBack();
		break;

	case IDM_EXIT:
		SendMessageW(hWnd, WM_CLOSE, 0, 0);
		return;

	case IDM_HIGHSCORES:
		DialogBoxW(hGameInstance, MAKEINTRESOURCEW(IDD_HIGHSCORES), hWnd, ScoresProc);
		return;

	case IDM_ABOUT:
		DialogBoxW(hGameInstance, MAKEINTRESOURCEW(IDD_ABOUT), hWnd, AboutProc);
		return;

	case IDM_MANUAL:
		DialogBoxW(hGameInstance, MAKEINTRESOURCEW(IDD_MANUAL), hWnd, AboutProc);
		return;

	case IDM_WASD:
		bWasd = !bWasd;
		UpdateMenu();
		return;

	case IDM_SMALL:
		tileSize = TILE_SIZE_SMALL;
		goto sizeSelectionUpdate;

	case IDM_NORMAL:
		tileSize = TILE_SIZE_NORMAL;
		goto sizeSelectionUpdate;

	case IDM_LARGE:
		tileSize = TILE_SIZE_LARGE;
	sizeSelectionUpdate:
		UpdateMenu();
		UpdateSize(hWnd);
		return;

	case IDM_1024:
	case IDM_2048:
	case IDM_4096:
	case IDM_8192:
		if (!newGameAsk())
			return;

		tilesInstance.Limit = 1024 << (id - IDM_1024);
		UpdateMenu();
		UpdateTitle(hWnd);
		tilesInstance.Start();
		break;

	default:
		return;

	}

	InvalidateRect(hWnd, nullptr, FALSE);
	UpdateWindow(hWnd);

}
