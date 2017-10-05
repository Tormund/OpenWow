#include "stdafx.h"

// General
#include "Model_Instance.h"

ModelInstance::ModelInstance(File& f)
{
	placementInfo = new ModelPlacementInfo;
	f.ReadBytes(placementInfo, ModelPlacementInfo::__size);

	// Convert rotation
	placementInfo->rotation = glm::radians(placementInfo->rotation);
	placementInfo->rotation.x = -placementInfo->rotation.x;
	placementInfo->rotation.y = placementInfo->rotation.y - PI / 2.0;

	sc = placementInfo->scale / 1024.0f;
}

ModelInstance::~ModelInstance()
{
	delete placementInfo;
}

void ModelInstance::draw()
{
	float dist = glm::length(placementInfo->position - _Camera->Position);
	if (dist > Settings::modeldrawdistance + modelObject->m_Radius * sc)
	{
		//return;
	}

	if (!_Render->frustum.intersectsSphere(placementInfo->position, modelObject->m_Radius * sc))
	{
		return;
	}

	_Pipeline->Clear();

	_Pipeline->Translate(placementInfo->position.x, placementInfo->position.y, placementInfo->position.z);
	_Pipeline->RotateX(placementInfo->rotation.z);
	_Pipeline->RotateY(placementInfo->rotation.y);
	_Pipeline->RotateZ(placementInfo->rotation.x);
	_Pipeline->Scale(sc);

	modelObject->draw();

	PERF_INC(PERF_MAP_MODELS_MDXs);
}
