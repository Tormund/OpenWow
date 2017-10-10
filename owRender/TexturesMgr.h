#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

class Module;
class File;
class Texture;

class TexturesMgr : public Module, public RefManager1DimAssync<Texture>
{
public:
	DEF_MODULE(TexturesMgr);

	Texture* Add(cstring _textureFileName);
	Texture* Add(File& _textureFile);

	Texture* Black() { return black; }
	Texture* White() { return white; }

protected:
	Texture* CreateAction(cstring name) override;
	void LoadAction(string _name, Texture* _texture) override;
	bool DeleteAction(cstring name) override;

public:
	bool LoadSoilTexture(File& _file, Texture* _texture);
	bool LoadBLPTexture(File& _file, Texture* _texture);

private:
	Texture* black;
	Texture* white;

	ContainerAssync<string, Texture*> textures;
};

#define _TexturesMgr TexturesMgr::instance()
