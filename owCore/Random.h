#pragma once

class Random {
public:
	static OW_CORE_DLL_API void SetSeed(unsigned long TSeed);

	static OW_CORE_DLL_API float Generate();
	static OW_CORE_DLL_API int GenerateMax(int TMax);
	static OW_CORE_DLL_API uint32_t GenerateMax(uint32_t TMax);
	static OW_CORE_DLL_API int GenerateRange(int TMin, int TMax);
	static OW_CORE_DLL_API uint32_t GenerateRange(uint32_t TMin, uint32_t TMax);
	static OW_CORE_DLL_API float GenerateRange(float TMin, float TMax);

private:
	static unsigned long GenerateRandomInteger();

private:
	static const unsigned long N = 624;
	static const unsigned long M = 397;
	static const unsigned long MATRIX_A = 0x9908b0dfUL;
	static const unsigned long UPPER_MASK = 0x80000000UL;
	static const unsigned long LOWER_MASK = 0x7fffffffUL;

	static unsigned long rStateVector[N];
	static unsigned long rStateVectorIndex;
};
