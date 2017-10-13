#include "stdafx.h"

// General
#include "TexturesMgr.h"

// Additional
#include <SOIL.h>
#include "Render.h"

// Header

#include "../shared/pack_begin.h"

struct BLPHeader
{
	uint8 magic[4];
	uint32 type;

	uint8 compression;  // Compression: 1 for uncompressed, 2 for DXTC, 3 (cataclysm) for plain A8R8G8B8 textures
	uint8 alpha_depth;  // Alpha channel bit depth: 0, 1, 4 or 8
	uint8 alpha_type;   // 0, 1, 7, or 8
	uint8 has_mips;     // 0 = no mips, 1 = has mips

	uint32 width;
	uint32 height;
	uint32 mipOffsets[16];
	uint32 mipSizes[16];
};

#include "../shared/pack_end.h"

//

bool TexturesMgr::Init()
{
	ADDCONSOLECOMMAND_CLASS("tm_info", TexturesMgr, PrintAllInfo);

	RefManager1DimAssync::Init();

	black = CreateAction("black.png");
	white = CreateAction("white.png");

	return true;
}

void TexturesMgr::Destroy()
{
	RefManager1DimAssync::Destroy();
	
	DeleteAll();

	Modules::log().Info("TexturesMgr[]: All textures destroyed.");
}

//

bool TexturesMgr::LoadSoilTexture(File& _file, Texture* _texture)
{
	_texture->Bind();

	// Read data
	int32 sizeX, sizeY;
	uint8* image = SOIL_load_image_from_memory(_file.GetData(), static_cast<int32>(_file.GetSize()), &sizeX, &sizeY, 0, SOIL_LOAD_RGBA);

	if (SOIL_last_result() != "Image loaded from memory")
	{
		Modules::log().Error("TexturesMgr[%s]: Error while loading texture. Error [%s].", _file.Path_Name(), SOIL_last_result());
		SOIL_free_image_data(image);
		return false;
	}

	_texture->SetSize(vec2(sizeX, sizeY));

	// Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texture->GetSize().x, _texture->GetSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Params
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool TexturesMgr::LoadBLPTexture(File& _file, Texture* _texture)
{
	_texture->Bind();

	// Read data
	BLPHeader header;
	_file.ReadBytes(&header, 148);

	//Modules::log().Print("Texture[]: compression=[%d], alpha_depth=[%d], alpha_type=[%d], has_mips=[%d] [%s]", header.compression, header.alpha_depth, header.alpha_type, header.has_mips, _file.Path_Name().c_str());

	assert1(header.magic[0] == 'B' && header.magic[1] == 'L' && header.magic[2] == 'P' && header.magic[3] == '2');
	assert1(header.type == 1);

	_texture->SetSize(vec2(header.width, header.height));

	uint8 mipmax = header.has_mips ? 16 : 1;

	if (header.compression == 2)
	{
		GLint format;
		uint32 blocksize;

		if (header.alpha_type == 0)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			blocksize = 8u;
		}
		else if (header.alpha_type == 1)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blocksize = 16u;
		}
		else if(header.alpha_type == 7)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blocksize = 16u;
		}
		else
		{
			fail1();
		}

		uint8* buf = new uint8[header.mipSizes[0]];
		//uint8* ucbuf = new uint8[header.height * header.width * 4];

		// do every mipmap level
		for (uint8 i = 0; i < mipmax; i++)
		{
			if (header.width == 0) header.width = 1;
			if (header.height == 0)	header.height = 1;

			if (header.mipOffsets[i])
			{
				assert1(header.mipSizes[i] > 0);

				_file.Seek(header.mipOffsets[i]);
				_file.ReadBytes(buf, header.mipSizes[i]);

				uint32 size = ((header.width + 3) / 4) * ((header.height + 3) / 4) * blocksize;

				glCompressedTexImage2D(GL_TEXTURE_2D, i, format, header.width, header.height, 0, size, buf);
			}
			else
			{
				break;
			}

			header.width /= 2;
			header.height /= 2;
		}


		delete[] buf;
		//delete[] ucbuf;
	}
	else if (header.compression == 1)
	{
		unsigned int pal[256];
		_file.ReadBytes(pal, 1024);

		unsigned char* buf = new unsigned char[header.mipSizes[0]];
		unsigned int* buf2 = new unsigned int[header.width * header.height];
		unsigned int* p;
		unsigned char* c, *a;

		bool hasalpha = (header.alpha_depth != 0);

		for (int i = 0; i < mipmax; i++)
		{
			if (header.width == 0) header.width = 1;
			if (header.height == 0)	header.height = 1;

			if (header.mipOffsets[i] && header.mipSizes[i])
			{
				_file.Seek(header.mipOffsets[i]);
				_file.ReadBytes(buf, header.mipSizes[i]);

				int cnt = 0;
				p = buf2;
				c = buf;
				a = buf + header.width * header.height;
				for (uint32 y = 0; y < header.height; y++)
				{
					for (uint32 x = 0; x < header.width; x++)
					{
						unsigned int k = pal[*c++];
						k = ((k & 0x00FF0000) >> 16) | ((k & 0x0000FF00)) | ((k & 0x000000FF) << 16);
						int alpha;

						if (header.alpha_depth == 8)
						{
							alpha = (*a++);
						}
						else if (header.alpha_depth == 1)
						{
							alpha = (*a & (1 << cnt++)) ? 0xff : 0;
							if (cnt == 8)
							{
								cnt = 0;
								a++;
							}
						}
						else if (header.alpha_depth == 0)
						{
							alpha = 0xff;
						}


						k |= alpha << 24;
						*p++ = k;
					}
				}

				glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA8, header.width, header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf2);
			}
			else
			{
				break;
			}

			header.width /= 2;
			header.height /= 2;
		}

		delete[] buf2;
		delete[] buf;
	}
	else
	{
		Modules::log().Warn("Texture[%s]: compression=[%d]", _file.Path_Name().c_str(), header.compression);
		//fail1();
	}

	// Params
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

Texture* TexturesMgr::Add(cstring _textureFileName)
{
	return RefManager1DimAssync::Add(_textureFileName);
}

Texture* TexturesMgr::Add(File& _textureFile)
{
	return RefManager1DimAssync::Add(_textureFile.Path_Name());
}

// Protected


Texture* TexturesMgr::CreateAction(cstring name)
{
	GLuint textureOpenglId;
	glGenTextures(1, &textureOpenglId);

	Texture* texture = new Texture(textureOpenglId, vec2(16, 16));

	return texture;
}

void TexturesMgr::LoadAction(string _name, Texture* _texture)
{
	//wglMakeCurrent(_Render->dc, _Render->glrc2);

	File f = _name;

	if (!f.Open())
	{
		Modules::log().Error("TexturesMgr[%s]: Error while open texture.", f.Path_Name().c_str());
		_texture = _TexturesMgr->Black();
		return;
	}

	// Load texture
	bool result;
	if (f.Extension() == "blp")
	{
		result = _TexturesMgr->LoadBLPTexture(f, _texture);
	}
	else
	{
		result = _TexturesMgr->LoadSoilTexture(f, _texture);
	}

	// Check result
	if (!result)
	{
		Modules::log().Error("TexturesMgr[%s]: Error while loading texture data.", f.Path_Name().c_str());
		delete _texture;
		_texture = _TexturesMgr->Black();
		return;
	}

	//Modules::log().Info("TexturesMgr[%s]: Texture loaded. Size [%0.0fx%0.0f].", f.Path_Name().c_str(), _texture->GetSize().x, _texture->GetSize().y);
}

bool TexturesMgr::DeleteAction(cstring name)
{
	return true;
}