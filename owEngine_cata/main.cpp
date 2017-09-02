#include "stdafx.h"

#include "GameState_Menu.h"

#include "shaders.h"

void shutdown(int _errCode);

int main(int argumentCount, char* arguments[])
{
	vector<string> argumentQueue;
	for (int i = 0; i < argumentCount; i++)
		argumentQueue.push_back(arguments[i]);

	// Init engine
	if (!_Engine->Init(argumentQueue))
		shutdown(1);

	/*vector<MPQArchive*> archives;
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/texture.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/model.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/wmo.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/terrain.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/interface.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/misc.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/dbc.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/patch.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft/Data/patch-2.MPQ"));*/

	/*archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/common.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/common-2.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/expansion.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/lichking.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/patch.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/patch-2.MPQ"));
	archives.push_back(new MPQArchive("D:/_games/World of Warcraft - Wrath of the Lich King/Data/patch-3.MPQ"));*/

	OpenDBs();

	initShaders();

	// Setting GameState
	if (!_Engine->SetGameState(new GameState_Menu))
		shutdown(1);

	// Main loop
	while (_Engine->Tick());

	/*for(auto it = archives.begin(); it != archives.end(); ++it)
		(*it)->close();
	archives.clear();*/

	shutdown(0);
}

void shutdown(int _errCode)
{
	_Engine->Destroy();
	Debug::Exit(_errCode);
}
