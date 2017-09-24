#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include "technique.h"

class DSGeomPassTech : public Technique {
public:
	 DSGeomPassTech();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);
	 void SetWorldMatrix(cmat4 WVP);
	 void SetColorTextureUnit(int TextureUnit);
	 void SetSpecularTextureUnit(int TextureUnit);
};


#endif