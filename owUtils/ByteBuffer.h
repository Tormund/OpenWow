#pragma once

class ByteBuffer
{
public:
	 ByteBuffer();
	 ByteBuffer(const ByteBuffer& _other);

	 ByteBuffer(uint64_t _size);
	 ByteBuffer(uint8* _data, uint64_t _size);
	 ~ByteBuffer();

	//

	 void Allocate(uint64_t _size);
	 void SetFilled();
	 void CopyData(uint8* _data, uint64_t _size);
	 void Init(uint8* _dataPtr, uint64_t _size);
     void Clear();

	//

	 const string ReadLine();
	 const void ReadBytes(void* _destination, uint64_t _size);

	//

	inline uint64_t GetSize() const { return bufferSize; }
	inline uint64_t GetPos() const { return position; }
	inline const uint8* GetData() const { return data; }
	inline uint8* GetDataFromCurrent() { return data + position; }
	inline bool IsEof() const { return isEof; }

	 void Seek(uint64_t _bufferOffsetAbsolute);
	 void SeekRelative(uint64_t _bufferOffsetRelative);

protected:
	bool isFilled;
	bool isOnlyPointerToData;

	bool isEof;
	bool allocated;
	uint8* data;
	uint64_t position;
	uint64_t bufferSize;
};