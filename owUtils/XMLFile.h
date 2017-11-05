#pragma once

class XMLNode
{
public:
	XMLNode(cstring _name, XMLNode* _parent);
	void DeleteChilds();
	void DeleteAll();

	void SetAsDataNode();

	string GetName() { return name; };
	XMLNode* GetParent() { return parent; };
	vector<XMLNode*>& GetChilds() { return childs; }
	map<string, string>& GetData() { return data; };
	bool IsKeyExists(cstring _keyName) const { return data.find(_keyName) != data.end(); }
	string GetKeyValue(string _keyName)
	{
		if (!IsKeyExists(_keyName))
			return "";
		return data[_keyName];
	}
	bool IsDataNode() const { return isDataNode; }

	void AddChild(XMLNode* _node);
	void AddData(cstring _keyName, cstring _data, bool append = false);

private:
	string name;
	XMLNode* parent;
	vector<XMLNode*> childs;
	map<string, string> data;

	bool isDataNode;
};

class XMLFile
{
public:
	 bool Open(cstring _filename);
	 void Destroy();

	 void Print(XMLNode* _startNode, int _level = 0) const;

	 XMLNode* GetRootNode() const { return rootNode; };

private:
	bool ProcessLine(string& _line);
	bool ProcessTag(string& _tag);
	bool ProcessData(string& _data);
	void DeleteDataNodes(XMLNode* _startNode);

private:
	string filename;
	XMLNode* rootNode;
};
