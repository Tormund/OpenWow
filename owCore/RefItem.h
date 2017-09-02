#pragma once

class RefItem
{
public:
	OW_CORE_DLL_API RefItem();
	inline virtual ~RefItem() {}

	OW_CORE_DLL_API virtual void AddRef();
	OW_CORE_DLL_API virtual void DelRef();

	inline uint32_t GetRefsCount() const { return refcount; }
	inline bool NeedDelete() const { return (GetRefsCount() <= 0); }

private:
	uint32_t refcount;
};