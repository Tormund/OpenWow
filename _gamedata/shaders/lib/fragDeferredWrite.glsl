layout( location = 0 ) out vec4 fragData0;
layout( location = 1 ) out vec4 fragData1;
layout( location = 2 ) out vec4 fragData2;
layout( location = 3 ) out vec4 fragData3;

void setMatID( const float id ) { fragData0.a = id; }
void setPos( const vec3 pos ) { fragData0.rgb = pos; }
void setNormal( const vec3 normal ) { fragData1.rgb = normal; fragData1.a = 1.0; }
void setAlbedo( const vec3 albedo ) { fragData2.rgb = albedo; fragData2.a = 1.0; }
void setAlbedo4( const vec4 albedo ) { fragData2 = albedo; }
void setSpecParams( const vec3 specCol, const float gloss ) { fragData3.rgb = specCol; fragData3.a = gloss; }
