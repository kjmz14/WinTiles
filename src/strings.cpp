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

#include "strings.hpp"
#include "../res/res.h"
#include "main.hpp"

#define LOAD_STR(a, b) LoadStringW(hGameInstance, a, b, sizeof(STRING)/sizeof(WCHAR))

namespace WTStrings
{

	STRING appName, confirm, newGame,
		gameOver, limit, over,
		emptyScore, highScore, error,
		loadFail, saveFail;

}

void WTStrings::load()
{

	LOAD_STR(IDS_APPNAME, appName);
	LOAD_STR(IDS_CONFIRM, confirm);
	LOAD_STR(IDS_NEW, newGame);
	LOAD_STR(IDS_GAMEOVER, gameOver);
	LOAD_STR(IDS_LIMIT, limit);
	LOAD_STR(IDS_OVER, over);
	LOAD_STR(IDS_HIGHSCORETEMPLATE, highScore);
	LOAD_STR(IDS_EMPTYSCORE, emptyScore);
	LOAD_STR(IDS_ERROR, error);
	LOAD_STR(IDS_LOADFAIL, loadFail);
	LOAD_STR(IDS_SAVEFAIL, saveFail);

}
