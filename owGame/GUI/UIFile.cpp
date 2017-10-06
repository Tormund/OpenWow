#include "../stdafx.h"

// Includes
#include "UIWindow.h"

// General
#include "UIFile.h"

// Additional
#include "UIElement.h"
#include "UIInput.h"
#include "UIButton.h"
#include "UIButton3t.h"
#include "UICheckBox.h"
#include "UIScrollBar.h"
#include "UIMgr.h"

bool UIFile::Load(cstring _filename)
{
	filename = _filename;

	XMLFile xmlFile;
	if (!xmlFile.Open(_filename))
	{
		Debug::Error("UIFile[%s]: Loading file error.", filename.c_str());
		return false;
	}

	parent = nullptr;
	rootElement = nullptr;
	XMLNode* rootXMLNode = xmlFile.GetRootNode();

	if (!ProcessXMLNode(rootXMLNode, nullptr))
	{
		Debug::Error("UIFile[%s]: Error while parsing data.", filename.c_str());
		xmlFile.Destroy();
		return false;
	}

	if (rootXMLNode == nullptr)
	{
		Debug::Error("UIFile[%s]: Can't init root node.", filename.c_str());
		xmlFile.Destroy();
		return false;
	}

	xmlFile.Destroy();

	return true;
}

void UIFile::Destroy()
{
	assert1(rootElement != nullptr);
	rootElement->Delete();
}

bool UIFile::IsElementExists(cstring _name) const
{
	return elements.find(_name) != elements.end();
}

UIElement* UIFile::operator[](cstring _name)
{
	if (!IsElementExists(_name))
	{
		assert4(false, "Element not found!", _name.c_str(), filename.c_str());
		return nullptr;
	}

	return elements.find(_name)->second;
}

void UIFile::SetParent(UIWindow* _parent)
{
	parent = _parent;
}

void UIFile::Show() const
{
	assert1(rootElement != nullptr);
	if (parent != nullptr)
		rootElement->Attach(parent);
	else
		_UIMgr->Attach(rootElement);
}

void UIFile::Hide() const
{
	assert1(rootElement != nullptr);
	rootElement->Detach();
}

bool UIFile::ProcessXMLNode(XMLNode* _node, UIElement* _parent)
{
	assert1(_node != nullptr);

	string nodeClassType = Utils::ToLower(_node->GetName());

	if (nodeClassType == "image")
		return true;

	// Create new element
	UIElement* element = LoadUIElement(_node);
	if (element == nullptr)
	{
		return false;
	}

	// Attach to parent
	if (_parent != nullptr)
	{
		UIWindow* parentAsUIWindow = dynamic_cast<UIWindow*>(_parent);
		if (parentAsUIWindow == nullptr)
		{
			Debug::Error("UIFile[%s]: Can't attach node [%s] to parent node (is not UIWindow).", nodeClassType.c_str());
			return false;
		}
		element->Attach(parentAsUIWindow);
	}

	UIWindow* elementAsUIWindow = dynamic_cast<UIWindow*>(element);

	// Set root window
	if (elementAsUIWindow != nullptr && rootElement == nullptr)
	{
		Debug::Print("UIFile[%s]: Node [%s] is UIWindow and set as rootElement.", filename.c_str(), _node->GetName().c_str());
		rootElement = elementAsUIWindow;
	}

	// Add childs
	for (auto it = _node->GetChilds().begin(); it != _node->GetChilds().end(); ++it)
	{
		assert1(*it != nullptr);
		if (!ProcessXMLNode(*it, element))
		{
			return false;
		}
	}

	return true;
}

UIElement* UIFile::LoadUIElement(XMLNode* _node)
{
	UIElement* _element = nullptr;

	// Try create UIElement by node name
	if (!CreateUIElementByXMLNode(_element, _node))
	{
		Debug::Error("UIFile[%s]: Can't create node [%s].", filename.c_str(), _node->GetName().c_str());
		return nullptr;
	}

	// Load common params
	if (!LoadCommonParams(_element, _node))
	{
		Debug::Error("UIFile[%s]: Can't load common params to node [%s].", filename.c_str(), _element->GetName().c_str());
		return nullptr;
	}

	return _element;
}

