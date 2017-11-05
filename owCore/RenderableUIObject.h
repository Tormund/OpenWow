#pragma once

class RenderableUIObject
{
public:
	virtual void RenderUI() = 0;
    
    //

	void SetDrawOrder(uint32 _order)
	{
		m_DrawOrder = _order;
	}
	uint32 GetDrawOrder() const
	{
		return m_DrawOrder;
	}

protected:
	bool Register(uint32 _order = 0);
	void Unregister();

private:
	uint32 m_DrawOrder;
};