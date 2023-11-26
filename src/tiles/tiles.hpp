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

#ifndef WINTILES_TILES_HPP
#define WINTILES_TILES_HPP

#include <random>

typedef unsigned long SCORE_TYPE;

#define BOARD_DATA_SIZE ((4 * 4 * 2) + 4)

class Tiles
{

 public:
	typedef enum
	{
		limitReached = -1,
		kept,
		update,
	} STATUS;

	bool MovementMade = false;

	unsigned short Board[4][4]{{}},
		Limit = 2048;

	SCORE_TYPE Score = 0;

	Tiles();

	void Start();

	void GoBack();

	bool MovementPossible();

	STATUS MoveLeft();

	STATUS MoveRight();

	STATUS MoveUp();

	STATUS MoveDown();

	void GetBytes(unsigned char*) const;

	void FromBytes(const unsigned char*);

 protected:
	SCORE_TYPE previousScore = 0;

	bool movementBlock = false;

	std::mt19937_64 mt;
	std::uniform_int_distribution<unsigned short> valueDist;

	void addRandomTile();

	unsigned char previousBoard[sizeof(Board)]{{}};

	unsigned short maxValue();

	bool afterMove();

};

#endif //WINTILES_TILES_HPP
