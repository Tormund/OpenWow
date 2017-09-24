#ifndef NULL_TECHNIQUE_H
#define	NULL_TECHNIQUE_H

#include "technique.h"

class NullTechnique : public Technique {
public:
	 NullTechnique();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);
};


#endif