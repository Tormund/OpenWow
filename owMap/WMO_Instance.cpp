#include "stdafx.h"

// General
#include "WMO_Instance.h"

WMOInstance::WMOInstance(WMO* _wmoObject, File& f) : wmoObject(_wmoObject)
{
	placementInfo = new WMOPlacementInfo;
	f.ReadBytes(placementInfo, WMOPlacementInfo::__size);
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

	glPushMatrix();
	{
		glTranslatef(placementInfo->position.x, placementInfo->position.y, placementInfo->position.z);

		glRotatef(placementInfo->rotation.y - 90.0f, 0, 1, 0);
		glRotatef(-placementInfo->rotation.x, 0, 0, 1);
		glRotatef(placementInfo->rotation.z, 1, 0, 0);

		wmoObject->draw(placementInfo->doodadSetIndex, placementInfo->position, placementInfo->rotation.y - 90.0f);
	}
	glPopMatrix();
}

void WMOInstance::reset()
{
	alreadyDraw.clear();
}

set<uint32_t> WMOInstance::alreadyDraw;