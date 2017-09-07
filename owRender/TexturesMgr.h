#pragma once

class Module;
class File;
class Texture;

class TexturesMgr : public Module, public RefManager<Texture, GLuint>
{
public:
	DEF_MODULE(TexturesMgr, OW_RENDER_DLL_API);

	OW_RENDER_DLL_API Texture* Generate();
	OW_RENDER_DLL_API Texture* Add(cstring _textureFileName);
	OW_RENDER_DLL_API Texture* Add(File& _textureFile);

	inline Texture* Black() { return black; }
	inline Texture* White() { return white; }

protected:
	GLuint GenerateID() override;
	Texture* CreateAction(cstring name, GLuint id) override;
	bool DeleteAction(cstring name, GLuint id) override;

private:
	bool LoadSoilTexture(File& _file, Texture* _texture);
	bool LoadBLPTexture(File& _file, Texture* _texture);

private:
	Texture* black;
	Texture* white;
};

#define _TexturesMgr TexturesMgr::instance()
