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

	Debug::Init();

	MPQFile::AddArchive(string("art.MPQ"));
	MPQFile::AddArchive(string("base-Win.MPQ"));
	MPQFile::AddArchive(string("expansion1.MPQ"));
	MPQFile::AddArchive(string("expansion2.MPQ"));
	MPQFile::AddArchive(string("expansion3.MPQ"));
	//MPQFile::AddArchive(string("sound.MPQ"));
	MPQFile::AddArchive(string("world.MPQ"));
	MPQFile::AddArchive(string("world2.MPQ"));

	MPQFile::AddArchive(string("wow-update-base-15211.MPQ"));
	MPQFile::AddArchive(string("wow-update-base-15354.MPQ"));
	MPQFile::AddArchive(string("wow-update-base-15595.MPQ"));

	MPQFile::AddArchive(string("enUS/expansion1-locale-enUS.MPQ"));
	//MPQFile::AddArchive(string("enUS/expansion1-speech-enUS.MPQ"));
	MPQFile::AddArchive(string("enUS/expansion2-locale-enUS.MPQ"));
	//MPQFile::AddArchive(string("enUS/expansion2-speech-enUS.MPQ"));
	MPQFile::AddArchive(string("enUS/expansion3-locale-enUS.MPQ"));
	//MPQFile::AddArchive(string("enUS/expansion3-speech-enUS.MPQ"));
	MPQFile::AddArchive(string("enUS/locale-enUS.MPQ"));
	//MPQFile::AddArchive(string("enUS/speech-enUS.MPQ"));
	MPQFile::AddArchive(string("enUS/wow-update-enUS-15211.MPQ"));
	MPQFile::AddArchive(string("enUS/wow-update-enUS-15354.MPQ"));
	MPQFile::AddArchive(string("enUS/wow-update-enUS-15595.MPQ"));

	// Init engine
	if (!_Engine->Init(argumentQueue))
	{
		_Engine->Destroy(1);
	}

	OpenDBs();

	// Setting GameState
	if (!_Engine->SetGameState(new GameState_Menu))
	{
		_Engine->Destroy(2);
	}

	Matrix m = glQuaternionRotate(vec3(-0.501694f, 0.510347f, 0.483113f), 0.504432f);
	Debug::Info("m [%f][%f][%f][%f]", m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	Debug::Info("m [%f][%f][%f][%f]", m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	Debug::Info("m [%f][%f][%f][%f]", m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	Debug::Info("m [%f][%f][%f][%f]", m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);

	mat4 mat = glQuaternionRotate2(vec3(-0.501694f, 0.510347f, 0.483113f), 0.504432f);
	Debug::Info("m2 [%f][%f][%f][%f]", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
	Debug::Info("m2 [%f][%f][%f][%f]", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
	Debug::Info("m2 [%f][%f][%f][%f]", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
	Debug::Info("m2 [%f][%f][%f][%f]", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

	// Main loop
	while (_Engine->Tick());

	// Clear arhives
	MPQFile::ClearArchives();

	_Engine->Destroy();
}
