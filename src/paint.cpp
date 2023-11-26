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

#include "paint.hpp"
#include "main.hpp"
#include "windowproc.hpp"
#include <string>

#define BG_COLOR (COLORREF)0x001F5C7A

HBRUSH
	hBackground = CreateSolidBrush(BG_COLOR),
	hBoard = CreateSolidBrush(0x00696969);

const auto hTransparentPen = (HPEN)GetStockObject(NULL_PEN);

std::map<int, TILE_STRUCT> tileColor =
	{
		{ 2, { 0x00DAE4EE, CreateSolidBrush(0x00DAE4EE) }},
		{ 4, { 0x00C8E0ED, CreateSolidBrush(0x00C8E0ED) }},
		{ 8, { 0x0079B1F2, CreateSolidBrush(0x0079B1F2) }},
		{ 16, { 0x006395F5, CreateSolidBrush(0x006395F5) }},
		{ 32, { 0x005F7CF6, CreateSolidBrush(0x005F7CF6) }},
		{ 64, { 0x003B5EF6, CreateSolidBrush(0x003B5EF6) }},
		{ 128, { 0x0072CFED, CreateSolidBrush(0x0072CFED) }},
		{ 256, { 0x0061CCED, CreateSolidBrush(0x0061CCED) }},
		{ 512, { 0x0050C8ED, CreateSolidBrush(0x0050C8ED) }},
		{ 1024, { 0x003FC5ED, CreateSolidBrush(0x003FC5ED) }},
		{ 2048, { 0x002EC2ED, CreateSolidBrush(0x002EC2ED) }},
		{ 4096, { 0x00FF00FF, CreateSolidBrush(0x00FF00FF) }},
		{ 8192, { 0x009000FF, CreateSolidBrush(0x009000FF) }}
	};

void OnPaint(HWND hWnd)
{

	PAINTSTRUCT ps = {};

	HDC hDC = BeginPaint(hWnd, &ps);
	HDC memDC = CreateCompatibleDC(hDC);

	RECT clientRect = {};
	GetClientRect(hWnd, &clientRect);

	HBITMAP memBitmap = CreateCompatibleBitmap(
		hDC,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top
	);

	auto hOldBitmap = SelectObject(memDC, memBitmap);

	FillRect(memDC, &ps.rcPaint, hBackground);

	auto hOldBrush = (HBRUSH)SelectObject(memDC, hBoard);

	Rectangle(memDC, PADDING, PADDING, PADDING + 4 * tileSize + 1, PADDING + 4 * tileSize + 1);

	for (int i = PADDING + tileSize; i < tileSize * 4; i += tileSize)
	{
		MoveToEx(memDC, i, PADDING, nullptr);
		LineTo(memDC, i, PADDING + tileSize * 4);
		MoveToEx(memDC, PADDING, i, nullptr);
		LineTo(memDC, PADDING + tileSize * 4, i);
	}

	auto hOldFont = (HFONT)SelectObject(memDC, hFont);

	TEXTMETRICW tm = {};
	GetTextMetricsW(memDC, &tm);

	SetTextColor(memDC, 0x00FFFFFF);
	SetBkColor(memDC, BG_COLOR);

	const auto score = std::to_wstring(tilesInstance.Score);

	TextOutW(memDC,
		PADDING + 2 * tileSize - tm.tmAveCharWidth * (int)score.length() / 2,
		2 * PADDING + 4 * tileSize,
		score.c_str(),
		(int)score.length());

	auto hOldPen = (HPEN)SelectObject(memDC, hTransparentPen);

	SetTextColor(memDC, 0);

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			const auto value = tilesInstance.Board[y][x];

			if (value == 0)
				continue;

			SelectObject(memDC, tileColor[value].brush);
			SetBkColor(memDC, tileColor[value].color);

			const int left = PADDING + tileSize * x + 1,
				top = PADDING + tileSize * y + 1;

			Rectangle(memDC, left, top, left + tileSize, top + tileSize);

			auto val = std::to_wstring(value);

			const int iValLength = (int)val.length(),
				textWidthCenter = tm.tmAveCharWidth * iValLength / 2,
				textHeightCenter = tm.tmHeight / 2;

			TextOutW(memDC,
				left + tileSize / 2 - textWidthCenter - iValLength,
				top + tileSize / 2 - textHeightCenter,
				val.c_str(),
				iValLength);
		}

	BitBlt(
		hDC,
		0, 0,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,
		memDC, 0, 0, SRCCOPY
	);

	SelectObject(memDC, hOldBrush);
	SelectObject(memDC, hOldPen);
	SelectObject(memDC, hOldFont);
	SelectObject(memDC, hOldBitmap);
	DeleteObject(memBitmap);
	DeleteDC(memDC);

	EndPaint(hWnd, &ps);

}
