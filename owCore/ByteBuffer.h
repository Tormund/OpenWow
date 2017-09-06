#pragma once

class ByteBuffer
{
public:
	OW_CORE_DLL_API ByteBuffer();
	OW_CORE_DLL_API ByteBuffer(const ByteBuffer& _other);

	OW_CORE_DLL_API ByteBuffer(uint64_t _size);
	OW_CORE_DLL_API ByteBuffer(uint8_t* _data, uint64_t _size);
	OW_CORE_DLL_API ~ByteBuffer();

	//

	OW_CORE_DLL_API void Allocate(uint64_t _size);
	OW_CORE_DLL_API void CopyData(uint8_t* _data, uint64_t _size);
	OW_CORE_DLL_API void Init(uint8_t* _dataPtr, uint64_t _size);

	//

	OW_CORE_DLL_API const string ReadLine();
	OW_CORE_DLL_API const void ReadBytes(void* _destination, uint64_t _size);

	//

	inline uint64_t GetSize() const { return bufferSize; }
	inline uint64_t GetPos() const { return position; }
	inline const uint8_t* GetData() const { return data; }
	inline uint8_t* GetDataFromCurrent() { return data + position; }
	inline bool IsEof() const { return isEof; }

	OW_CORE_DLL_API void Seek(uint64_t _bufferOffsetAbsolute);
	OW_CORE_DLL_API void SeekRelative(uint64_t _bufferOffsetRelative);

protected:
	bool isFilled;
	bool isOnlyPointerToData;

	bool isEof;
	bool allocated;
	uint8_t* data;
	uint64_t position;
	uint64_t bufferSize;
};