#pragma once

template <typename T, typename P>
class ContainerAssync : public map<T, P>
{
public:
	void Init()
	{
		InitializeCriticalSection(&cs);
	}

	void Destroy()
	{
		DeleteCriticalSection(&cs);
	}

	void add(T element, P object)
	{
		EnterCriticalSection(&cs); // THREAD

		map<T, P>::insert(make_pair(element, object));

		LeaveCriticalSection(&cs); // THREAD
	}

	bool get(T _index, P * _element)
	{
		EnterCriticalSection(&cs); // THREAD

		auto it = map<T, P>::find(_index);
		bool result = (it != map<T, P>::end());
		
		if (result)
		{
			*_element = *it;
		}

		LeaveCriticalSection(&cs); // THREAD

		return result;
	}

	bool exists(T _index)
	{
		EnterCriticalSection(&cs); // THREAD

		bool result = (map<T, P>::find(_index) != map<T, P>::end());

		LeaveCriticalSection(&cs); // THREAD

		return result;
	}

	bool pop_front(T * _index, P * _element)
	{
		EnterCriticalSection(&cs); // THREAD

		auto it = map<T, P>::begin();
		bool result = (it != map<T, P>::end());

		if (result)
		{
			*_index = it->first;
			*_element = it->second;

			map<T, P>::erase(it);
		}

		LeaveCriticalSection(&cs); // THREAD

		return result;
	}

private:
	CRITICAL_SECTION cs;
};