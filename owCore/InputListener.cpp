#include "stdafx.h"

// General
#include "InputListener.h"

// Additional
#include "InputListenerCollection.h"

bool InputListenerObject::Register()
{
    InputListenerObjectCollection::RegisterObject(this);

    return true;
}

void InputListenerObject::Unregister()
{
    InputListenerObjectCollection::UnregisterObject(this);
}