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

#include "save.hpp"
#include "main.hpp"
#include "windowproc.hpp"
#include "key.hpp"
#include "extra.hpp"
#include <cstdio>
#include <cstring>

#define CONFIG_DATA_SIZE 1

namespace config
{

	static void GetBytes(unsigned char*);

	static void FromBytes(const unsigned char*);

}

void config::GetBytes(unsigned char* data)
{

	*data = 0;

	*data |= lShiftOf1024(tilesInstance.Limit);
	*data |= (unsigned char)bWasd << 3;
	*data |= (unsigned char)((tileSize - TILE_SIZE_SMALL) / TILE_SIZE_STEP) << 4;

}

void config::FromBytes(const unsigned char* data)
{

	struct
	{
		unsigned short Limit;
		int tileSize;
	} gotten{};

	gotten.Limit = 1024 << (*data & 0b11);
	bWasd = (*data >> 3) & 1;
	gotten.tileSize = TILE_SIZE_SMALL + TILE_SIZE_STEP * (*data >> 4);

	if (gotten.Limit < 1024 || gotten.Limit > 8192)
		tilesInstance.Limit = 2048;
	else
		tilesInstance.Limit = gotten.Limit;

	switch (gotten.tileSize)
	{

	case TILE_SIZE_SMALL:
	case TILE_SIZE_NORMAL:
	case TILE_SIZE_LARGE:
		tileSize = gotten.tileSize;
		break;

	default:
		tileSize = TILE_SIZE_NORMAL;
		break;

	}

}

namespace Save
{

	static const int size = HIGHSCORES_DATA_SIZE + BOARD_DATA_SIZE + CONFIG_DATA_SIZE;

	static const char* fileName = "wintiles.dat";

	static unsigned char readBuffer[size]{}, writeBuffer[size]{};

	FILE* file = nullptr;

}

bool Save::Read()
{

	if (fopen_s(&file, fileName, "rb") != 0)
	{
		tilesInstance.Start();
		return true;
	}

	fseek(file, 0, SEEK_END);
	const long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (fileSize == size)
	{

		const size_t read = fread(readBuffer, sizeof(unsigned char), fileSize, file);

		if (read == size)
		{
			highScores.FromBytes(readBuffer);
			tilesInstance.FromBytes(readBuffer + HIGHSCORES_DATA_SIZE);
			config::FromBytes(readBuffer + HIGHSCORES_DATA_SIZE + BOARD_DATA_SIZE);
		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}

	fclose(file);
	return true;

}

bool Save::Save()
{

	size_t written = 0;

	highScores.GetBytes(writeBuffer);
	tilesInstance.GetBytes(writeBuffer + HIGHSCORES_DATA_SIZE);
	config::GetBytes(writeBuffer + HIGHSCORES_DATA_SIZE + BOARD_DATA_SIZE);

	if (memcmp(readBuffer, writeBuffer, size) != 0)
	{

		if (fopen_s(&file, fileName, "wb") == 0)
		{
			written = fwrite(writeBuffer, sizeof(unsigned char), size, file);
			fclose(file);
		}

		return written == size;

	}

	return true;

}
