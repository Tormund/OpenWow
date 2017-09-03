#pragma once

#define DEF_MODULE_SIMPLE(_className) \
public:\
static _className* instance() { \
	static _className instance; \
	return &instance; \
}\
\
_className() { \
	inited = false; \
	moduleName = typeid(this).name(); \
	systemModule = false;\
}\
\
bool Init();\
void Destroy();

#define DEF_MODULE(_className, _dllApi) \
public:\
static _dllApi _className* instance() { \
	static _className instance; \
	return &instance; \
}\
\
_className() { \
	inited = false; \
	moduleName = typeid(this).name(); \
	systemModule = false;\
}\
\
_dllApi bool Init();\
_dllApi void Destroy();

class Module {
public:
	virtual bool Init() = 0;
	virtual void Destroy() = 0;

	inline void SetInited() { inited = true; }
	inline bool IsInited() const { return inited; }

	inline string GetName() const { return moduleName; }

	inline void SetSystemModule() { systemModule = true; }
	inline bool IsSystemModule() const { return systemModule; }

protected:
	bool inited;
	string moduleName;
	bool systemModule;
};