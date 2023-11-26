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

#pragma once

#ifndef WINTILES_HIGHSCORES_HPP
#define WINTILES_HIGHSCORES_HPP

#include "../tiles.hpp"

#define NAME_LENGTH 15
#define SCORES_COUNT 5

#define NAME_SIZE ((NAME_LENGTH + 1) * 2)
#define SCORE_SIZE (NAME_SIZE + 4)
#define HIGHSCORES_DATA_SIZE (SCORE_SIZE * SCORES_COUNT)

typedef struct
{
	wchar_t name[NAME_LENGTH + 1];
	SCORE_TYPE score;
} ScoreEntry;

class HighScores
{

 public:
	ScoreEntry Scores[SCORES_COUNT]{};

	bool Broken(SCORE_TYPE score) const;

	void Add(const std::wstring& name, SCORE_TYPE score);

	void GetBytes(unsigned char*) const;

	void FromBytes(const unsigned char*);

};

#endif //WINTILES_HIGHSCORES_HPP
