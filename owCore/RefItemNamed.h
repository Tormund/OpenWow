#pragma once

#include "RefItem.h"

class RefItemNamed : public RefItem
{
public:
	RefItemNamed(cstring _name);
	inline virtual ~RefItemNamed() { Debug::Warn("Item [%s] deleted.", name.c_str()); }

	inline cstring GetName() const { return name; }

private:
	std::string name;
};