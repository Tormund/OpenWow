#pragma once

template<class MODULE>
inline MODULE * ModulesMgr::GetModule()
{
	for (auto it = modules.begin(); it != modules.end(); ++it)
	{
		MODULE * ptr = nullptr;
		if ((ptr = dynamic_cast<MODULE*>(*it)))
		{
			return ptr;
		}
	}

	assert3(false, "Error getting module.", typeid(MODULE).name());
	return nullptr;
}