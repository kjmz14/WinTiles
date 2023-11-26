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

#include "tiles.hpp"
#include <algorithm>
#include <cstring>

#if defined(__GNUC__)

#define NATIVE_POPCNT(x) __builtin_popcountll(x)

#elif defined(_M_X64) && defined(_MSC_VER)

#include <intrin.h>

#define NATIVE_POPCNT(x) __popcnt64(x)

#endif

inline static unsigned long long popcount(unsigned long long x)
{

#if defined(NATIVE_POPCNT) && !defined(NO_POPCNT)

	return NATIVE_POPCNT(x);

#else
	x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
	x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
	x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);

	return x;
#endif

}

void Tiles::addRandomTile()
{

	typedef struct
	{
		int x, y;
	} COORD;

	struct
	{
		COORD array[16];
		int count;
	} freeTiles{};

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (Board[y][x] == 0)
				freeTiles.array[freeTiles.count++] = { x, y };

	if (freeTiles.count == 0)
		return;

	std::uniform_int_distribution<int> indexDist(0, freeTiles.count - 1);
	const auto place = freeTiles.array[indexDist(mt)];

	Board[place.y][place.x] = valueDist(mt) * 2;

}

Tiles::Tiles()
{

	std::random_device rd;
	mt.seed(rd());
	valueDist = std::uniform_int_distribution<unsigned short>(1, 2);

}

void Tiles::Start()
{

	memset(&Board, 0, sizeof(Board));

	addRandomTile();
	addRandomTile();

	memcpy(previousBoard, Board, sizeof(Board));

	MovementMade = movementBlock = false;
	Score = previousScore = 0;

}

unsigned short Tiles::maxValue()
{

	unsigned short max = 2;

	for (const auto& x : Board)
		for (const auto& y : x)
		{
			max = std::max(max, y);

			if (max == Limit)
				return max;
		}

	return max;

}

void Tiles::GoBack()
{

	if (movementBlock)
		return;

	memcpy(Board, previousBoard, sizeof(Board));
	Score = previousScore;

}

bool Tiles::MovementPossible()
{

	if (movementBlock)
		return false;

	for (const auto& y : Board)
		for (unsigned short x : y)
			if (x == 0)
				return true;

	for (const auto& y : Board)
		for (int x = 0; x < 3; x++)
			if (y[x] == y[x + 1])
				return true;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 3; y++)
			if (Board[y][x] == Board[y + 1][x])
				return true;

	return false;

}

bool Tiles::afterMove()
{

	if (!MovementPossible())
		movementBlock = true;

	if (maxValue() >= Limit)
	{
		movementBlock = true;
		return true;
	}

	return false;

}

Tiles::STATUS Tiles::MoveLeft()
{

	if (movementBlock)
		return kept;

	const unsigned int prevScore = Score;
	unsigned short prevBoard[4][4];

	memcpy(prevBoard, Board, sizeof(Board));

	bool changesMade = false;

	for (auto& y : Board)
	{

		const auto moveZeroes = [&y, &changesMade](bool skipFirst) -> void
		{
		  bool movementDone;
		  do
		  {
			  movementDone = false;

			  for (int x = skipFirst; x < 3; x++)
				  if (y[x] == 0 && y[x + 1] != 0)
				  {
					  y[x] = y[x + 1];
					  y[x + 1] = 0;
					  changesMade = movementDone = true;
				  }

		  } while (movementDone);
		};

		moveZeroes(false);

		bool valuesChanged = false;

		for (int x = 0; x < 3; x++)
			if (y[x] != 0 && y[x] == y[x + 1])
			{
				Score += y[x] <<= 1;
				y[x + 1] = 0;
				changesMade = valuesChanged = true;
			}

		if (valuesChanged)
			moveZeroes(true);

	}

	if (changesMade)
	{
		MovementMade = true;
		memcpy(previousBoard, prevBoard, sizeof(previousBoard));
		previousScore = prevScore;
		addRandomTile();
	}

	if (afterMove())
		return limitReached;

	return update;

}

Tiles::STATUS Tiles::MoveRight()
{

	if (movementBlock)
		return kept;

	const unsigned int prevScore = Score;
	unsigned short prevBoard[4][4];

	memcpy(prevBoard, Board, sizeof(Board));

	bool changesMade = false;

	for (auto& y : Board)
	{

		const auto moveZeroes = [&y, &changesMade](bool skipLast) -> void
		{
		  bool movementDone;
		  do
		  {
			  movementDone = false;

			  for (int x = 3 - skipLast; x > 0; x--)
				  if (y[x] == 0 && y[x - 1] != 0)
				  {
					  y[x] = y[x - 1];
					  y[x - 1] = 0;
					  changesMade = movementDone = true;
				  }

		  } while (movementDone);
		};

		moveZeroes(false);

		bool valuesChanged = false;

		for (int x = 3; x > 0; x--)
			if (y[x] != 0 && y[x] == y[x - 1])
			{
				Score += y[x] <<= 1;
				y[x - 1] = 0;
				changesMade = valuesChanged = true;
			}

		if (valuesChanged)
			moveZeroes(true);

	}

	if (changesMade)
	{
		MovementMade = true;
		memcpy(previousBoard, prevBoard, sizeof(previousBoard));
		previousScore = prevScore;
		addRandomTile();
	}

	if (afterMove())
		return limitReached;

	return update;

}

