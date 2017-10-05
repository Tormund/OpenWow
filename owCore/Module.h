#pragma once

//-----------------------------------------

#define DEF_MODULE(_className) \
CLASS_INSTANCE(_className)\
_className() \
{ \
	inited = false; \
	moduleName = typeid(this).name(); \
}\
\
bool Init();\
void Destroy();

//------------------------------------------

/*#define DEF_MODULE_API(_className, _dllApi) \
CLASS_INSTANCE_API(_className, _dllApi) \
_className() \
{ \
	inited = false; \
	moduleName = typeid(this).name(); \
}\
\
_dllApi bool Init();\
_dllApi void Destroy();*/

//------------------------------------------

class Module
{
public:
	virtual bool Init() = 0;
	virtual void Destroy() = 0;

	inline void SetInited() { inited = true; }
	inline bool IsInited() const { return inited; }

	inline string GetName() const { return moduleName; }

protected:
	bool inited;
	string moduleName;
};
