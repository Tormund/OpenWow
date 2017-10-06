#pragma once

#include "RefItem.h"

class RefItemNamed : public RefItem
{
public:
	RefItemNamed(cstring _name) : RefItem(), name(_name) {}
	virtual ~RefItemNamed() override { Debug::Warn("Item [%s] deleted.", name.c_str()); }

	cstring GetName() const { return name; }

private:
	std::string name;
};