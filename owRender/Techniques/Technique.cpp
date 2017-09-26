#include "../stdafx.h"

// General
#include "Technique.h"

Technique::Technique(cstring _fileName)
{
	fileName = _fileName;
	programOglObj = 0;
	glfxEffectObj = glfxGenEffect();

	Debug::Print("Technique[%s]: Created. GLOBJ = [%d]", _fileName.c_str(), glfxEffectObj);
}

Technique::~Technique()
{
	if (programOglObj != 0)
	{
		glDeleteProgram(programOglObj);
		programOglObj = 0;
	}

	glfxDeleteEffect(glfxEffectObj);
}

bool Technique::CompileProgram(cstring _programName)
{
	File f = fileName;
	
	if (!glfxParseEffectFromFile(glfxEffectObj, f.FullPath().c_str())) // FIXME
	{
		string log = string(glfxGetEffectLog(glfxEffectObj));
		Debug::Error("Techique[%s]: Error creating program.", fileName.c_str());
		Debug::Error("Techique[%s]: Log [%s].", fileName.c_str(), log.c_str());
		fail1();
		return false;
	}

	programOglObj = glfxCompileProgram(glfxEffectObj, _programName.c_str());
	Debug::Info("Technique[%s]: Program [%s] OGLID [%d]", fileName.c_str(), _programName.c_str(), glfxEffectObj);

	if (programOglObj < 0)
	{
		string log = string(glfxGetEffectLog(glfxEffectObj));
		Debug::Error("Techique[%s]: Error compiling program [%s].", fileName.c_str(), _programName.c_str());
		Debug::Error("Techique[%s]: Log [%s].", fileName.c_str(), log.c_str());
		fail1();
		return false;
	}

	Debug::Print("Technique[%s]: Compiled. Program [%s]. GLOBJ = [%d]", fileName.c_str(), _programName.c_str(), glfxEffectObj);

	return true;
}