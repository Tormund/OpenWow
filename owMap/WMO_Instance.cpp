#include "stdafx.h"

// General
#include "WMO_Instance.h"

WMOInstance::WMOInstance(WMO* _wmoObject, File& f) : wmoObject(_wmoObject)
{
	placementInfo = new WMOPlacementInfo;
	f.ReadBytes(placementInfo, WMOPlacementInfo::__size);

	// Convert rotation
	placementInfo->rotation = degToRad(placementInfo->rotation);
	placementInfo->rotation.x = -placementInfo->rotation.x;
	placementInfo->rotation.y = placementInfo->rotation.y - PI / 2.0;
}

WMOInstance::WMOInstance(WMO* _wmoObject, WMOPlacementInfo* _placementInfo) : wmoObject(_wmoObject)
{
	placementInfo = _placementInfo;
}

WMOInstance::~WMOInstance()
{
	delete placementInfo;
}

void WMOInstance::draw()
{
	if (alreadyDraw.find(placementInfo->uniqueId) != alreadyDraw.end())
	{
		return;
	}
	alreadyDraw.insert(placementInfo->uniqueId);

	//
	
	_Pipeline->Clear();
	{
		_Pipeline->Translate(placementInfo->position);

		_Pipeline->RotateX(placementInfo->rotation.z);
		_Pipeline->RotateY(placementInfo->rotation.y);
		_Pipeline->RotateZ(placementInfo->rotation.x);

		wmoObject->draw(placementInfo->doodadSetIndex);
		PERF_INC(PERF_MAP_MODELS_WMOs);
	}
}

void WMOInstance::reset()
{
	alreadyDraw.clear();
}

set<uint32> WMOInstance::alreadyDraw;
