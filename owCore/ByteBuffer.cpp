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

ByteBuffer::ByteBuffer(size_t _size) :
	isFilled(false),
	isOnlyPointerToData(false),
	isEof(_size == 0),
	allocated(_size > 0),
	data(nullptr),
	position(0),
	bufferSize(_size)
{

	if (_size > 0)
	{
		data = new uint8_t[_size + 1];
		data[_size] = 0;
	}
}

ByteBuffer::ByteBuffer(uint8_t* _data, size_t _size) :
	isFilled(false),
	isOnlyPointerToData(false),
	isEof(_size == 0),
	allocated(_size > 0),
	data(nullptr),
	position(0),
	bufferSize(_size)
{

	if (_size > 0)
	{
		if (_data != nullptr)
		{
			data = _data;
			isFilled = true;
			isOnlyPointerToData = true;
		}
	}
}

ByteBuffer::~ByteBuffer()
{
	if (!isOnlyPointerToData)
		if (data != nullptr)
			delete data;

	isEof = true;
	allocated = false;
	data = nullptr;
	position = 0;
	bufferSize = 0;
}

//

void ByteBuffer::Allocate(size_t _size)
{
	isEof = (_size == 0);

	if (_size > 0)
	{
		if (!allocated)
		{
			data = new uint8_t[_size + 1];
			data[_size] = 0;
			allocated = true;
		}
	}

	position = 0;
	bufferSize = _size;
}

void ByteBuffer::CopyData(uint8_t* _data, size_t _size)
{
	if (!allocated)
		Allocate(_size);

	if (_size > bufferSize)
	{
		Debug::Error("ByteBuffer[]: Source data size [%d] bigger than allocated memory [%d]!", _size, bufferSize);
		Debug::Error("ByteBuffer[]: Copy part of source data.");
		_size = bufferSize;
	}

	if (_data != nullptr)
	{
		memcpy(data, _data, _size);
		isFilled = true;
	}

	position = 0;
}

void ByteBuffer::Init(uint8_t* _dataPtr, size_t _size)
{
	isEof = (_size == 0);

	if (_size > 0)
		if (_dataPtr != nullptr)
		{
			data = _dataPtr;
			isFilled = true;
			isOnlyPointerToData = true;
		}

	position = 0;
	bufferSize = _size;
}

//

const string ByteBuffer::ReadLine()
{
	if (isEof)
		return 0;

	// Find first incorrect symbol
	size_t lineEndPos;
	for (lineEndPos = position; (lineEndPos < bufferSize) && (data[lineEndPos] != '\n' && data[lineEndPos] != '\r'); lineEndPos++);

	// Find first correct symbol after incorrects
	size_t nextLineBeginPos;
	for (nextLineBeginPos = lineEndPos; (nextLineBeginPos < bufferSize) && (data[nextLineBeginPos] == '\n' || data[nextLineBeginPos] == '\r'); nextLineBeginPos++);

	size_t charsCount = lineEndPos - position;

	char* _string = new char[charsCount + 1];
	_string[charsCount] = '\0';
	ReadBytes(&_string[0], charsCount);

	// Skip \r and \n
	SeekRelative(nextLineBeginPos - lineEndPos);

	string line(_string);

	delete[] _string;

	return Utils::Trim(line);
}

const size_t ByteBuffer::ReadBytes(void* _destination, size_t _size)
{
	if (isEof)
		return 0;

	size_t posAfterRead = position + _size;
	if (posAfterRead > bufferSize)
	{
		_size = bufferSize - position;
		isEof = true;
	}

	memcpy(_destination, &(data[position]), _size);

	position = posAfterRead;

	return _size;
}

//

void ByteBuffer::Seek(size_t _bufferOffsetAbsolute)
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

void ByteBuffer::SeekRelative(size_t _bufferOffsetRelative)
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