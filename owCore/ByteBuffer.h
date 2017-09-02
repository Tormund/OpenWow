#pragma once

class ByteBuffer
{
public:
	OW_CORE_DLL_API ByteBuffer();
	OW_CORE_DLL_API ByteBuffer(const ByteBuffer& _other);

	OW_CORE_DLL_API ByteBuffer(size_t _size);
	OW_CORE_DLL_API ByteBuffer(uint8_t* _data, size_t _size);
	OW_CORE_DLL_API ~ByteBuffer();

	//

	OW_CORE_DLL_API void Allocate(size_t _size);
	OW_CORE_DLL_API void CopyData(uint8_t* _data, size_t _size);
	OW_CORE_DLL_API void Init(uint8_t* _dataPtr, size_t _size);

	//

	OW_CORE_DLL_API const string ReadLine();
	OW_CORE_DLL_API const size_t ReadBytes(void* _destination, size_t _size);

	//

	inline size_t GetSize() const { return bufferSize; }
	inline size_t GetPos() const { return position; }
	inline const uint8_t* GetData() const { return data; }
	inline uint8_t* GetDataFromCurrent() { return data + position; }
	inline bool IsEof() const { return isEof; }

	OW_CORE_DLL_API void Seek(size_t _bufferOffsetAbsolute);
	OW_CORE_DLL_API void SeekRelative(size_t _bufferOffsetRelative);

protected:
	bool isFilled;
	bool isOnlyPointerToData;

	bool isEof;
	bool allocated;
	uint8_t* data;
	size_t position;
	size_t bufferSize;
};