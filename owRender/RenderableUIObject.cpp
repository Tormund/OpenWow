#include "stdafx.h"

// General
#include "RenderableUIObject.h"

// Additional
#include "RenderableUIObjectCollection.h"

bool RenderableUIObject::Register(uint32 _order)
{
	RenderableUIObjectCollection::RegisterObject(this);
	SetDrawOrder(_order);

	return true;
}

void RenderableUIObject::Unregister()
{
	RenderableUIObjectCollection::UnregisterObject(this);
}
