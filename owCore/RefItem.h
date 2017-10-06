#pragma once

class RefItem
{
public:
	RefItem() : m_RefsCount(0)	{}
	virtual ~RefItem() {}

	void AddRef();
	void DelRef();

	uint32 GetRefsCount() const { return m_RefsCount; }
	bool NeedDelete() const { return (m_RefsCount <= 0); }

protected:
	uint32 m_RefsCount;
};