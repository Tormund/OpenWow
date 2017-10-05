#pragma once

#include "AnimationTypes.h"
#include "File.h"
#include "TimeManager.h"

template<class T>
inline T interpolate(const float r, const T& v1, const T& v2)
{
	return v1 * (1.0f - r) + v2 * r;
}

template<class T>
inline T interpolateHermite(const float r, const T& v1, const T& v2, const T& in, const T &out)
{
	// basis functions
	float h1 = 2.0f*r*r*r - 3.0f*r*r + 1.0f;
	float h2 = -2.0f*r*r*r + 3.0f*r*r;
	float h3 = r*r*r - 2.0f*r*r + r;
	float h4 = r*r*r - r*r;

	// interpolation
	return static_cast<T>(v1 * h1 + v2 * h2 + in * h3 + out * h4);
}

template<class T>
inline T interpolateBezier(const float r, const T &v1, const T &v2, const T &in, const T &out)
{
	float InverseFactor = (1.0f - r);
	float FactorTimesTwo = r*r;
	float InverseFactorTimesTwo = InverseFactor*InverseFactor;

	// basis functions
	float h1 = InverseFactorTimesTwo * InverseFactor;
	float h2 = 3.0f * r * InverseFactorTimesTwo;
	float h3 = 3.0f * FactorTimesTwo * InverseFactor;
	float h4 = FactorTimesTwo * r;

	// interpolation
	return static_cast<T>(v1*h1 + v2*h2 + in*h3 + out*h4);
}

template<>
inline quat interpolate<quat>(const float r, const quat& v1, const quat& v2)
{
	return glm::slerp(v1, v2, r); //Quaternion::slerp(r, v1, v2);
}

//

typedef pair<size_t, size_t> AnimRange;

enum Interpolations
{
	INTERPOLATION_NONE,
	INTERPOLATION_LINEAR,
	INTERPOLATION_HERMITE
};

template <class T>
class Identity
{
public:
	static const T& conv(const T& t)
	{
		return t;
	}
};

struct M2CompQuat
{
	__int16 x, y, z, w;
};

class Quat16ToQuat32
{
public:
	static const quat conv(const M2CompQuat t)
	{
		return quat(
			float(t.w > 0 ? t.w - 32767 : t.w + 32767) / 32767.0f,
			float(t.x > 0 ? t.x - 32767 : t.x + 32767) / 32767.0f,
			float(t.y > 0 ? t.y - 32767 : t.y + 32767) / 32767.0f,
			float(t.z > 0 ? t.z - 32767 : t.z + 32767) / 32767.0f);
	}
};

class ShortToFloat
{
public:
	static const float conv(const short t)
	{
		return t / 32767.0f;
	}
};


#define	MAX_ANIMATED (500u)

/*
	Generic animated value class:

	T is the data type to animate
	D is the data type stored in the file (by default this is the same as T)
	Conv is a conversion object that defines T conv(D) to convert from D to T (by default this is an identity function)	(there might be a nicer way to do this? meh meh)
*/
template <class T, class D = T, class Conv = Identity<T> >
class Animated
{
public:
	bool uses(uint32 anim)
	{
		if (global_sequence > -1)
		{
			anim = 0;
		}

		return (data[anim].size() > 0);
	}

	T getValue(int anim, int time)
	{
		// obtain a time value and a data range
		if (global_sequence > -1)
		{
			// TODO
			if (globals[global_sequence] == 0)
			{
				time = 0;
			}
			else
			{
				time = _TimeManager->globalTime % globals[global_sequence];
			}
			anim = 0;
		}

		if (data[anim].size() > 1 && times[anim].size() > 1)
		{
			size_t t1, t2;
			size_t pos = 0;
			int max_time = times[anim][times[anim].size() - 1];
			if (max_time > 0)
			{
				time %= max_time; // I think this might not be necessary?
			}
			for (size_t i = 0; i < times[anim].size() - 1; i++)
			{
				if (time >= times[anim][i] && time < times[anim][i + 1])
				{
					pos = i;
					break;
				}
			}
			t1 = times[anim][pos];
			t2 = times[anim][pos + 1];
			float r = (time - t1) / (float)(t2 - t1);

			if (interpolation_type == INTERPOLATION_LINEAR)
			{
				return interpolate<T>(r, data[anim][pos], data[anim][pos + 1]);
			}
			else if (interpolation_type == INTERPOLATION_NONE)
			{
				return data[anim][pos];
			}
			else
			{
				// INTERPOLATION_HERMITE is only used in cameras afaik?
				return interpolateHermite<T>(r, data[anim][pos], data[anim][pos + 1], in[anim][pos], out[anim][pos]);
			}
		}
		else
		{
			// default value
			if (data[anim].size() == 0)
			{
				return T();
			}
			else
			{
				return data[anim][0];
			}
		}
	}

