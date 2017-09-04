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

	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/art.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/base-Win.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/expansion1.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/expansion2.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/expansion3.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/sound.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/world.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/world2.MPQ");

	MPQFile::AddArchive("wow-update-base-15211.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/wow-update-base-15354.MPQ");
	MPQFile::AddArchive("D:/_games/World of Warcraft/Data/wow-update-base-15595.MPQ");

	OpenDBs();

	initShaders();

	// Setting GameState
	if (!_Engine->SetGameState(new GameState_Menu))
		shutdown(1);

	// Main loop
	while (_Engine->Tick());


	// Clear arhives
	//for(auto it = archives.begin(); it != archives.end(); ++it)
	//	(*it)->close();
	//archives.clear();
	MPQFile::ClearArchives();

	shutdown(0);
}

void shutdown(int _errCode)
{
	_Engine->Destroy();
	Debug::Exit(_errCode);
}
