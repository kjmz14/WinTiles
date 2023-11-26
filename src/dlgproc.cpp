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

#include "dlgproc.hpp"
#include "../res/res.h"
#include "strings.hpp"
#include "main.hpp"
#include <cwchar>

INT_PTR CALLBACK AboutProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM)
{

	switch (uMsg)
	{

	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
			EndDialog(hWnd, LOWORD(wParam));
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd, IDOK);
		break;

	default:
		return FALSE;

	}

	return TRUE;

}

#define SCORE_BUFFER 33

INT_PTR CALLBACK ScoresProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM)
{

	switch (uMsg)
	{

	case WM_INITDIALOG:
	{
		WCHAR wstr[SCORE_BUFFER]{};

		const auto convertScore = [&wstr](int number, const ScoreEntry& score) -> void
		{
		  swprintf(wstr, SCORE_BUFFER, WTStrings::highScore, number, score.score, score.name);
		};

		for (int i = 0; i < SCORES_COUNT; i++)
		{
			static const auto scores = highScores.Scores;

			if (scores[i].score == 0)
			{
				swprintf(wstr, SCORE_BUFFER, WTStrings::emptyScore, SCORES_COUNT - i);
				SetDlgItemTextW(hWnd, IDC_HIGH5 - i, wstr);
				continue;
			}

			convertScore(SCORES_COUNT - i, scores[i]);
			SetDlgItemTextW(hWnd, IDC_HIGH5 - i, wstr);
		}
	}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
			EndDialog(hWnd, LOWORD(wParam));
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd, IDOK);
		break;

	default:
		return FALSE;

	}

	return TRUE;

}

// lParam is Score
INT_PTR CALLBACK NameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static SCORE_TYPE score;

	switch (uMsg)
	{

	case WM_INITDIALOG:
		SendDlgItemMessageW(hWnd, IDC_NAME, EM_SETLIMITTEXT, NAME_LENGTH, 0);
		score = (SCORE_TYPE)lParam;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDOK:
		{
			WCHAR name[NAME_LENGTH + 1]{};
			GetDlgItemTextW(hWnd, IDC_NAME, name, NAME_LENGTH + 1);
			highScores.Add(name, score);
		}
		case IDCANCEL:
			EndDialog(hWnd, LOWORD(wParam));

		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd, IDOK);
		break;

	default:
		return FALSE;

	}

	return TRUE;

}
