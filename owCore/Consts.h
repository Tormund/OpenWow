#pragma once

extern vec2 VECTOR_ZERO;

#define MAX_FRAMES_PER_SEC 60

#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define PI 3.14159265359f

// Pi / 4 * x
#define M_1_PI_4 0.78539816339
#define M_3_PI_4 2.35619449019
#define M_5_PI_4 3.92699081699
#define M_7_PI_4 5.49778714378
#define M_9_PI_4 7.06858347058

#define M_1_PI_6 0.52359877559
#define M_1_PI_12 0.26179938779

#define M_2_PI 6.28318530718

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

enum TextAlignW
{
	TEXT_ALIGNW_LEFT = 0,
	TEXT_ALIGNW_CENTER,
	TEXT_ALIGNW_RIGHT
};

enum TextAlignH
{
	TEXT_ALIGNH_TOP = 0,
	TEXT_ALIGNH_CENTER,
	TEXT_ALIGNH_BOTTOM
};

enum InputMode
{
	MODE_CHARACTERS = 0,
	MODE_NUMBERS,
	MODE_ANY
};
