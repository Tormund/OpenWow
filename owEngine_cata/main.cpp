#include "stdafx.h"

#include "GameState_Menu.h"

void shutdown(int _errCode);

int main(int argumentCount, char* arguments[])
{
	vector<string> argumentQueue;
	for (int i = 0; i < argumentCount; i++)
	{
		argumentQueue.push_back(arguments[i]);
	}

	MPQFile::AddArchive(string("art.MPQ"));
	MPQFile::AddArchive(string("base-Win.MPQ"));
	MPQFile::AddArchive(string("expansion1.MPQ"));
	MPQFile::AddArchive(string("expansion2.MPQ"));
	MPQFile::AddArchive(string("expansion3.MPQ"));
	MPQFile::AddArchive(string("sound.MPQ"));
	MPQFile::AddArchive(string("world.MPQ"));
	MPQFile::AddArchive(string("world2.MPQ"));

	MPQFile::AddArchive(string("wow-update-base-15211.MPQ"));
	MPQFile::AddArchive(string("wow-update-base-15354.MPQ"));
	MPQFile::AddArchive(string("wow-update-base-15595.MPQ"));

	MPQFile::AddArchive(string("ruRU/expansion1-locale-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/expansion1-speech-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/expansion2-locale-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/expansion2-speech-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/expansion3-locale-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/expansion3-speech-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/locale-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/speech-ruRU.MPQ"));
	MPQFile::AddArchive(string("ruRU/wow-update-ruRU-15211.MPQ"));
	MPQFile::AddArchive(string("ruRU/wow-update-ruRU-15354.MPQ"));
	MPQFile::AddArchive(string("ruRU/wow-update-ruRU-15595.MPQ"));

	// Init engine
	if (!_Engine->Init(argumentQueue))
	{
		_Engine->Destroy(1);
	}

	OpenDBs();
	initShaders();

	// Setting GameState
	if (!_Engine->SetGameState(new GameState_Menu))
	{
		_Engine->Destroy(2);
	}

	// Main loop
	while (_Engine->Tick());

	// Clear arhives
	MPQFile::ClearArchives();

	_Engine->Destroy();
}