bool UIFile::CreateUIElementByXMLNode(UIElement*& _element, XMLNode* _node)
{
	string elementClassName = Utils::ToLower(_node->GetName());
	string elementName = _node->GetKeyValue("name");

	if (elementClassName == "uielement")
		_element = new UIElement();

	else if (elementClassName == "uiwindow")
		_element = new UIWindow();

	else if (elementClassName == "uiinput")
		_element = new UIInput();

	else if (elementClassName == "uibutton3t")
		_element = new UIButton3t();

	else if (elementClassName == "uibutton")
		_element = new UIButton();

	else if (elementClassName == "uiscrollbar")
		_element = new UIScrollBar();

	//
	if (_element == nullptr)
	{
		Debug::Warn("UIFile[%s]: Unknown element class [%s].", filename.c_str(), elementClassName.c_str());
		return true;
	}

	// SetName
	string resultElementName = elementName.empty() ? elementClassName : elementName;

	// Check dublicates
	if (IsElementExists(resultElementName))
	{
		int cntr = 0;
		while (IsElementExists(resultElementName + "_" + to_string(cntr))) cntr++;
		resultElementName += "_" + to_string(cntr);
	}

	// We have correct name
	_element->SetName(resultElementName);
	elements[_element->GetName()] = _element;
	Debug::Info("UIFile[%s]: Element [%s] added to table.", filename.c_str(), resultElementName.c_str());

	return true;
}

bool UIFile::LoadCommonParams(UIElement*& _element, XMLNode* _node)
{
	// Position
	string buff = _node->GetKeyValue("pos");
	auto pos = Utils::ToPoint(buff);

	// Size
	buff = _node->GetKeyValue("size");
	auto size = Utils::ToPoint(buff);

	// Image
	Image* image = GetImage(_node);

	// COLOR
	buff = _node->GetKeyValue("color");
	auto color = Utils::ToColorFromName(buff);

	// Init elemetns
	if (auto cast = dynamic_cast<UIWindow*>(_element))
	{
		cast->Init(pos, size, image, color);
	}
	else if (auto cast = dynamic_cast<UIInput*>(_element))
	{
		auto inputMode = Utils::ToInputMode(_node->GetKeyValue("mode"));
		cast->Init(pos, size, inputMode);
	}
	else if (auto cast = dynamic_cast<UIButton3t*>(_element))
	{
		auto textureName = _node->GetKeyValue("texture");
		cast->Init(pos, textureName);
	}
	else if (auto cast = dynamic_cast<UIScrollBar*>(_element))
	{
		cast->Init(pos, size);
	}
	else
		_element->Init(pos, size, image, color);

	// Text
	buff = _node->GetKeyValue("text");
	if (!buff.empty())
	{
		_element->SetText(buff);
		_element->ShowText();
	}

	// Text aligns
	buff = _node->GetKeyValue("textAlignW");
	if (!buff.empty())
		_element->SetTextAlignW(Utils::ToTextAlignW(buff));

	buff = _node->GetKeyValue("textAlignH");
	if (!buff.empty())
		_element->SetTextAlignH(Utils::ToTextAlignH(buff));

	// Text offset
	buff = _node->GetKeyValue("textOffset");
	if (!buff.empty())
		_element->SetTextOffset(Utils::ToPoint(buff));

	return true;
}

Image* UIFile::GetImage(XMLNode* _node)
{
	XMLNode* imageNode = nullptr;

	for (auto it = _node->GetChilds().begin(); it != _node->GetChilds().end(); ++it)
		if ((*it)->GetName() == "image")
		{
			if (imageNode != nullptr)
			{
				Debug::Error("UIFile[%s]: Node[%s]: Image: Dublicate param.", filename.c_str(), _node->GetName().c_str());
				return nullptr;
			}
			else
				imageNode = *it;
		}

	if (imageNode == nullptr)
		return nullptr;

	string textureName = imageNode->GetKeyValue("textureName");
	if (textureName.empty())
	{
		Debug::Error("UIFile[%s]: Node[%s]: Image: 'textureName' is empty.", filename.c_str(), _node->GetName().c_str());
		return nullptr;
	}

	auto texture = _TexturesMgr->Add(textureName);
	if (texture == nullptr)
	{
		Debug::Error("UIFile[%s]: Node[%s]: Image: Can't find texture [%s].", filename.c_str(), _node->GetName().c_str(), textureName.c_str());
		return nullptr;
	}

	string buff = imageNode->GetKeyValue("start");
	vec2 imageStart = Utils::ToPoint(buff);

	buff = imageNode->GetKeyValue("size");
	vec2 imageSize = Utils::ToPoint(buff);
	if (imageSize == VECTOR_ZERO)
	{
		Debug::Warn("UIFile[%s]: Node[%s]: Image: Size is zero. Set as texture size.", filename.c_str(), _node->GetName().c_str());
		imageSize = texture->GetSize();
	}

	buff = imageNode->GetKeyValue("offset");
	vec2 imageRenderOffset = Utils::ToPoint(buff);

	return new Image(texture, imageStart, imageSize, imageRenderOffset);
}
