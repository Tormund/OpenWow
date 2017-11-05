#pragma once

#include "UpdatableObject.h"

class UpdatableObjectCollection
{
public:
	static bool RegisterObject(UpdatableObject* _uiObject);
	static void UnregisterObject(UpdatableObject* _uiObject);

	static void Update(double _Time, double _deltaTime);

private:
	static vector<UpdatableObject*> m_Objects;
};