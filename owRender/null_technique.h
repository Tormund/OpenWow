#ifndef NULL_TECHNIQUE_H
#define	NULL_TECHNIQUE_H

#include "technique.h"

class NullTechnique : public Technique {
public:
	OW_RENDER_DLL_API NullTechnique();
	OW_RENDER_DLL_API virtual bool Init();

	OW_RENDER_DLL_API void SetWVP(cmat4 WVP);
};


#endif