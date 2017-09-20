#pragma once

class InfoPortions
{
	CLASS_INSTANCE_API(InfoPortions, OW_GAME_DLL_API);

public:
	OW_GAME_DLL_API void SetInfoPortion(cstring _infoPortionName);
	OW_GAME_DLL_API void DeleteInfoPortion(cstring _infoPortionName);
	OW_GAME_DLL_API bool GetInfoPortion(cstring _infoPortionName);

private:
	vector<string> debugInfoPortions;
};

#define _InfoPortions InfoPortions::instance()