#pragma once

#include "MDX_Headers.h"

class MDX_Part_Color
{
public:
	void init(File& f, M2Color& mcd, uint32* global);

private:
	Animated<vec3> color;
	Animated<float, short, ShortToFloat> opacity;

	//

	friend class MDX;
	friend class ModelRenderPass;
};