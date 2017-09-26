#include "stdafx.h"

// General
#include "WMO_Instance.h"

WMOInstance::WMOInstance(WMO* _wmoObject, File& f) : wmoObject(_wmoObject)
{
	placementInfo = new WMOPlacementInfo;
	f.ReadBytes(placementInfo, WMOPlacementInfo::__size);

	// Convert rotation
	placementInfo->rotation = glm::radians(placementInfo->rotation);
	placementInfo->rotation.x = -placementInfo->rotation.x;
	placementInfo->rotation.y = placementInfo->rotation.y - PI / 2.0;


	// Debug
	//Debug::Info("OFFSET [%f, %f, %f] ROT = [%f]", placementInfo->position.x, placementInfo->position.y, placementInfo->position.z, placementInfo->rotation.x);
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

	if (!_Settings->disable_pipeline)
	{
		_Pipeline->Translate(placementInfo->position);

		_Pipeline->RotateX(placementInfo->rotation.z);
		_Pipeline->RotateY(placementInfo->rotation.y);
		_Pipeline->RotateZ(placementInfo->rotation.x);
		
	}

	wmoObject->draw(placementInfo->doodadSetIndex, placementInfo->position, placementInfo->rotation.x);
	_Perfomance->Inc(PERF_WMOs);
}

void WMOInstance::reset()
{
	alreadyDraw.clear();
}

set<uint32_t> WMOInstance::alreadyDraw;