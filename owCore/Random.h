#pragma once

class Random {
public:
	static  void SetSeed(unsigned long TSeed);

	static  float Generate();
	static  int GenerateMax(int TMax);
	static  uint32_t GenerateMax(uint32_t TMax);
	static  int GenerateRange(int TMin, int TMax);
	static  uint32_t GenerateRange(uint32_t TMin, uint32_t TMax);
	static  float GenerateRange(float TMin, float TMax);

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
