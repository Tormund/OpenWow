#pragma once

inline int indexMapBuf(int x, int y)
{
	return ((y + 1) / 2) * 9 + (y / 2) * 8 + x;
}

template <class V>
inline void stripify(V *in, V *out)
{
	for (int row = 0; row < 8; row++)
	{
		V* thisrow = &in[indexMapBuf(0, row * 2)];
		V* nextrow = &in[indexMapBuf(0, (row + 1) * 2)];

		if (row > 0)
		{
			*out++ = thisrow[0];
		}

		for (int col = 0; col < 9; col++)
		{
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}

		if (row < 7)
			*out++ = nextrow[8];
	}
}

// high res version, size = 16*18 + 7*2 + 8*2
template <class V>
inline void stripify2(V *in, V *out)
{
	for (int row = 0; row < 8; row++)
	{
		V *thisrow = &in[indexMapBuf(0, row * 2)];
		V *nextrow = &in[indexMapBuf(0, row * 2 + 1)];
		V *overrow = &in[indexMapBuf(0, (row + 1) * 2)];

		if (row > 0) *out++ = thisrow[0];// jump end
		for (int col = 0; col < 8; col++)
		{
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}
		*out++ = thisrow[8];
		*out++ = overrow[8];
		*out++ = overrow[8];// jump start
		*out++ = thisrow[0];// jump end
		*out++ = thisrow[0];
		for (int col = 0; col < 8; col++)
		{
			*out++ = overrow[col];
			*out++ = nextrow[col];
		}
		if (row < 8) *out++ = overrow[8];
		if (row < 7) *out++ = overrow[8];// jump start
	}
}

inline bool IsBadTileIndex(int i, int j)
{
	if (i < 0)
	{
		return true;
	}

	if (j < 0)
	{
		return true;
	}

	if (i >= C_TilesInMap)
	{
		return true;
	}

	if (j >= C_TilesInMap)
	{
		return true;
	}

	return false;
}

inline bool IsGoodTileIndex(int i, int j)
{
	return (!IsBadTileIndex(i, j));
}