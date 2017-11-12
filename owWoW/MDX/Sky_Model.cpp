#include "../stdafx.h"

// Global
#include "Sky_Model.h"

// Additional
#include "ModelsManager.h"

Sky_Model::Sky_Model(cstring _filename)
{
	m_MDX = _ModelsMgr->Add(_filename);

	if (!m_MDX->IsLoaded())
	{
		_ModelsMgr->Delete(_filename);
		m_MDX = nullptr;
	}
}

bool Sky_Model::Draw()
{
	if (m_MDX == nullptr)
	{
		return false;
	}

	_Pipeline->Clear();
	_Pipeline->Translate(_Camera->Position);
	_Pipeline->Scale(2.0f);

	m_MDX->Render();

	return true;
}