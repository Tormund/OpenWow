
/*vec4 fromRGBA(vec4 colorRGBA)
{
	//float r = float((colorRGBA & 0xFF000000u) >> 24) / 255.0f;
	//float g = float((colorRGBA & 0x00FF0000u) >> 16) / 255.0f;
	//float b = float((colorRGBA & 0x0000FF00u) >> 8 ) / 255.0f;	
	//float a = float((colorRGBA & 0x000000FFu)      ) / 255.0f;
	
	return vec4(r, g, b, a);
}*/

vec4 fromBGRA(vec4 colorBGRA)
{
	//float r = float(uint((colorBGRA & 0x0000FF00u) >> 8 )) / 255.0f;
	//float g = float(uint((colorBGRA & 0x00FF0000u) >> 16)) / 255.0f;
	//float b = float(uint((colorBGRA & 0xFF000000u) >> 24)) / 255.0f;
	//float a = float(uint((colorBGRA & 0x000000FFu)      )) / 255.0f;
	
	return vec4(colorBGRA.z, colorBGRA.y, colorBGRA.x, colorBGRA.w) / 127.0;
}