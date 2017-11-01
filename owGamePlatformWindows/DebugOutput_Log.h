#pragma once

class DebugOutput_Log : public DebugOutput
{
public:
	bool Init();
	void Destroy();

protected:
	void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	ofstream logStream;
};
