#pragma once

class InfoPortions
{
	CLASS_INSTANCE(InfoPortions);

public:
	 void SetInfoPortion(cstring _infoPortionName);
	 void DeleteInfoPortion(cstring _infoPortionName);
	 bool GetInfoPortion(cstring _infoPortionName);

private:
	vector<string> debugInfoPortions;
};

#define _InfoPortions InfoPortions::instance()