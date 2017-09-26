#include "stdafx.h"

// General
#include "Model_Instance.h"

ModelInstance::ModelInstance(Model* _modelObject, File& f) : modelObject(_modelObject)
{
	placementInfo = new ModelPlacementInfo;
	f.ReadBytes(placementInfo, ModelPlacementInfo::__size);

	sc = placementInfo->scale / 1024.0f;
}

ModelInstance::~ModelInstance()
{
	delete placementInfo;
}

void ModelInstance::draw()
{
	float dist = glm::length(placementInfo->position - _Camera->Position) - modelObject->rad;
	if (dist > _Settings->modeldrawdistance)
	{
		return;
	}

	if (!_Render->frustum.intersectsSphere(placementInfo->position, modelObject->rad*sc))
	{
		return;
	}

	//

	glPushMatrix();
	{
		glTranslatef(placementInfo->position.x, placementInfo->position.y, placementInfo->position.z);

		glRotatef(placementInfo->rotation.y - 90.0f, 0, 1, 0);
		glRotatef(-placementInfo->rotation.x, 0, 0, 1);
		glRotatef(placementInfo->rotation.z, 1, 0, 0);

		glScalef(sc, sc, sc);

		modelObject->draw();
	}
	glPopMatrix();
}
