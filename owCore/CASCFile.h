#ifndef _CASCFILE_H_
#define _CASCFILE_H_

#include <CascLib\src\CascLib.h>

class CASCFolder;

#ifdef _WIN32
#    ifdef BUILDING_WOW_DLL
#        define _CASCFILE_API_ __declspec(dllexport)
#    else
#        define _CASCFILE_API_ __declspec(dllimport)
#    endif
#else
#    define _CASCFILE_API_
#endif

class _CASCFILE_API_ CASCFile
{
  public:
    CASCFile(cstring path, int id = -1);
    ~CASCFile();

  protected:
    virtual bool openFile();
    virtual bool isAlreadyOpened();
    virtual bool getFileSize(unsigned int & s);
    virtual unsigned long readFile();
    virtual void doPostOpenOperation();
    virtual bool doPostCloseOperation();

  private:
    HANDLE m_handle;
};




#endif /* _CASCFILE_H_ */
