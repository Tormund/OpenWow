#pragma once

class GPUTimer
{
public:
	GPUTimer();
	~GPUTimer();

	void beginQuery(uint32 frameID);
	void endQuery();
	bool updateResults();

	void reset();
	float getTimeMS() const { return _time; }

private:
	std::vector < uint32 >  _queryPool;
	uint32                  _numQueries;
	uint32                  _queryFrame;
	float                   _time;
	bool                    _activeQuery;
};