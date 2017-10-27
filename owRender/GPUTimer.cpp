#include "stdafx.h"

// General 
#include "GPUTimer.h"

// Additional
#include "OpenGL.h"

GPUTimer::GPUTimer() : _numQueries(0), _queryFrame(0), _activeQuery(false)
{
	reset();
}

GPUTimer::~GPUTimer()
{
	if (!_queryPool.empty())
	{
		glDeleteQueries((uint32)_queryPool.size(), &_queryPool[0]);
	}
}

void GPUTimer::beginQuery(uint32 frameID)
{
	assert1(!_activeQuery);

	if (_queryFrame != frameID)
	{
		if (!updateResults()) return;

		_queryFrame = frameID;
		_numQueries = 0;
	}

	// Create new query pair if necessary
	uint32 queryObjs[2];
	if (_numQueries++ * 2 == _queryPool.size())
	{
		glGenQueries(2, queryObjs);
		_queryPool.push_back(queryObjs[0]);
		_queryPool.push_back(queryObjs[1]);
	}
	else
	{
		queryObjs[0] = _queryPool[(_numQueries - 1) * 2];
	}

	_activeQuery = true;
	glQueryCounter(queryObjs[0], GL_TIMESTAMP);
}

void GPUTimer::endQuery()
{
	if (_activeQuery)
	{
		glQueryCounter(_queryPool[_numQueries * 2 - 1], GL_TIMESTAMP);
		_activeQuery = false;
	}
}

bool GPUTimer::updateResults()
{
	if (_numQueries == 0)
	{
		_time = 0;
		return true;
	}

	// Make sure that last query is available
	GLint available;
	glGetQueryObjectiv(_queryPool[_numQueries * 2 - 1], GL_QUERY_RESULT_AVAILABLE, &available);
	if (!available)
	{
		return false;
	}

	//  Accumulate time
	GLuint64 timeStart = 0, timeEnd = 0, timeAccum = 0;
	for (uint32 i = 0; i < _numQueries; ++i)
	{
		glGetQueryObjectui64v(_queryPool[i * 2], GL_QUERY_RESULT, &timeStart);
		glGetQueryObjectui64v(_queryPool[i * 2 + 1], GL_QUERY_RESULT, &timeEnd);
		timeAccum += timeEnd - timeStart;
	}

	_time = (float)((double)timeAccum / 1000000.0);
	return true;
}

void GPUTimer::reset()
{
	_time = 0.f;
}