#pragma once

class RefItem
{
public:
	RefItem();
	inline virtual ~RefItem() {}

	virtual void AddRef();
	virtual void DelRef();

	inline uint32_t GetRefsCount() const { return refcount; }
	inline bool NeedDelete() const { return (GetRefsCount() <= 0); }

private:
	uint32_t refcount;
};