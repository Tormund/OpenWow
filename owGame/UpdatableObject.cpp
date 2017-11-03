#include "stdafx.h"

// General
#include "UpdatableObject.h"

// Additional
#include "UpdatableObjectCollection.h"

bool UpdatableObject::Register()
{
    UpdatableObjectCollection::RegisterObject(this);
	return true;
}

void UpdatableObject::Unregister()
{
    UpdatableObjectCollection::UnregisterObject(this);
}
