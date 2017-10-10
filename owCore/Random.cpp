#include "stdafx.h"

// General
#include "Random.h"

unsigned long Random::rStateVector[Random::N];
unsigned long Random::rStateVectorIndex;

// Set random seed
void Random::SetSeed(unsigned long TSeed)
{
	rStateVector[0] = TSeed & 0xffffffffUL;
	for (rStateVectorIndex = 1; rStateVectorIndex < N; ++rStateVectorIndex)
	{
		rStateVector[rStateVectorIndex] = (1812433253UL * (rStateVector[rStateVectorIndex - 1] ^ (rStateVector[rStateVectorIndex - 1] >> 30)) + rStateVectorIndex);
		rStateVector[rStateVectorIndex] &= 0xffffffffUL;
	}
}

// Generates a random number [0, 1)
float Random::Generate()
{
	return GenerateRange(0.0f, 0.999999f);
}

// Generates a random number [0, TMax]
int Random::GenerateMax(int TMax)
{
	return GenerateRandomInteger() % TMax;
}

// Generates a random number [0, TMax]
uint32 Random::GenerateMax(uint32 TMax)
{
	return GenerateRandomInteger() % TMax;
}

// Generates a random number [TMin, TMax]
int Random::GenerateRange(int TMin, int TMax)
{
	return GenerateRandomInteger() % (TMax - TMin + 1) + TMin;
}

// Generates a random number [TMin, TMax]
uint32 Random::GenerateRange(uint32 TMin, uint32 TMax)
{
	return GenerateRandomInteger() % (TMax - TMin + 1) + TMin;
}

// Generates a random number [TMin, TMax]
float Random::GenerateRange(float TMin, float TMax)
{
	return GenerateRandomInteger() * (1.0f / 4294967295.0f) * (TMax - TMin) + TMin;
}

//

unsigned long Random::GenerateRandomInteger()
{
	static unsigned long mag01[2] = {0x0UL, MATRIX_A};
	unsigned long y;

	if (rStateVectorIndex >= N)
	{
		uint32 kk;

		for (kk = 0; kk < N - M; ++kk)
		{
			y = (rStateVector[kk] & UPPER_MASK) | (rStateVector[kk + 1] & LOWER_MASK);
			rStateVector[kk] = rStateVector[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		for (; kk < N - 1; ++kk)
		{
			y = (rStateVector[kk] & UPPER_MASK) | (rStateVector[kk + 1] & LOWER_MASK);
			rStateVector[kk] = rStateVector[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		y = (rStateVector[N - 1] & UPPER_MASK) | (rStateVector[0] & LOWER_MASK);
		rStateVector[N - 1] = rStateVector[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		rStateVectorIndex = 0;
	}

	y = rStateVector[rStateVectorIndex++];

	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}
