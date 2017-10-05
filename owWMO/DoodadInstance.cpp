#include "stdafx.h"

// General
#include "DoodadInstance.h"

DoodadInstance::DoodadInstance(File& f)
{
	placementInfo = new DoodadPlacementInfo;
	f.ReadBytes(placementInfo, DoodadPlacementInfo::__size);

	placementInfo->position = From_XYZ_To_XZminusY_RET(placementInfo->position);

	//Debug::Error("QUAT = [%f %f %f %f]", placementInfo->orientation.x, placementInfo->orientation.y, placementInfo->orientation.z, placementInfo->orientation.w);

	//placementInfo->orientation.x = glm::radians(placementInfo->orientation.x);
	//placementInfo->orientation.y = glm::radians(placementInfo->orientation.y);
	//placementInfo->orientation.z = glm::radians(placementInfo->orientation.z);



	//uint32 d1;
	//f.ReadBytes(&d1, 4); // (B,G,R,A) Lightning-color. 
	//lcol = fromARGB(d1);
}

DoodadInstance::~DoodadInstance()
{
	delete placementInfo;
}

void DoodadInstance::Draw()
{
	_Pipeline->Push(); // Save world matrix

	// Transforms
	_Pipeline->Translate(placementInfo->position);
	_Pipeline->Quat(placementInfo->orientation.w, -placementInfo->orientation.z, placementInfo->orientation.x, placementInfo->orientation.y);
	_Pipeline->Scale(placementInfo->scale, -placementInfo->scale, -placementInfo->scale);

	// Get actual position
	vec3 pos = vec3(_Pipeline->GetWorld() * vec4(1.0f, 1.0f, 1.0f, 1.0f));
	float radius = modelObject->m_Radius * placementInfo->scale;

	// Distance test
	if (glm::length2(pos - _Camera->Position) > (Settings::doodaddrawdistance2 + radius))
	{
		_Pipeline->Pop(); // restore matrix
		return;
	}

	// Frustrum test
	if (!_Render->frustum.intersectsSphere(pos, radius))
	{
		_Pipeline->Pop(); // restore matrix
		return;
	}

	modelObject->draw();
	PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);

	_Pipeline->Pop();  // restore matrix
}


