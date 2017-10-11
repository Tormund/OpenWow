#pragma once

#include "SceneNode.h"

class RenderableItem
{
public:
	RenderableItem() {}
	RenderableItem(uint32 type, float sortKey, SceneNode* node)
		: node(node), type(type), sortKey(sortKey)
	{}

private:
	SceneNode* node;
	int        type;  // Type is stored explicitly for better cache efficiency when iterating over list
	float      sortKey;
};

typedef std::vector< RenderableItem > RenderQueue;