#pragma once

#include <iostream>

class Module;
class File;
class Texture;

class TexturesMgr : public Module, public RefManager1DimAssync<Texture>
{
public:
	DEF_MODULE(TexturesMgr);

	Texture* Add(cstring _textureFileName);
	Texture* Add(File& _textureFile);

	Texture* DefaultTexture() { return m_DefaultTexture; }

protected:
	Texture* CreateAction(cstring name) override;
	void LoadAction(string _name, Texture* _texture) override;
	bool DeleteAction(cstring name) override;

public:
	bool LoadBLPTexture(File& _file, Texture* _texture);

private:
    Texture* m_DefaultTexture;
    uint32   m_DefaultTexture2DObj;
    uint32   m_DefaultTexture3DObj;
    uint32   m_DefaultTextureCubeObj;
    vec2     m_DefaultTextureSize;

	ContainerAssync<string, Texture*> textures;
};

#define _TexturesMgr TexturesMgr::instance()
