#pragma once

class RenderConfig
{
public:
    CLASS_INSTANCE2(RenderConfig);

    RenderConfig();
	~RenderConfig();

public:
    inline void Switch(bool& var)
    {
        var = !var;
    }

	inline void CalculateAspectFactor()
	{
		aspectFactor = 1.0f;
		aspectRatio = windowSizeX / windowSizeY;
		if (aspectRatio >= 1.0f)
		{
			aspectFactor = aspectRatio;
		}
	}

	//

	template<typename T>
	inline void SetWindowSize(T _x, T _y)
	{
		windowSizeX = static_cast<float>(_x);
		windowSizeY = static_cast<float>(_y);
		CalculateAspectFactor();
	}

	inline vec2 GetWindowSize()
	{
		return vec2(windowSizeX, windowSizeY);
	}

	//

public:
	// Render settings
	float windowSizeX;
	float windowSizeY;
	float aspectRatio;
	float aspectFactor;

    //----------------------------------------------------
    //                     Distances                    --
    //----------------------------------------------------
    struct
    {
        float mapdrawdistance;
        float modeldrawdistance;
        float doodaddrawdistance;
        float highresdistance;
        float culldistance;
        float fogdistance;

        float mapdrawdistance2;
        float modeldrawdistance2;
        float doodaddrawdistance2;
        float highresdistance2;
        float culldistance2;

        void Update()
        {
            highresdistance2 = highresdistance * highresdistance;
            mapdrawdistance2 = mapdrawdistance * mapdrawdistance;
            modeldrawdistance2 = modeldrawdistance * modeldrawdistance;
            doodaddrawdistance2 = doodaddrawdistance * doodaddrawdistance;
            culldistance2 = culldistance * culldistance;
        }
    } Distances;

    //----------------------------------------------------
    //                   Render OpenGL                  --
    //----------------------------------------------------

    struct
    {
        bool EXT_texture_filter_anisotropic = false;
        bool EXT_texture_compression_s3tc = false;
        bool EXT_texture_sRGB = false;

        bool OES_EGL_image = false;

        int	majorVersion = 1;
        int	minorVersion = 0;
    } OpenGL;

    //----------------------------------------------------
    //                    Render Caps                   --
    //----------------------------------------------------

    struct 
    {
        uint16	maxJointCount;
        uint16	maxTexUnitCount;
        bool	texFloat;
        bool	texNPOT;
        bool	rtMultisampling;
        bool	geometryShaders;
        bool	tesselation;
        bool	computeShaders;
        bool	instancing;
    } DeviceCaps;

    //----------------------------------------------------
    //                      Quality                     --
    //----------------------------------------------------

    struct
    {
        bool Terrain_LowDetail;
        bool Terrain_MCCV;
        bool Terrain_MCLV;

        uint16 Texture_Sampler;
    } Quality;

	// Drawing

	bool draw_map_chunk;
	bool draw_map_wmo;
	bool draw_map_wmo_doodads;
	bool draw_map_mdx;
    bool draw_water;

	bool drawfog;
    bool timeEnable;
};

#define _Config RenderConfig::instance()