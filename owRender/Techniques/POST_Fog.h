#pragma once

#include "technique.h"

class POST_Fog : public Technique
{
public:
    POST_Fog() : Technique("shaders/Common_SimpleVertex.vs", "shaders/POST_Fog.fs") {}

    void SetCameraPos(cvec3 _cameraPos)
    {
        setVec3("gCameraPosition", _cameraPos);
    }

	 void SetScreenSize(unsigned int Width, unsigned int Height)
	 {
		 setVec2("gScreenSize", (float)Width, (float)Height);
	 }



     void SetFogDistance(float _distance)
     {
         setFloat("gFogDistance", _distance);
     }

     void SetFogModifier(float _modifier)
     {
         setFloat("gFogModifier", _modifier);
     }

     void SetFogColor(cvec3 _color)
     {
         setVec3("gFogColor", _color);
     }
};