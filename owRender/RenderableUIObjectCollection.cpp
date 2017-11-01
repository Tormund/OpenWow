#include "stdafx.h"

// General
#include "RenderableUIObjectCollection.h"

vector<RenderableUIObject*> RenderableUIObjectCollection::m_UIObjects;
bool                        RenderableUIObjectCollection::m_UIObjectsNeedSort;

struct RenderableUIObjectCompare
{
	bool operator() (const RenderableUIObject* left, const RenderableUIObject* right) const
	{
		return left->GetDrawOrder() < right->GetDrawOrder();
	}
};

bool RenderableUIObjectCollection::RegisterObject(RenderableUIObject* _uiObject)
{
	m_UIObjects.push_back(_uiObject);
	m_UIObjectsNeedSort = true;

	return true;
}

void RenderableUIObjectCollection::UnregisterObject(RenderableUIObject * _uiObject)
{
}

void RenderableUIObjectCollection::RenderUI()
{
	if (m_UIObjectsNeedSort)
	{
		std::sort(m_UIObjects.begin(), m_UIObjects.end(), RenderableUIObjectCompare());
		m_UIObjectsNeedSort = false;
	}

	for (auto it : m_UIObjects)
	{
		it->RenderUI();
	}
}
