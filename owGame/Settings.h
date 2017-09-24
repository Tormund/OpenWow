#pragma once

#define ADD_SETTING(_type, name) \
public:\
 _type Get_##name##() const { \
return m_##name; \
}\
 void Set_##name##(_type _value)\
{\
m_##name = _value;\
}\
private:\
_type m_##name;


class Settings
{
	CLASS_INSTANCE_API(Settings, );

public:
	 Settings();

	ADD_SETTING(double, zaloopa);
};

#define _Settings Settings::instance()