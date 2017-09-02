#pragma once

#include <list>
#include "File.h"

class WowChunk : public ByteBuffer
{
public:
	WowChunk(char* _name, uint8_t* _dataPtr, size_t _size) : ByteBuffer(_dataPtr, _size)
	{
		name = new char[5];
		strncpy_s(name, 4, _name, 4);
		name[4] = 0;
	}
	~WowChunk()
	{
		ByteBuffer::~ByteBuffer();
	}

	const char* GetName() const
	{
		return name;
	}

	bool NameIs(const char* _name) const
	{
		return strncmp(name, _name, 4) == 0;
	}

private:
	char* name;
};

class WowChunkFile : public File
{
public:
	OW_CORE_DLL_API WowChunkFile(WowChunk* _chunk);
	WowChunkFile(const File& _file) : File(_file), chunksInited(false), chunksIterator(chunks.begin()) {}
	WowChunkFile(const ByteBuffer& _buffer) : File(_buffer), chunksInited(false), chunksIterator(chunks.begin()) {}
	OW_CORE_DLL_API ~WowChunkFile();

	//

	OW_CORE_DLL_API bool InitChunks();

	//
	OW_CORE_DLL_API void PrintAllChunks();
	inline size_t GetChunksSize() const { return chunks.size(); }
	OW_CORE_DLL_API WowChunk* GetChunk(cstring _name);
	OW_CORE_DLL_API WowChunk* GetNext();

private:
	bool chunksInited;
	std::list<WowChunk*> chunks;
	std::list<WowChunk*>::iterator chunksIterator;
};

inline void flipcc(char* fcc)
{
	std::swap(fcc[0], fcc[3]);
	std::swap(fcc[1], fcc[2]);
}