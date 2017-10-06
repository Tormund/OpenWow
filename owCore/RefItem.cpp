#include "stdafx.h"

// General
#include "RefItem.h"


void RefItem::AddRef()
{
	m_RefsCount += 1;
}

void RefItem::DelRef()
{
	if (NeedDelete())
	{
		assert1(false);
		return;
	}

	m_RefsCount -= 1;
}