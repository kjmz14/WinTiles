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

#include "highscores.hpp"
#include <algorithm>
#include <cstring>

bool HighScores::Broken(SCORE_TYPE score) const
{

	for (int i = SCORES_COUNT - 1; i >= 0; i--)
		if (score > Scores[i].score)
			return true;

	return false;

}

void HighScores::Add(const std::wstring& name, SCORE_TYPE score)
{

	for (int i = SCORES_COUNT - 1; i >= 0; i--)
		if (score > Scores[i].score)
		{
			for (int j = 1; j <= i; j++)
				Scores[j - 1] = Scores[j];

			Scores[i].score = score;

			const int length = std::min((int)name.length(), NAME_LENGTH + 1);
			wcsncpy_s(Scores[i].name, name.c_str(), length);

			break;
		}

}

void HighScores::GetBytes(unsigned char* data) const
{

	for (int i = 0; i < SCORES_COUNT; i++)
	{

		const int padding = i * SCORE_SIZE;

		for (int j = 0; j < NAME_SIZE; j += 2)
		{
			const auto widechar = Scores[i].name[j / 2];
			data[padding + j] = widechar & 0xFF;
			data[padding + j + 1] = (widechar >> 8) & 0xFF;
		}

		for (int j = 0; j < 4; j++)
			data[padding + NAME_SIZE + j] = (Scores[i].score >> (8 * j)) & 0xFF;

	}

}

void HighScores::FromBytes(const unsigned char* data)
{

	for (int i = 0; i < SCORES_COUNT; i++)
	{

		const int padding = i * SCORE_SIZE;

		for (int j = 0; j < NAME_SIZE; j += 2)
			Scores[i].name[j / 2] = data[padding + j] | (data[padding + j + 1] << 8);

		for (int j = 0; j < 4; j++)
			Scores[i].score |= (SCORE_TYPE)(data[padding + NAME_SIZE + j]) << (8 * j);

	}

}
