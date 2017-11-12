#pragma once

// 

class IWoWTemplate
{
public:
    virtual void Render() = 0;

private:
    mat4 m_TransformRel;
};