#pragma once

#include "ByteBuffer.h"

class File : public IFile, public ByteBuffer
{
public:
	OW_CORE_DLL_API File();
	OW_CORE_DLL_API File(const File& _file);
	OW_CORE_DLL_API File(const ByteBuffer& _buffer);
	OW_CORE_DLL_API File(cstring _fullFileName);
	OW_CORE_DLL_API File(const char* _fullFileName);
	OW_CORE_DLL_API File(cstring _name, cstring _path);

	OW_CORE_DLL_API ~File();
	//

	OW_CORE_DLL_API File& operator=(const File& _file);
	OW_CORE_DLL_API File& operator=(cstring _fullFileName);
	OW_CORE_DLL_API File& operator=(const char* _fullFileName);

	//

	inline const string Name() const { return name; }
	inline const string Path() const { return path; }
	inline const string Extension() const { return extension; }

	inline const string Path_Name() const { return string(path + name); }

	//

	OW_CORE_DLL_API virtual bool Open(bool _isLocalFile = false) override;

	// STATIC MEMBERS

	static OW_CORE_DLL_API size_t getSize(cstring _name);
	static OW_CORE_DLL_API bool exists(cstring _name);

private:
	bool OpenLocalFile();
	void ParsePathAndExtension();

private:
	string name;
	string path;
	string extension;

protected:
	static bool usePackedGamedata;
	static const char* gamedata;
	static const char* archives;
};



