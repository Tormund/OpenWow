#include "stdafx.h"

// General
#include "InfoPortions.h"


void InfoPortions::SetInfoPortion(cstring _infoPortionName)
{
	auto _debugInfoPortions = &debugInfoPortions;

	// Already exists
	if (find(_debugInfoPortions->begin(), _debugInfoPortions->end(), _infoPortionName) != _debugInfoPortions->end())
	{
		return;
	}

	_debugInfoPortions->push_back(_infoPortionName);
}

void InfoPortions::DeleteInfoPortion(cstring _infoPortionName)
{
	auto _debugInfoPortions = &debugInfoPortions;
	auto _debugInfoPortionsIt = find(_debugInfoPortions->begin(), _debugInfoPortions->end(), _infoPortionName);

	// Not exists
	if (_debugInfoPortionsIt == _debugInfoPortions->end())
	{
		return;
	}

	// Erase
	_debugInfoPortions->erase(_debugInfoPortionsIt);
}

bool InfoPortions::GetInfoPortion(cstring _infoPortionName)
{
	auto _debugInfoPortions = &debugInfoPortions;
	auto _debugInfoPortionsIt = find(_debugInfoPortions->begin(), _debugInfoPortions->end(), _infoPortionName);

	// Not exists
	if (_debugInfoPortionsIt == _debugInfoPortions->end())
	{
		return false;
	}

	return true;
}