#include "stdafx.h"

// General
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer() :
	isFilled(false),
	isOnlyPointerToData(false),
	isEof(true),
	allocated(false),
	data(nullptr),
	position(0),
	bufferSize(0)
{}

ByteBuffer::ByteBuffer(const ByteBuffer& _other) :
	isFilled(false),
	isOnlyPointerToData(false),
	isEof(_other.bufferSize == 0),
	allocated(_other.bufferSize > 0),
	data(nullptr),
	position(0),
	bufferSize(_other.bufferSize)
{

	if (_other.bufferSize > 0)
	{
		if (_other.data != nullptr)
		{
			data = _other.data;
			isFilled = true;
			isOnlyPointerToData = true;
		}
	}
}

ByteBuffer::ByteBuffer(uint64_t _size) :
	isFilled(false),
	isOnlyPointerToData(false),
	isEof(true),
	allocated(false),
	data(nullptr),
	position(0),
	bufferSize(0)
{
	Allocate(_size);
}

ByteBuffer::ByteBuffer(uint8* _data, uint64_t _size) :
	isFilled(false),
	isOnlyPointerToData(false),
	isEof(true),
	allocated(false),
	data(nullptr),
	position(0),
	bufferSize(0)
{

	Init(_data, _size);
}

ByteBuffer::~ByteBuffer()
{
	if (!allocated)
	{
		return;
	}

	if (isOnlyPointerToData)
	{
		return;
	}

	if (data != nullptr)
	{
		//Modules::log().Error("Data deleted!!!");
		delete[] data;
	}
}

//

void ByteBuffer::Allocate(uint64_t _size)
{
	isEof = true;

	if (_size > 0)
	{
		if (!allocated)
		{
			data = new uint8[_size];
			allocated = true;
		}
		else
		{
			fail1();
		}
	}

	position = 0;
	bufferSize = _size;
}

void ByteBuffer::SetFilled()
{
	isEof = (bufferSize == 0);

	isFilled = true;
	isOnlyPointerToData = false;
}

void ByteBuffer::CopyData(uint8* _data, uint64_t _size)
{
	if (!allocated)
	{
		Allocate(_size);
	}

	if (_size > bufferSize)
	{
		//Modules::log().Error("ByteBuffer[]: Source data size [%d] bigger than allocated memory [%d]!", _size, bufferSize);
		//Modules::log().Error("ByteBuffer[]: Copy part of source data.");
		_size = bufferSize;
	}

	if (_data != nullptr)
	{
		memcpy(data, _data, _size);
		isFilled = true;
	}

	position = 0;
}

void ByteBuffer::Init(uint8* _dataPtr, uint64_t _size)
{
	isEof = (_size == 0);

	if (_size > 0)
	{
		if (_dataPtr != nullptr)
		{
			data = _dataPtr;
			isFilled = true;
			isOnlyPointerToData = true;
		}
	}

	position = 0;
	bufferSize = _size;
}

//

const string ByteBuffer::ReadLine()
{
	if (isEof)
	{
		return "";
	}

	// Find first incorrect symbol
	uint64_t lineEndPos;
	for (lineEndPos = position; (lineEndPos < bufferSize) && (data[lineEndPos] != '\n' && data[lineEndPos] != '\r'); lineEndPos++);

	// Find first correct symbol after incorrects
	uint64_t nextLineBeginPos;
	for (nextLineBeginPos = lineEndPos; (nextLineBeginPos < bufferSize) && (data[nextLineBeginPos] == '\n' || data[nextLineBeginPos] == '\r'); nextLineBeginPos++);

	uint64_t charsCount = lineEndPos - position;

	char* _string = new char[charsCount + 1];
	_string[charsCount] = '\0';
	ReadBytes(&_string[0], charsCount);

	// Skip \r and \n
	SeekRelative(nextLineBeginPos - lineEndPos);

	string line(_string);

	delete[] _string;

	return Utils::Trim(line);
}

const void ByteBuffer::ReadBytes(void* _destination, uint64_t _size)
{
	if (isEof)
	{
		return;
	}

	uint64_t posAfterRead = position + _size;
	if (posAfterRead > bufferSize)
	{
		_size = bufferSize - position;
		isEof = true;
	}

	memcpy(_destination, &(data[position]), _size);

	position = posAfterRead;
}

//

void ByteBuffer::Seek(uint64_t _bufferOffsetAbsolute)
{
	if (_bufferOffsetAbsolute >= bufferSize)
	{
		position = bufferSize;
		isEof = true;
	}
	else
	{
		position = _bufferOffsetAbsolute;
		isEof = false;
	}
}

void ByteBuffer::SeekRelative(uint64_t _bufferOffsetRelative)
{
	if (position + _bufferOffsetRelative >= bufferSize)
	{
		position = bufferSize;
		isEof = true;
	}
	else
	{
		position += _bufferOffsetRelative;
		isEof = false;
	}
}