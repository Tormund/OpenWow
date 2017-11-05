#include "stdafx.h"

// General
#include "RenderableUIObjectCollection.h"

vector<RenderableUIObject*> RenderableUIObjectCollection::m_Objects;
bool                        RenderableUIObjectCollection::m_ObjectsNeedSort;

struct RenderableUIObjectCompare
{
	bool operator() (const RenderableUIObject* left, const RenderableUIObject* right) const
	{
		return left->GetDrawOrder() < right->GetDrawOrder();
	}
};

bool RenderableUIObjectCollection::RegisterObject(RenderableUIObject* _uiObject)
{
	m_Objects.push_back(_uiObject);
	m_ObjectsNeedSort = true;

	return true;
}

void RenderableUIObjectCollection::UnregisterObject(RenderableUIObject * _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
}

void RenderableUIObjectCollection::RenderUI()
{
	if (m_ObjectsNeedSort)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), RenderableUIObjectCompare());
		m_ObjectsNeedSort = false;
	}

	for (auto it : m_Objects)
	{
		it->RenderUI();
	}
}
