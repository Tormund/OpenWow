#include "../stdafx.h"

// General
#include "WMO_Instance.h"

WMOInstance::WMOInstance(WMO* _wmoObject, WMOPlacementInfo* _placementInfo) : 
    wmoObject(_wmoObject),
    placementInfo(_placementInfo)
{
    assert1(wmoObject);
    assert1(_placementInfo);

	CalculateMatrix();
}

WMOInstance::~WMOInstance()
{
	delete placementInfo;
}

//

void WMOInstance::Render()
{
	if (alreadyDraw.find(placementInfo->uniqueId) != alreadyDraw.end())
	{
		return;
	}
	alreadyDraw.insert(placementInfo->uniqueId);

	//
	
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_RelTransform);

		wmoObject->Render(placementInfo->doodadSetIndex);
		PERF_INC(PERF_MAP_MODELS_WMOs);
	}
}

void WMOInstance::CalculateMatrix()
{
	// Convert rotation
	placementInfo->rotation = degToRad(placementInfo->rotation);
	placementInfo->rotation.x = -placementInfo->rotation.x;
	placementInfo->rotation.y = placementInfo->rotation.y - PI / 2.0;

	// Build relative matrix
	m_RelTransform.translate(placementInfo->position);
	m_RelTransform.rotate(placementInfo->rotation.z, placementInfo->rotation.y, placementInfo->rotation.x);

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}
}

//

void WMOInstance::reset()
{
	alreadyDraw.clear();
}

set<uint32> WMOInstance::alreadyDraw;
