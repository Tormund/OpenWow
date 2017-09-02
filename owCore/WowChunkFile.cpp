#include "stdafx.h"

// General
#include "WowChunkFile.h"

WowChunkFile::WowChunkFile(WowChunk* _chunk)
{
	//Init(_chunk->data, _chunk->GetSize());

}

WowChunkFile::~WowChunkFile()
{
	if (!chunksInited)
	{
		if (chunksIterator != chunks.end())
		{
			//Debug::Error("WowChunkFile[%s]: Not all data readed!", Name().c_str());
			//Debug::Error("WowChunkFile[%s]: Missed data:", Name().c_str());
			//WowChunk* chunk;
			//while((chunk = GetNext()) != nullptr) {
			//	Debug::Info("WowChunkFile[%s]: Name [%s](%d)", Name().c_str(), chunk->name, chunk->Size());
			//}
		}

		for (auto it = chunks.begin(); it != chunks.end();)
		{
			auto elem = *it;
			it = chunks.erase(it);
			delete elem;
		}
	}

	File::~File();
}

bool WowChunkFile::InitChunks()
{
	if (GetSize() == 0)
	{
		Debug::Error("WowChunkFile[%s]: Can't init chunks from empty buffer.", Path_Name().c_str());
		return false;
	}

	while (!IsEof())
	{
		char chunkName[5];
		ReadBytes(chunkName, 4);
		std::swap(chunkName[0], chunkName[3]);
		std::swap(chunkName[1], chunkName[2]);
		chunkName[4] = 0;

		size_t chunkSize;
		ReadBytes(&chunkSize, 4);

		size_t nextpos = GetPos() + chunkSize;

		WowChunk* chunk = new WowChunk(chunkName, data + GetPos(), chunkSize);
		chunks.push_back(chunk);

		//Debug::Info("WowChunkFile[%s]: Chunk [%s] Size (%d) added.", Name().c_str(), chunkName, chunkSize);

		if (chunk->NameIs("MCNR"))
		{
			nextpos += 3 * 3 + 2 * 2;
		}

		Seek(nextpos);
	}

	if (chunks.size() == 0)
	{
		Debug::Error("WowChunkFile[%s]: CHUNKS IS ZERO!!!!", Path_Name().c_str());
		return false;
	}

	chunksIterator = chunks.begin();
	chunksInited = true;

	return true;
}

void WowChunkFile::PrintAllChunks()
{
	for (auto it = chunks.begin(); it != chunks.end(); ++it)
		Debug::Info("[%s]", (*it)->GetName());
}

WowChunk* WowChunkFile::GetChunk(cstring _name)
{
	assert1(!_name.empty());

	if (!chunksInited)
	{
		Debug::Error("WowChunkFile[%s]: Chunks not inited!", Path_Name().c_str());
		return nullptr;
	}

	for (auto it = chunks.begin(); it != chunks.end(); ++it)
		if ((*it)->NameIs(_name.c_str()))
			return *it;

	return nullptr;
}

WowChunk* WowChunkFile::GetNext()
{
	if (!chunksInited)
	{
		Debug::Error("WowChunkFile[%s]: Chunks not inited!", Path_Name().c_str());
		return nullptr;
	}

	if (chunksIterator == chunks.end())
		return nullptr;

	return *(chunksIterator++);
}
