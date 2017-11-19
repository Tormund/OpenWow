#pragma once

#include "technique.h"
#include "../lights_common.h"

class POST_Simple : public Technique
{
public:
	POST_Simple() : Technique("shaders/Common_SimpleVertex.vs", "shaders/POST_Simple.fs") {}

    void SetCameraPos(cvec3 _cameraPos)
    {
        setVec3("gCameraPosition", _cameraPos);
    }

	 void SetScreenSize(unsigned int Width, unsigned int Height)
	 {
		 setVec2("gScreenSize", (float)Width, (float)Height);
	 }

     void SetAmbientColor(cvec3 _ambientColor)
     {
         setVec3("gAmbientColor", _ambientColor);
     }

     void SetAmbientIntensitive(float _ambientIntensitive)
     {
         setFloat("gAmbientIntensitive", _ambientIntensitive);
     }
};