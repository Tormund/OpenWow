#include "../stdafx.h"

// General
#include "Technique.h"

// Additional


Technique::Technique(cstring _fileName)
{
	File f_vs = _fileName + ".vs";
	if (!f_vs.Open())
	{
		fail1();
	}

	File f_fs = _fileName + ".fs";
	if (!f_fs.Open())
	{
		fail1();
	}

	shaderId = _Render->r->createShader(string((char*)f_vs.GetData()).c_str(), string((char*)f_fs.GetData()).c_str(), nullptr, nullptr, nullptr, nullptr);
	Modules::log().Error("SH %s", _Render->r->getShaderLog().c_str());

	Modules::log().Error("ID == %d", shaderId);
}

Technique::Technique(cstring _fileNameVS, cstring _fileNameFS)
{
	File f_vs = _fileNameVS;
	if (!f_vs.Open())
	{
		fail1();
	}

	File f_fs = _fileNameFS;
	if (!f_fs.Open())
	{
		fail1();
	}

	shaderId = _Render->r->createShader(string((char*)f_vs.GetData()).c_str(), string((char*)f_fs.GetData()).c_str(), nullptr, nullptr, nullptr, nullptr);
	Modules::log().Error("SH %s", _Render->r->getShaderLog().c_str());

	Modules::log().Error("ID == %d", shaderId);
}

Technique::~Technique()
{

}

bool Technique::CompileProgram(cstring _programName)
{

	return true;
}