#pragma once

#include "MDX_Headers.h"

struct MDX_Part_Transparency
{
public:
	void init(File& f, M2TextureWeight& mtd, uint32* global);

private:
	Animated<float, short, ShortToFloat> trans;

	//

	friend class MDX;
	friend class ModelRenderPass;
};