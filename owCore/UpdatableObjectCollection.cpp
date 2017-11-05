#include "stdafx.h"

// General
#include "UpdatableObjectCollection.h"

vector<UpdatableObject*> UpdatableObjectCollection::m_Objects;

bool UpdatableObjectCollection::RegisterObject(UpdatableObject* _uiObject)
{
	m_Objects.push_back(_uiObject);

	return true;
}

void UpdatableObjectCollection::UnregisterObject(UpdatableObject * _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
}

void UpdatableObjectCollection::Update(double _Time, double _deltaTime)
{
    for (auto it : m_Objects)
    {
        it->Input(_Time, _deltaTime);
    }

	for (auto it : m_Objects)
	{
		it->Update(_Time, _deltaTime);
	}
}