Tiles::STATUS Tiles::MoveUp()
{

	if (movementBlock)
		return kept;

	const unsigned int prevScore = Score;
	unsigned short prevBoard[4][4];

	memcpy(prevBoard, Board, sizeof(Board));

	bool changesMade = false;

	for (int x = 0; x < 4; x++)
	{

		const auto moveZeroes = [x, &changesMade, this](bool skipFirst) -> void
		{
		  bool movementDone;
		  do
		  {
			  movementDone = false;

			  for (int y = skipFirst; y < 3; y++)
				  if (Board[y][x] == 0 && Board[y + 1][x] != 0)
				  {
					  Board[y][x] = Board[y + 1][x];
					  Board[y + 1][x] = 0;
					  changesMade = movementDone = true;
				  }

		  } while (movementDone);
		};

		moveZeroes(false);

		bool valuesChanged = false;

		for (int y = 0; y < 3; y++)
			if (Board[y][x] != 0 && Board[y][x] == Board[y + 1][x])
			{
				Score += Board[y][x] <<= 1;
				Board[y + 1][x] = 0;
				changesMade = valuesChanged = true;
			}

		if (valuesChanged)
			moveZeroes(true);

	}

	if (changesMade)
	{
		MovementMade = true;
		memcpy(previousBoard, prevBoard, sizeof(previousBoard));
		previousScore = prevScore;
		addRandomTile();
	}

	if (afterMove())
		return limitReached;

	return update;

}

Tiles::STATUS Tiles::MoveDown()
{

	if (movementBlock)
		return kept;

	const unsigned int prevScore = Score;
	unsigned short prevBoard[4][4];

	memcpy(prevBoard, Board, sizeof(Board));

	bool changesMade = false;

	for (int x = 0; x < 4; x++)
	{

		const auto moveZeroes = [x, &changesMade, this](bool skipLast) -> void
		{
		  bool movementDone;
		  do
		  {
			  movementDone = false;

			  for (int y = 3 - skipLast; y > 0; y--)
				  if (Board[y][x] == 0 && Board[y - 1][x] != 0)
				  {
					  Board[y][x] = Board[y - 1][x];
					  Board[y - 1][x] = 0;
					  changesMade = movementDone = true;
				  }

		  } while (movementDone);
		};

		moveZeroes(false);

		bool valuesChanged = false;

		for (int y = 3; y > 0; y--)
			if (Board[y][x] != 0 && Board[y][x] == Board[y - 1][x])
			{
				Score += Board[y][x] <<= 1;
				Board[y - 1][x] = 0;
				changesMade = valuesChanged = true;
			}

		if (valuesChanged)
			moveZeroes(true);

	}

	if (changesMade)
	{
		MovementMade = true;
		memcpy(previousBoard, prevBoard, sizeof(previousBoard));
		previousScore = prevScore;
		addRandomTile();
	}

	if (afterMove())
		return limitReached;

	return update;

}

void Tiles::GetBytes(unsigned char* data) const
{

	if (!movementBlock && MovementMade)
	{
		int byteCount = 0;

		for (const auto y : Board)
			for (int x = 0; x < 4; x++)
			{
				data[byteCount++] = y[x] & 0xFF;
				data[byteCount++] = (y[x] >> 8) & 0xFF;
			}

		if (Score == 0)
			return;

		for (int i = 0; i < 4; i++)
			data[byteCount++] = (Score >> 8 * i) & 0xFF;
	}
	else
	{
		memset(data, 0, BOARD_DATA_SIZE);
	}

}

void Tiles::FromBytes(const unsigned char* data)
{

	int byteCount = 0;
	bool nonZero = false;

	for (auto& y : Board)
		for (auto& x : y)
		{
			unsigned short val = 0;
			val |= data[byteCount++];
			val |= (unsigned short)data[byteCount++] << 8;

			if (popcount(val) > 1 || val == 1)
			{
				nonZero = false;
				break;
			}

			if (val != 0)
				nonZero = true;

			x = val;
		}

	if (!nonZero) //No board was saved
	{
		Start();
		return;
	}

	memcpy(previousBoard, Board, sizeof(Board));

	for (int i = 0; i < 4; i++)
		Score |= (unsigned long)data[byteCount++] << (8 * i);

	previousScore = Score;
	MovementMade = true;
	movementBlock = false;

}
