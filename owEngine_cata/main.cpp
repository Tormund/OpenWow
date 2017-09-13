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

	MPQFile::AddArchive("art.MPQ");
	MPQFile::AddArchive("base-Win.MPQ");
	MPQFile::AddArchive("expansion1.MPQ");
	MPQFile::AddArchive("expansion2.MPQ");
	MPQFile::AddArchive("expansion3.MPQ");
	MPQFile::AddArchive("sound.MPQ");
	MPQFile::AddArchive("world.MPQ");
	MPQFile::AddArchive("world2.MPQ");

	MPQFile::AddArchive("wow-update-base-15211.MPQ");
	MPQFile::AddArchive("wow-update-base-15354.MPQ");
	MPQFile::AddArchive("wow-update-base-15595.MPQ");

	MPQFile::AddArchive("ruRU/expansion1-locale-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/expansion1-speech-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/expansion2-locale-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/expansion2-speech-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/expansion3-locale-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/expansion3-speech-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/locale-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/speech-ruRU.MPQ");
	MPQFile::AddArchive("ruRU/wow-update-ruRU-15211.MPQ");
	MPQFile::AddArchive("ruRU/wow-update-ruRU-15354.MPQ");
	MPQFile::AddArchive("ruRU/wow-update-ruRU-15595.MPQ");

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