	void init(M2Track<D>& b, File& f, uint32* gs)
	{
		globals = gs;
		interpolation_type = b.interpolation_type;
		global_sequence = b.global_sequence;
		if (global_sequence != -1)
		{
			assert1(gs);
		}

		assert1(b.timestamps.size == b.values.size);
		sizes = b.timestamps.size;
		if (b.timestamps.size == 0)	return;

		// times
		for (size_t j = 0; j < b.timestamps.size; j++)
		{
			M2Array<uint32>* pHeadTimes = (M2Array<uint32>*)(f.GetData() + b.timestamps.offset + j * sizeof(M2Array<uint32>));

			uint32* ptimes = (uint32*)(f.GetData() + pHeadTimes->offset);
			for (size_t i = 0; i < pHeadTimes->size; i++)
			{
				times[j].push_back(ptimes[i]);
			}
		}

		// keyframes
		for (size_t j = 0; j < b.values.size; j++)
		{
			M2Array<D>* pHeadKeys = (M2Array<D>*)(f.GetData() + b.values.offset + j * sizeof(M2Array<D>));

			D* keys = (D*)(f.GetData() + pHeadKeys->offset);
			switch (interpolation_type)
			{
				case INTERPOLATION_NONE:
				case INTERPOLATION_LINEAR:
				for (size_t i = 0; i < pHeadKeys->size; i++)
				{
					data[j].push_back(Conv::conv(keys[i]));
				}
				break;

				case INTERPOLATION_HERMITE:
				for (size_t i = 0; i < pHeadKeys->size; i++)
				{
					data[j].push_back(Conv::conv(keys[i * 3 + 0]));
					in[j].push_back(  Conv::conv(keys[i * 3 + 1]));
					out[j].push_back( Conv::conv(keys[i * 3 + 2]));
				}
				break;
			}
		}
	}

	void init(M2Track<D>& b, File& f, uint32* gs, File* animfiles)
	{
		globals = gs;
		interpolation_type = b.interpolation_type;
		global_sequence = b.global_sequence;
		if (global_sequence != -1)
		{
			assert1(gs);
		}

		assert1(b.timestamps.size == b.values.size);
		sizes = b.timestamps.size;
		if (b.timestamps.size == 0)	return;

		// times
		for (size_t j = 0; j < b.timestamps.size; j++)
		{
			M2Array<uint32>* pHeadTimes = (M2Array<uint32>*)(f.GetData() + b.timestamps.offset + j * sizeof(M2Array<uint32>));
			
			uint32* ptimes;
			if (animfiles[j].GetSize() > 0)
				ptimes = (uint32*)(animfiles[j].GetData() + pHeadTimes->offset);
			else
				ptimes = (uint32*)(f.GetData() + pHeadTimes->offset);

			for (size_t i = 0; i < pHeadTimes->size; i++)
			{
				times[j].push_back(ptimes[i]);
			}
		}

		// keyframes
		for (size_t j = 0; j < b.values.size; j++)
		{
			M2Array<D>* pHeadKeys = (M2Array<D>*)(f.GetData() + b.values.offset + j * sizeof(M2Array<D>));
			assert1((D*)(f.GetData() + pHeadKeys->offset));

			D *keys;
			if (animfiles[j].GetSize() > 0)
				keys = (D*)(animfiles[j].GetData() + pHeadKeys->offset);
			else
				keys = (D*)(f.GetData() + pHeadKeys->offset);

			switch (interpolation_type)
			{
				case INTERPOLATION_NONE:
				case INTERPOLATION_LINEAR:
				for (size_t i = 0; i < pHeadKeys->size; i++)
					data[j].push_back(Conv::conv(keys[i]));
				break;

				case INTERPOLATION_HERMITE:
				for (size_t i = 0; i < pHeadKeys->size; i++)
				{
					data[j].push_back(Conv::conv(keys[i * 3 + 0]));
					in[j].push_back(  Conv::conv(keys[i * 3 + 1]));
					out[j].push_back( Conv::conv(keys[i * 3 + 2]));
				}
				break;
			}
		}
	}

	void fix(T fixfunc(const T&))
	{
		switch (interpolation_type)
		{
			case INTERPOLATION_NONE:
			case INTERPOLATION_LINEAR:
			for (size_t i = 0; i < sizes; i++)
			{
				for (size_t j = 0; j < data[i].size(); j++)
				{
					data[i][j] = fixfunc(data[i][j]);
				}
			}
			break;

			case INTERPOLATION_HERMITE:
			for (size_t i = 0; i < sizes; i++)
			{
				for (size_t j = 0; j < data[i].size(); j++)
				{
					data[i][j] = fixfunc(data[i][j]);
					in[i][j] = fixfunc(in[i][j]);
					out[i][j] = fixfunc(out[i][j]);
				}
			}
			break;
		}
	}

private:
	int32 interpolation_type;
	int32 global_sequence;
	uint32* globals;

	size_t sizes;
	vector<int32> times[MAX_ANIMATED];
	vector<T> data[MAX_ANIMATED];

	// Hermite interpolation
	vector<T> in[MAX_ANIMATED];
	vector<T> out[MAX_ANIMATED];
};