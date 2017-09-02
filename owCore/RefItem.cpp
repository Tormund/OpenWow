#include "stdafx.h"

// General
#include "RefItem.h"

RefItem::RefItem() : refcount(0)
{}

void RefItem::AddRef()
{
	refcount += 1;
}

void RefItem::DelRef()
{
	if (NeedDelete())
	{
		assert1(false);
		return;
	}

	refcount -= 1;
}