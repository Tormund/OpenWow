#pragma once

struct BaseLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	BaseLight()
	{
		ambient = vec3(0.0f, 0.0f, 0.0f);
		diffuse = vec3(0.0f, 0.0f, 0.0f);
		specular = vec3(0.0f, 0.0f, 0.0f);
	}
};

struct DirectionalLight : public BaseLight
{
	vec3 Direction;

	DirectionalLight()
	{
		Direction = vec3(0.0f, 0.0f, 0.0f);
	}
};

struct PointLight : public BaseLight
{
	vec3 Position;
	float Radius;

	struct
	{
		float Constant;
		float Linear;
		float Quadratic;
	} Attenuation;

	PointLight()
	{
		Position = vec3(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 0.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Quadratic = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = vec3(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
};

#define COLOR_WHITE1 vec3(1.0f, 1.0f, 1.0f)
#define COLOR_RED1 vec3(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN1 vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN1 vec3(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE1 vec3(0.0f, 0.0f, 1.0f)