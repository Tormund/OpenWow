#pragma once

__interface IFile
{
	OW_CORE_DLL_API virtual bool Open(bool _isLocalFile = false) = 0;
};