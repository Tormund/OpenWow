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

	inline Texture* Black() { return black; }
	inline Texture* White() { return white; }

protected:
	Texture* CreateAction(cstring name) override;
	void LoadAction(string _name, Texture* _texture) override;
	bool DeleteAction(cstring name) override;

public:
	bool LoadSoilTexture(File& _file, Texture* _texture);
	bool LoadBLPTexture(File& _file, Texture* _texture);
	bool loadBLP(File& _file, Texture* _texture);
	void decompressDXTC(GLint format, int w, int h, size_t size, unsigned char* src, unsigned char* dest);

private:
	Texture* black;
	Texture* white;

public:
	HANDLE m_TextureLoader;
	HANDLE m_TextureAddedEvent;
	ContainerAssync<string, Texture*> textures;
};

#define _TexturesMgr TexturesMgr::instance()
