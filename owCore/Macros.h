#pragma once

// Settings

//#define WMO_INCL
//#define MDX_INCL
//#define MDX_PARTICLES_ENABLE

//

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define GL_BUFFER_OFFSET(i) ((char*)(0) + (i))


#define CLASS_INSTANCE(_className)\
public:\
static _className* instance()\
{\
	static _className instance;\
	return &instance;\
}

#define CLASS_INSTANCE_API(_className, _dllApi)\
public:\
static _dllApi _className* instance()\
{\
	static _className instance;\
	return &instance;\
}

#define ERASE_VECTOR(_name) \
for (auto it = _name.begin(); it != _name.end();) \
{ \
    auto obj = *it; \
    it = _name.erase(it); \
    delete obj; \
}