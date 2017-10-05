#pragma once

struct MDX_Part_Transparency
{
public:
	void init(File& f, M2TextureWeight& mtd, uint32* global);

public:
	Animated<float, short, ShortToFloat> trans;
};