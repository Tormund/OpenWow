#pragma once

class XMLNode;
class XMLFile;

class UIFile {
public:
	OW_GAME_DLL_API bool Load(cstring _filename);
	OW_GAME_DLL_API void Destroy();

	OW_GAME_DLL_API UIWindow* GetRootWindow() const { return rootElement; }

	OW_GAME_DLL_API bool IsElementExists(cstring _name) const;
	OW_GAME_DLL_API UIElement* operator[](cstring _name);

	OW_GAME_DLL_API void SetParent(UIWindow* _parent);
	OW_GAME_DLL_API void Show() const;
	OW_GAME_DLL_API void Hide() const;

private:
	bool ProcessXMLNode(XMLNode* _node, UIElement* _parent);
	UIElement* LoadUIElement(XMLNode* _node);
	bool CreateUIElementByXMLNode(UIElement*& _element, XMLNode* _node);
	bool LoadCommonParams(UIElement*& _element, XMLNode* _node);

	Image* GetImage(XMLNode* _node);

private:
	string filename;

	UIWindow* parent;
	UIWindow* rootElement;

	map<string, UIElement*> elements;
};
