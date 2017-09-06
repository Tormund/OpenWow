#include "stdafx.h"

#include "GameState_Menu.h"

#include "shaders.h"

void shutdown(int _errCode);

int main(int argumentCount, char* arguments[])
{
	//_Engine->QuickStart(argumentCount, arguments, new GameState_Menu);

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
		shutdown(1);
	}

	OpenDBs();
	initShaders();

	// Setting GameState
	if (!_Engine->SetGameState(new GameState_Menu))
	{
		shutdown(2);
	}

	// Main loop
	while (_Engine->Tick());

	// Clear arhives
	MPQFile::ClearArchives();

	shutdown(0);
}

void shutdown(int _errCode)
{
	_Engine->Destroy();
	Debug::Exit(_errCode);
}
