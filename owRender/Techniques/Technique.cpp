#include "../stdafx.h"

// General
#include "Technique.h"

// Additional

string ProcessShader(File& f)
{
	if (!f.Open())
	{
		Modules::log().Error("Error open shader [%s].", f.Path_Name().c_str());
		return "";
	}

	string data = "";

	while (!f.IsEof())
	{
		string line = f.ReadLine();

		// Skip empty lines
		if (line.length() == 0)
		{
			continue;
		}

		// Find directive
		if (line[0] == '#' && line[1] == 'i' && line[2] == 'n')
		{
			size_t firstBracketPosition = line.find('"');
			size_t lastBracketPosition = line.find_last_of('"');
			assert1(firstBracketPosition != string::npos);
			assert1(firstBracketPosition != lastBracketPosition);

			string inludeFileName = line.substr(firstBracketPosition + 1, lastBracketPosition - firstBracketPosition - 1);
			data += ProcessShader(File(f.Path() + inludeFileName)) + '\n';

			continue;
		}

		data += line + '\n';
	}

	return data;
}

//

Technique::Technique(cstring _fileName)
{
	string shVS = ProcessShader(File(_fileName + ".vs"));
	string shFS = ProcessShader(File(_fileName + ".fs"));

	shaderId = _Render->r->createShader(shVS.c_str(), shFS.c_str(), nullptr, nullptr, nullptr, nullptr);

	Modules::log().Error("SH %s", _Render->r->getShaderLog().c_str());
	Modules::log().Error("ID == %d", shaderId);
}

Technique::Technique(cstring _fileNameVS, cstring _fileNameFS)
{
	string shVS = ProcessShader(File(_fileNameVS));
	string shFS = ProcessShader(File(_fileNameFS));

	shaderId = _Render->r->createShader(shVS.c_str(), shFS.c_str(), nullptr, nullptr, nullptr, nullptr);

	Modules::log().Error("SH %s", _Render->r->getShaderLog().c_str());
	Modules::log().Error("ID == %d", shaderId);
}

Technique::~Technique()
{

}
