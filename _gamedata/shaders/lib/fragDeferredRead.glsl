uniform sampler2D gbuf0;
uniform sampler2D gbuf1;
uniform sampler2D gbuf2;
uniform sampler2D gbuf3;

float getMatID(const vec2 coord) 
{ 
	return texture(gbuf0, coord).a; 
}

vec3 getPos(const vec2 coord) 
{ 
	return texture(gbuf0, coord).rgb; 
}

vec3 getNormal(const vec2 coord) 
{ 
	return texture(gbuf1, coord).rgb; 
}

vec3 getAlbedo(const vec2 coord) 
{ 
	return texture(gbuf2, coord).rgb; 
}

vec4 getAlbedo4(const vec2 coord) 
{ 
	return texture(gbuf2, coord); 
}

vec4 getSpecParams(const vec2 coord) 
{ 
	return texture(gbuf3, coord).rgba; 
}
