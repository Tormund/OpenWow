#include "stdafx.h"

// General
#include "XMLFile.h"

XMLNode::XMLNode(cstring _name, XMLNode* _parent)
{
	name = _name;
	parent = _parent;
	isDataNode = false;

	//Log::Info("XML: Node [%s] added.", name.c_str());
}

void XMLNode::DeleteChilds()
{
	for (auto it : childs)
		if (it != nullptr)
		{
			it->DeleteAll();
			delete it;
		}

	childs.clear();
}

void XMLNode::DeleteAll()
{
	DeleteChilds();

	data.clear();
}

void XMLNode::SetAsDataNode()
{
	isDataNode = true;

	if (!childs.empty())
	{
		//Log::Error("XMLFile: Data node [%s] has childs. Deleting childs.", name.c_str());
		DeleteChilds();
	}
}

void XMLNode::AddChild(XMLNode* _node)
{
	if (isDataNode)
	{
		//Log::Error("XML: Data node [%s] can't has childs.", name.c_str());
		return;
	}

	assert1(_node != nullptr);

	childs.push_back(_node);
}

void XMLNode::AddData(cstring _keyName, cstring _value, bool append)
{
	if (isDataNode)
	{
		//Log::Error("XML: Data node [%s] can't has data.", name.c_str());
		return;
	}

	if (append)
	{
		assert1(data.find(_keyName) != data.end());
		data[_keyName] += _value;
	}
	else
	{
		if (data.find(_keyName) != data.end())
			//Log::Error("Node [%s] key [%s] exists with value [%s], override with value [%s]", name.c_str(), _keyName.c_str(), data[_keyName].c_str(), _value.c_str());
		data[_keyName] = _value;
	}
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

bool XMLFile::Open(cstring _filename)
{
	filename = _filename;
	rootNode = nullptr;

	if (filename.empty())
	{
		//Log::Error("XML[%s]: No such file or directory!", filename.c_str());
		return false;
	}

	ifstream xmlStream;
	xmlStream.open(filename, ios::in);
	if (!xmlStream.is_open())
	{
		//Log::Error("XML[%s]: Can not open file!", filename.c_str());
		return false;
	}

	bool result = true;

	while (xmlStream.good() && result)
	{
		result = ProcessLine(Utils::getLine(xmlStream));
		if (!result)
		{
			//Log::Error("XML[%s]: Error while read file!", filename.c_str());
			if (rootNode != nullptr)
			{
				rootNode->DeleteAll();
				delete rootNode;
			}
			break;
		}
	}

	xmlStream.close();
	xmlStream.clear();

	if (rootNode == nullptr)
	{
		//Log::Error("XML[%s]: File is empty!", filename.c_str());
		return false;
	}

	DeleteDataNodes(rootNode);

	return true;
}

void XMLFile::Destroy()
{
	if (rootNode == nullptr)
		return;

	rootNode->DeleteAll();

	if (rootNode != nullptr)
		delete rootNode;
}

void XMLFile::Print(XMLNode* _startNode, int _level) const
{
	assert1(_startNode != nullptr);

	string line = "";
	string spaces = "";
	for (auto i = 0; i < _level; i++) spaces += "   ";
	line += spaces + '<' + _startNode->GetName() + '>';
	////Log::Print(line);

	//for (auto it : _startNode->GetData())
	//	//Log::Print(spaces + "[" + it.first + "=\"" + it.second + "\"]");

	//Log::Print("");

	for (auto it = _startNode->GetChilds().begin(); it != _startNode->GetChilds().end(); ++it)
	{
		assert1(*it != nullptr);
		Print(*it, _level + 1);
	}
}

XMLNode* currentNode = nullptr;
bool XMLFile::ProcessLine(string& _line)
{
	_line = Utils::Trim(_line);

	if (_line.empty())
		return true;

	auto openBracketPos = _line.find_first_of('<');
	auto closeBracketPos = _line.find_first_of('>');

	if (openBracketPos == string::npos && closeBracketPos != string::npos)
	{
		//Log::Error("XML[%s]: Syntax error: Can't find '<', but '>' finded. Line [%s]", filename.c_str(), _line.c_str());
		return false;
	}

	if (openBracketPos > closeBracketPos)
	{
		//Log::Error("XML[%s]: Syntax error: '<' previous than '>'. Line [%s]", filename.c_str(), _line.c_str());
		return false;
	}

	string data;
	string otherString;

	// data from 0 to openBracketPos;
	if (openBracketPos > 0 && openBracketPos <= _line.length())
	{
		// Process data
		data = _line.substr(0, openBracketPos);
		if (!ProcessData(data))
		{
			//Log::Error("XML[%s]: Error processing data [%s].", filename.c_str(), data);
			return false;
		}

		// Process other
		otherString = _line.substr(openBracketPos);
		assert1(!otherString.empty());
		//Log::Info("Proc line [%s]...", otherString.c_str());
		return ProcessLine(otherString);
	}

	// line is data only
	if (openBracketPos == string::npos)
	{
		// Process data
		data = _line;
		if (!ProcessData(data))
		{
			//Log::Error("XML[%s]: Error processing data [%s].", filename.c_str(), data.c_str());
			return false;
		}

		return true;
	}

	// Process TAG
	string tag = _line.substr(0, closeBracketPos + 1);
	if (!ProcessTag(tag))
	{
		//Log::Error("XML[%s]: Error processing tag [%s].", filename.c_str(), tag.c_str());
		return false;
	}

	// Process other line
	if (_line.size() > closeBracketPos + 1)
	{
		otherString = _line.substr(closeBracketPos + 1);
		assert1(!otherString.empty());
		//Log::Info("Proc line [%s]...", otherString.c_str());
		return ProcessLine(otherString);
	}

	return true;
}

bool XMLFile::ProcessTag(string& _tag)
{
	assert1(!_tag.empty());
	_tag = _tag.substr(1, _tag.size() - 2);
	//Log::Info("Proc tag [%s]...", _tag.c_str());

	// Skip comments
	if (_tag[0] == '?' || _tag[0] == '!')
		return true;

	// Close bracket]
	if (_tag[0] == '/')
	{
		_tag = _tag.substr(1);

		// If we find close tag, but hasn't open tag
		if (currentNode == nullptr)
		{
			//Log::Error("XML[%s]: Syntax error: Not currentTag previous endTag [%s] ", filename.c_str(), _tag.c_str());
			return false;
		}

		// If end tag not much current tag
		if (currentNode->GetName() != _tag)
		{
			//Log::Error("XML[%s]: Syntax error: EndTag [%s] is not much currentTag [%s]", filename.c_str(), _tag.c_str(), currentNode->GetName().c_str());
			return false;
		}

		currentNode = currentNode->GetParent();

		return true;
	}

	//////////////////////////
	// Process open tag
	//////////////////////////

	// If current node is data node, than break, because data nodes must not has childs.
	if (currentNode != nullptr)
		if (currentNode->IsDataNode())
		{
			//Log::Error("XML[%s]: Syntax error: Data tag [%s] can't has childs", filename.c_str(), currentNode->GetName().c_str());
			return false;
		}

	// If we have slash at end
	bool isSingleNode = false;
	auto slashPos = _tag.find_last_of('/');
	if (slashPos != string::npos && slashPos == _tag.length() - 1)
	{
		_tag = _tag.substr(0, slashPos);
		isSingleNode = true;
		//Log::Green("Is single");
	}

	// If we have space
	string currentNodeName = _tag;
	auto spacePos = _tag.find_first_of(' ');
	if (spacePos != string::npos)
	{
		currentNodeName = _tag.substr(0, spacePos);
	}

	// Add node
	XMLNode* newNode = nullptr;
	if (rootNode == nullptr)
	{
		newNode = new XMLNode(currentNodeName, rootNode);
		rootNode = newNode;
	}
	else
	{
		newNode = new XMLNode(currentNodeName, currentNode);
		currentNode->AddChild(newNode);
	}

	if (!isSingleNode)
		currentNode = newNode;

	// parse attribs
	if (spacePos != string::npos)
	{
		string attrs = Utils::Trim(_tag.substr(spacePos + 1));

		while (attrs.length() >= 5)
		{ // a="b"
			auto equalPos = attrs.find_first_of('=');
			string key = Utils::Trim(attrs.substr(0, equalPos));
			string value = Utils::Trim(attrs.substr(equalPos + 1));

			// First brackets
			auto quotesPos = value.find_first_of('"');
			if (quotesPos != 0)
			{
				//Log::Error("XML[%s]: Syntax error: Attributes error. Line [%s].", filename.c_str(), value.c_str());
				return false;
			}
			value = value.substr(1);

			// Second brackets
			quotesPos = value.find_first_of('"');
			if (quotesPos == 0)
			{
				//Log::Error("XML[%s]: Syntax error: Attributes error. Line [%s].", filename.c_str(), value.c_str());
				return false;
			}
			value = value.substr(0, quotesPos);

			// Add attributes
			assert1(!key.empty());
			assert1(!value.empty());
			assert1(newNode != nullptr);
			newNode->AddData(key, value);

			//Log::Green("KEY [%s] VALUE [%s]", key.c_str(), value.c_str());
			string substr = attrs.substr(key.length() + value.length() + 3);

			attrs = Utils::Trim(substr);
		}
	}

	return true;
}

bool XMLFile::ProcessData(string& _data)
{
	assert1(!_data.empty());
	//Log::Info("Proc data [%s]...", _data.c_str());

	// Add data or append data
	if (!currentNode->IsDataNode())
	{
		currentNode->GetParent()->AddData(currentNode->GetName(), _data);
		currentNode->SetAsDataNode();
		//Log::Print("XML: Node [%s] is data node.", currentNode->GetName().c_str());
	}
	else
		currentNode->GetParent()->AddData(currentNode->GetName(), _data, true);

	return true;
}

void XMLFile::DeleteDataNodes(XMLNode* _startNode)
{
	assert1(_startNode != nullptr);
	for (auto it = _startNode->GetChilds().begin(); it != _startNode->GetChilds().end(); )
	{
		auto node = *it;
		assert1(node != nullptr);
		if (node->IsDataNode())
		{
			//Log::Error("DATA Node FINDED [%s]", node->GetName().c_str());
			it = _startNode->GetChilds().erase(it);
			delete node;
		}
		else
		{
			DeleteDataNodes(node);
			++it;
		}
	}
}
