#pragma once

#include "RefManager.h"

template <class OBJECT_TYPE>
class IntManager : public RefManager<OBJECT_TYPE, uint32_t>
{
public:
	IntManager() : baseid(1) {}

protected:
	uint32_t GenerateID() override
	{
		baseid += 1;
		return baseid;
	}

private:
	uint32_t baseid;
};