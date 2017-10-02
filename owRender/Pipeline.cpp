#include "stdafx.h"

// General
#include "Pipeline.h"
#include "TechniquesManager.h"

void Pipeline::SetProjection(float _fov, float _aspectRatio, float _near, float _far)
{
	projection = glm::perspective(_fov, _aspectRatio, _near, _far);

	float tang = tanf(DEG_TO_RAD * _fov * 0.5f);

	nh = _near * tang;
	nw = nh * _aspectRatio;

	fh = _far  * tang;
	fw = fh * _aspectRatio;

	nearDist = _near;
	farDist = _far;
}

void Pipeline::SetProjection(cmat4 _projectionMatrix)
{
	projection = _projectionMatrix;
}

//

void Pipeline::SetCamera(Camera* _camera)
{
	assert1(_camera != nullptr);

	view = mat4();
	camera = _camera;
}

void Pipeline::SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up)
{
	vec3 CameraRight = glm::normalize(glm::cross(_rotation, _up));
	vec3 CameraUp = glm::normalize(glm::cross(CameraRight, _rotation));

	view = glm::lookAt(_position, _position + _rotation, CameraUp);
	camera = nullptr;
}

void Pipeline::RenderCamera(Camera* _camera)
{
	Camera* activeCamera = _camera;

	if (activeCamera == nullptr)
	{
		activeCamera = camera;
	}

	vec3 nc = activeCamera->Position + activeCamera->Direction * nearDist;
	vec3 fc = activeCamera->Position + activeCamera->Direction * farDist;

	// compute the 4 corners of the frustum on the near plane
	vec3 ntl = nc + activeCamera->CameraUp * nh - activeCamera->CameraRight * nw;
	vec3 ntr = nc + activeCamera->CameraUp * nh + activeCamera->CameraRight * nw;
	vec3 nbl = nc - activeCamera->CameraUp * nh - activeCamera->CameraRight * nw;
	vec3 nbr = nc - activeCamera->CameraUp * nh + activeCamera->CameraRight * nw;

	// compute the 4 corners of the frustum on the far plane
	vec3 ftl = fc + activeCamera->CameraUp * fh - activeCamera->CameraRight * fw;
	vec3 ftr = fc + activeCamera->CameraUp * fh + activeCamera->CameraRight * fw;
	vec3 fbl = fc - activeCamera->CameraUp * fh - activeCamera->CameraRight * fw;
	vec3 fbr = fc - activeCamera->CameraUp * fh + activeCamera->CameraRight * fw;

	_TechniquesMgr->m_Debug_GeometryPass->Bind();
	_Pipeline->Clear();
	_TechniquesMgr->m_Debug_GeometryPass->SetPVW();
	_TechniquesMgr->m_Debug_GeometryPass->SetColor(vec3(1.0f, 1.0f, 1.0f));

	vector<vec3> verts;

	// Near
	verts.push_back(ntl);
	verts.push_back(ntr);
	verts.push_back(ntr);
	verts.push_back(nbr);
	verts.push_back(nbr);
	verts.push_back(nbl);
	verts.push_back(nbl);
	verts.push_back(ntl);

	// Lines
	verts.push_back(ntl);
	verts.push_back(ftl);
	verts.push_back(ntr);
	verts.push_back(ftr);
	verts.push_back(nbr);
	verts.push_back(fbr);
	verts.push_back(nbl);
	verts.push_back(fbl);

	// Far
	verts.push_back(ftl);
	verts.push_back(ftr);
	verts.push_back(ftr);
	verts.push_back(fbr);
	verts.push_back(fbr);
	verts.push_back(fbl);
	verts.push_back(fbl);
	verts.push_back(ftl);


	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

	// Vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINES, 0, verts.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &buffer);

	_TechniquesMgr->m_Debug_GeometryPass->Unbind();
}
