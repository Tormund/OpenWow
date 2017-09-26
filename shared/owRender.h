#pragma once

// Common includes
#include "../owRender/Common.h"

// Render
#include "../owRender/Camera.h"
#include "../owRender/FontsMgr.h"
#include "../owRender/GLFWBackend.h"
#include "../owRender/Pipeline.h"
#include "../owRender/Render.h"
#include "../owRender/TexturesMgr.h"
#include "../owRender/ShadowMapFBO.h"

// Techiques
#include "../owRender/TechniquesManager.h"

#include "../owRender/Techniques/Technique.h"
#include "../owRender/Techniques/ds_dir_light_pass_tech.h"
#include "../owRender/Techniques/ds_light_pass_tech.h"
#include "../owRender/Techniques/ds_point_light_pass_tech.h"
#include "../owRender/Techniques/null_technique.h"
#include "../owRender/Techniques/SimpleColorTechnique.h"

//
#include "../owRender/frustum.h"
#include "../owRender/Gbuffer.h"