#version 330

struct GBuffer
{
	sampler2D gWorldSpacePosMap;     // GL_COLOR_ATTACHMENT0
	sampler2D gNormalMap;		     // GL_COLOR_ATTACHMENT1

	sampler2D gAmbientMap;           // GL_COLOR_ATTACHMENT2
	sampler2D gDiffuseMap;		     // GL_COLOR_ATTACHMENT3
	sampler2D gSpecularMap;		     // GL_COLOR_ATTACHMENT4
	sampler2D gSpecularShininessMap; // GL_COLOR_ATTACHMENT5
};
uniform GBuffer gbuffer;

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
uniform vec2 gScreenSize;
uniform vec3 gEyeWorldPos;

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
			SpecularColor = vec4(Light.specular * texture(gbuffer.gSpecularMap, _pixelXY).xyz * SpecularFactor, 1.0f);
		}
	}

	return vec4(Light.ambient, 1.0f) + DiffuseColor + SpecularColor;
}

vec4 CalcPointLight(vec3 _position, vec3 _normal, vec2 _pixelXY)
{
	vec4 pointLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec3 LightDirection = _position - gPointLight.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	//if(Distance < gPointLight.Radius) {
	vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, _position, _normal, _pixelXY);

	float Attenuation =
		gPointLight.Atten.Constant +
		gPointLight.Atten.Linear * Distance +
		gPointLight.Atten.Quadratic * Distance * Distance;

	Attenuation = max(1.0, Attenuation);

	pointLightColor = Color / Attenuation;
	//}

	return pointLightColor;
}

// -----------------------------------------------------------------------------------------------



void main(void)
{
	vec2 pixelXY = gl_FragCoord.xy / gScreenSize;
	vec3 WorldPos = texture(gbuffer.gWorldSpacePosMap, pixelXY).xyz;
	vec4 Color = texture(gbuffer.gDiffuseMap, pixelXY);
	vec3 Normal = normalize(texture(gbuffer.gNormalMap, pixelXY).xyz);

	FragColor = Color * CalcPointLight(WorldPos, Normal, pixelXY);
}
