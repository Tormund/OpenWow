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

	

	//uint32_t d1;
	//f.ReadBytes(&d1, 4); // (B,G,R,A) Lightning-color. 
	//lcol = fromARGB(d1);
}

DoodadInstance::~DoodadInstance()
{
	delete placementInfo;
}

void DoodadInstance::Draw(cvec3 ofs, float roll)
{
	vec3 tpos(ofs + placementInfo->position);
	rotate(ofs.x, ofs.z, &tpos.x, &tpos.z, roll);

	/*if (glm::length2(tpos - _Camera->Position) > (_Settings->doodaddrawdistance2 + modelObject->rad * placementInfo->scale))
	{
		return;
	}*/

	/*if (!_Render->frustum.intersectsSphere(tpos, modelObject->rad * placementInfo->scale))
	{
		return;
	}*/

	/*glPushMatrix();
	{
		glTranslatef(placementInfo->position.x, placementInfo->position.y, placementInfo->position.z);

		vec3 vdir(-placementInfo->orientation.z, placementInfo->orientation.x, placementInfo->orientation.y);
		glQuaternionRotate2(vdir, placementInfo->orientation.w);
		
		_Pipeline->SetRotationQuaternion(placementInfo->orientation);

		glScalef(placementInfo->scale, -placementInfo->scale, -placementInfo->scale);*/
		
		_Pipeline->Clear();

		_Pipeline->Translate(ofs);
		_Pipeline->RotateY(roll - PI / 2.0f);

		_Pipeline->Translate(placementInfo->position);

		vec3 vdir(-placementInfo->orientation.z, placementInfo->orientation.x, placementInfo->orientation.y);
		mat4 m = glQuaternionRotate2(vdir, placementInfo->orientation.w);
		_Pipeline->Mult(m);

		_Pipeline->Scale(placementInfo->scale, -placementInfo->scale, -placementInfo->scale);

	
		modelObject->draw();
		_Perfomance->Inc(PERF_WMOs_DOODADS);
}


