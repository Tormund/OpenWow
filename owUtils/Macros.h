#pragma once

// =================================================================================================
// Build settings
// =================================================================================================

#define DOODADS_INCL

//#define MDX_PARTICLES_ENABLE

// =================================================================================================
// USEFULL MACROS
// =================================================================================================

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