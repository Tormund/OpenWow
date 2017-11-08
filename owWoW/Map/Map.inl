#pragma once

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