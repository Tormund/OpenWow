#pragma once

#include "RenderableUIObject.h"

class RenderableUIObjectCollection
{
public:
	static bool RegisterObject(RenderableUIObject* _uiObject);
	static void UnregisterObject(RenderableUIObject* _uiObject);

	static void RenderUI();

private:
	static vector<RenderableUIObject*>   m_Objects;
	static bool                          m_ObjectsNeedSort;
};