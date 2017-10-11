#pragma once

struct SceneNodeTypes
{
	enum List
	{
		Undefined = 0,
		MapChunk,
		WMO,
		WMO_Doodad,
		MDX,
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
	uint8               m_Type;
	SceneNode*          m_Parent;
	vector<SceneNode*>  m_Childs;

	Vec3f               m_Translate;
	Vec3f               m_Rotate;
	Vec3f               m_Scale;

	Matrix4f            m_RelTransform;
	Matrix4f            m_AbsTransform;

	BoundingBox         m_Bounds;
};