#pragma once

#define ADD_SETTING(_type, name) \
public:\
OW_GAME_DLL_API _type Get_##name##() const { \
return m_##name; \
}\
OW_GAME_DLL_API void Set_##name##(_type _value)\
{\
m_##name = _value;\
}\
private:\
_type m_##name;


class Settings
{
	CLASS_INSTANCE_API(Settings, OW_GAME_DLL_API);

public:
	OW_GAME_DLL_API Settings();

	ADD_SETTING(double, zaloopa);
};

#define _Settings Settings::instance()