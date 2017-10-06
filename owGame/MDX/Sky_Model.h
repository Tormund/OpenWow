#pragma once

#include "MDX.h"

class Sky_Model
{
public:
	Sky_Model(cstring _filename);

	bool Draw();

private:
	MDX* m_MDX;
};