#pragma once

struct SceneNodeTypes
{
	enum List
	{
		Undefined = 0,
		WMO_Object,
		MDX_OBject
	};
};

class SceneNode
{
public:
	SceneNode() : m_Parent(nullptr) {}
	SceneNode(SceneNode* _parent) : m_Parent(_parent) {}

	const Matrix4f& getRelTrans() { return m_RelTransform; }
	const Matrix4f& getAbsTrans() { return m_AbsTransform; }

protected:
	SceneNode*      m_Parent;

	Vec3f           m_Translate;
	Vec3f           m_Rotate;
	Vec3f           m_Scale;

	Matrix4f        m_RelTransform;
	Matrix4f        m_AbsTransform;

	BoundingBox     m_Bounds;
};