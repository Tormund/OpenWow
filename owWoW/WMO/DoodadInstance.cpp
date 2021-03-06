#include "../stdafx.h"

// General
#include "DoodadInstance.h"

DoodadInstance::DoodadInstance(File& f)
{
	placementInfo = new DoodadPlacementInfo;
	f.ReadBytes(placementInfo, DoodadPlacementInfo::__size);

	CalculateMatrix();
}

DoodadInstance::~DoodadInstance()
{
	delete placementInfo;
}

void DoodadInstance::SetModel(MDX* _model)
{
	assert1(_model != nullptr);

	modelObject = _model;
	m_Bounds = modelObject->m_Bounds;
	m_Bounds.transform(m_AbsTransform);
}

void DoodadInstance::Render()
{
	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	_Pipeline->Push(); // Save world matrix
	{
		_Pipeline->Mult(m_RelTransform);

		// Get actual position
		//vec3 pos = _Pipeline->GetWorld() * vec3(1.0f, 1.0f, 1.0f);
		//float radius = modelObject->m_Bounds.Radius * placementInfo->scale;

		// Distance test
		//if ((pos - _Camera->Position).length2() > (_Config.doodaddrawdistance2 + radius))
		//{
		//	_Pipeline->Pop(); // restore matrix
		//	return;
		//}

		// Frustrum test
		if (_CameraFrustum->_frustum.cullBox(aabb))
		{
			_Pipeline->Pop(); // restore matrix
			return;
		}

		modelObject->Render();
		PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
	}
	_Pipeline->Pop();  // restore matrix
}

void DoodadInstance::CalculateMatrix()
{
	// Convert
	placementInfo->position = From_XYZ_To_XZminusY_RET(placementInfo->position);

	// Build relative matrix
	m_RelTransform.translate(placementInfo->position);
	m_RelTransform *= Quaternion(-placementInfo->orientation.z, placementInfo->orientation.x, placementInfo->orientation.y, placementInfo->orientation.w);
	m_RelTransform.scale(placementInfo->scale, -placementInfo->scale, -placementInfo->scale);

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}
}


