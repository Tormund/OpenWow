#version 330
#include "lib/fragDeferredRead.glsl"

struct BaseLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight
{
	BaseLight Base;
	vec3 Direction;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Quadratic;
};

struct PointLight
{
	BaseLight Base;
	vec3 Position;
	Attenuation Atten;
	float Radius;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Cutoff;
};

// Light type
uniform int gLightType;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLight;
uniform SpotLight gSpotLight;

// Specular
uniform float gSpecularPower; // Shrinesses

// Screen
uniform vec3 gEyeWorldPos;
uniform vec2 gScreenSize;

// OUT
out vec4 FragColor;

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 _position, vec3 _normal, vec2 _pixelXY)
{
	float DiffuseFactor = max(dot(_normal, -LightDirection), 0.0f);

	vec4 DiffuseColor = vec4(Light.diffuse * DiffuseFactor, 0.0f);
	vec4 SpecularColor = vec4(0, 0, 0, 0.0);

	if (DiffuseFactor > 0)
	{
		vec3 VertexToEye = normalize(gEyeWorldPos - _position);
		vec3 LightReflect = normalize(reflect(-LightDirection, _normal));


		float SpecularFactor = dot(VertexToEye, LightReflect);
		SpecularFactor = pow(SpecularFactor, gSpecularPower);

		if (SpecularFactor > 0)
		{
			SpecularColor = vec4(Light.specular * getSpecParams(_pixelXY).xyz * SpecularFactor, 1.0f);
		}
	}

	return vec4(Light.ambient, 1.0f) + DiffuseColor + SpecularColor;
}

vec4 CalcDirectionalLight(vec3 _position, vec3 _normal, vec2 _pixelXY)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, _position, _normal, _pixelXY);
}

// -----------------------------------------------------------------------------------------------

void main(void)
{
	vec2 pixelXY = gl_FragCoord.xy / gScreenSize;
	
	vec3 WorldPos = getPos(pixelXY);
	vec3 Normal = getNormal(pixelXY);
	vec3 Color = getAlbedo(pixelXY);

	if(getMatID(pixelXY) > 0.0)
	{
		FragColor = vec4(Color, 1.0)  * vec4(CalcDirectionalLight(WorldPos, Normal, pixelXY).xyz, 1.0f);
	}
	else
	{
		FragColor = vec4(Color, 1.0);
	}
}
