#pragma once

class IWoWObject
{
public:
    enum WowObjectType
    {
        OBJECT_MAP,
        OBJECT_MAPTILE,
        OBJECT_MAPCHUNK,
        OBJECT_WMO,
        OBJECT_MDX
    };

public:


private:
    WowObjectType m_Type;
    bool m_Visible;
    IWoWObject* m_Parent;
};