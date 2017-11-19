#pragma once

template<typename T>
inline T Sky::GetByTimeTemplate(vector<SkyParam<T>>* _array, uint32 _paramNum, uint32 _time) const
{
    if (_array[_paramNum].empty())
    {
        return T();
    }

    T parBegin, parEnd;
    uint32 timeBegin, timeEnd;
    size_t last = _array[_paramNum].size() - 1;

    if (_time < _array[_paramNum][0].time)
    {
        // interpolate between last and first
        parBegin = _array[_paramNum][last].param;
        timeBegin = _array[_paramNum][last].time;
        parEnd = _array[_paramNum][0].param;
        timeEnd = _array[_paramNum][0].time + (2 * 60 * 24); // next day
        _time += (2 * 60 * 24);
    }
    else
    {
        for (uint32 i = last; i >= 0; i--)
        {
            if (_time >= _array[_paramNum][i].time)
            {
                parBegin = _array[_paramNum][i].param;
                timeBegin = _array[_paramNum][i].time;

                if (i == last) // if current is last, then interpolate with first
                {
                    parEnd = _array[_paramNum][0].param;
                    timeEnd = _array[_paramNum][0].time + (2 * 60 * 24);
                }
                else
                {
                    parEnd = _array[_paramNum][i + 1].param;
                    timeEnd = _array[_paramNum][i + 1].time;
                }
                break;
            }
        }
    }

    float tt = (float)(_time - timeBegin) / (float)(timeEnd - timeBegin);
    return parBegin * (1.0f - tt) + (parEnd * tt);
}
