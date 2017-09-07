#include "stdafx.h"

// General
#include "TexturesMgr.h"

// Additional
#include <SOIL.h>

bool TexturesMgr::Init()
{
	ADDCONSOLECOMMAND_CLASS("tm_info", TexturesMgr, PrintAllInfo);
	black = CreateAction("black.png", GenerateID());
	white = CreateAction("white.png", GenerateID());

	return true;
}

void TexturesMgr::Destroy()
{
	DeleteAll();

	Debug::Info("TexturesMgr: All textures destroyed.");
}

//

bool TexturesMgr::LoadSoilTexture(File& _file, Texture* _texture)
{
	_texture->Bind();

	// Read data
	int32_t sizeX, sizeY;
	uint8_t* image = SOIL_load_image_from_memory(_file.GetData(), static_cast<int>(_file.GetSize()), &sizeX, &sizeY, 0, SOIL_LOAD_RGBA);

	if (SOIL_last_result() != "Image loaded from memory")
	{
		Debug::Error("TexturesMgr[%s]: Error while loading texture. Error [%s].", _file.Path_Name(), SOIL_last_result());
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

	/*
	#define GL_NEAREST_MIPMAP_NEAREST         0x2700
	#define GL_LINEAR_MIPMAP_NEAREST          0x2701
	#define GL_NEAREST_MIPMAP_LINEAR          0x2702
	#define GL_LINEAR_MIPMAP_LINEAR           0x2703
	*/

	// set texture filtering parameters
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
	struct BLPHeader
	{
		uint8_t magic[4];
		uint32_t formatVersion;

		uint8_t compression;     // Compression: 1 for uncompressed, 2 for DXTC, 3 (cataclysm) for plain A8R8G8B8 textures
		uint8_t alphaBitDepth;   // Alpha channel bit depth: 0, 1, 4 or 8
		uint8_t preferredFormat; // (compressed, alpha:0 or 1): 0, (compressed, alpha:8): 1, uncompressed: 2,4 or 8 (mostly 8)
		uint8_t hasMips;

		uint32_t width;
		uint32_t height;
		uint32_t mipOffsets[16];
		uint32_t mipSizes[16];
	} header;
	_file.ReadBytes(&header, 148);

	assert1(header.formatVersion == 1);
	assert1(header.magic[0] == 'B' && header.magic[1] == 'L' && header.magic[2] == 'P' && header.magic[3] == '2');

	_texture->SetSize(vec2(header.width, header.height));

	int mipmax = header.hasMips ? 16 : 1;

	if (header.compression == 2)
	{
		GLint format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		int blocksize = 8;

		// guesswork here :(
		if (header.alphaBitDepth == 8 || header.alphaBitDepth == 4)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blocksize = 16;
		}

		if (header.alphaBitDepth == 8 && header.preferredFormat == 7)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blocksize = 16;
			
		}

		uint8_t *buf = new uint8_t[header.mipSizes[0]];
		//uint8_t *ucbuf = new uint8_t[header.height * header.width * 4];

		// do every mipmap level
		for (int i = 0; i < mipmax; i++)
		{
			if (header.width == 0) header.width = 1;
			if (header.height == 0)	header.height = 1;

			if (header.mipOffsets[i] && header.mipSizes[i])
			{
				_file.Seek(header.mipOffsets[i]);
				_file.ReadBytes(buf, header.mipSizes[i]);

				int size = ((header.width + 3) / 4) * ((header.height + 3) / 4) * blocksize;

				glCompressedTexImage2DARB(GL_TEXTURE_2D, i, format, header.width, header.height, 0, size, buf);
			}
			else
			{
				break;
			}

			header.width >>= 1;
			header.height >>= 1;
		}


		delete[] buf;
		//delete[] ucbuf;
	}
	else if (header.compression == 1)
	{

		Debug::Error("Texture COD = []");
		unsigned int pal[256];
		_file.ReadBytes(pal, 1024);

		unsigned char *buf = new unsigned char[header.mipSizes[0]];
		unsigned int *buf2 = new unsigned int[header.width * header.height];
		unsigned int *p;
		unsigned char *c, *a;

		bool hasalpha = (header.alphaBitDepth != 0);

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
				for (uint32_t y = 0; y < header.height; y++)
				{
					for (uint32_t x = 0; x < header.width; x++)
					{
						unsigned int k = pal[*c++];
						k = ((k & 0x00FF0000) >> 16) | ((k & 0x0000FF00)) | ((k & 0x000000FF) << 16);
						int alpha;

						if (header.alphaBitDepth == 8)
						{
							alpha = (*a++);
						}
						else if (header.alphaBitDepth == 1)
						{
							alpha = (*a & (1 << cnt++)) ? 0xff : 0;
							if (cnt == 8)
							{
								cnt = 0;
								a++;
							}
						}
						else if (header.alphaBitDepth == 0)
						{
							alpha = 0xff;
						}


						k |= alpha << 24;
						*p++ = k;
					}
				}

				//memset(buf2, 0x00, header.width * header.height); // DELETE ME!
				glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA8, header.width, header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf2);

			}
			else
			{
				break;
			}

			header.width >>= 1;
			header.height >>= 1;
		}

		delete[] buf2;
		delete[] buf;
	}
	else
	{
		assert1(false);
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

Texture* TexturesMgr::Generate()
{
	GLuint textureOpenglId = GenerateID();
	string name = "texture_" + std::to_string(textureOpenglId);

	Texture* texture = new Texture(textureOpenglId);
	do_add(name, textureOpenglId, texture);

	return texture;
}

Texture* TexturesMgr::Add(cstring _textureFileName)
{
	return RefManager::Add(_textureFileName);
}

Texture* TexturesMgr::Add(File& _textureFile)
{
	return RefManager::Add(_textureFile.Path_Name());
}

// Protected

GLuint TexturesMgr::GenerateID()
{
	GLuint textureOpenglId;
	glGenTextures(1, &textureOpenglId);

	return textureOpenglId;
}

Texture* TexturesMgr::CreateAction(cstring name, GLuint id)
{
	File f(name);

	if (!f.Open())
	{
		Debug::Error("TexturesMgr[%s]: Error while open texture.", f.Path_Name().c_str());
		return black;
	}

	Texture* texture = new Texture(id);

	// Load texture
	bool result;
	if (f.Extension() == "blp")
		result = LoadBLPTexture(f, texture);
	else
		result = LoadSoilTexture(f, texture);

	// Check result
	if (!result)
	{
		Debug::Error("TexturesMgr[%s]: Error while loading texture data.", f.Path_Name().c_str());
		delete texture;
		return black;
	}

	//Debug::Info("TexturesMgr[%s]: Texture loaded. Size [%fx%f].", f.Path_Name().c_str(), texture->size.x, texture->size.y);

	return texture;
}

bool TexturesMgr::DeleteAction(cstring name, GLuint id)
{
	return true;
}

struct Color4
{
	unsigned char r, g, b;
};

void decompressDXTC(GLint format, int w, int h, size_t size, unsigned char *src, unsigned char *dest)
{
	// sort of copied from linghuye
	int bsx = (w < 4) ? w : 4;
	int bsy = (h < 4) ? h : 4;

	for (int y = 0; y < h; y += bsy)
	{
		for (int x = 0; x < w; x += bsx)
		{
			unsigned long long alpha = 0;
			unsigned int a0 = 0, a1 = 0;

			if (format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
			{
				alpha = *(unsigned long long*)src;
				src += 8;
			}

			unsigned int c0 = *(unsigned short*)(src + 0);
			unsigned int c1 = *(unsigned short*)(src + 2);
			src += 4;

			Color4 color[4];
			color[0].b = (unsigned char)((c0 >> 11) & 0x1f) << 3;
			color[0].g = (unsigned char)((c0 >> 5) & 0x3f) << 2;
			color[0].r = (unsigned char)((c0) & 0x1f) << 3;
			color[1].b = (unsigned char)((c1 >> 11) & 0x1f) << 3;
			color[1].g = (unsigned char)((c1 >> 5) & 0x3f) << 2;
			color[1].r = (unsigned char)((c1) & 0x1f) << 3;
			if (c0 > c1)
			{
				color[2].r = (color[0].r * 2 + color[1].r) / 3;
				color[2].g = (color[0].g * 2 + color[1].g) / 3;
				color[2].b = (color[0].b * 2 + color[1].b) / 3;
				color[3].r = (color[0].r + color[1].r * 2) / 3;
				color[3].g = (color[0].g + color[1].g * 2) / 3;
				color[3].b = (color[0].b + color[1].b * 2) / 3;
			}
			else
			{
				color[2].r = (color[0].r + color[1].r) / 2;
				color[2].g = (color[0].g + color[1].g) / 2;
				color[2].b = (color[0].b + color[1].b) / 2;
				color[3].r = 0;
				color[3].g = 0;
				color[3].b = 0;
			}

			for (int j = 0; j < bsy; j++)
			{
				unsigned int index = *src++;
				unsigned char* dd = dest + (w*(y + j) + x) * 4;
				for (int i = 0; i < bsx; i++)
				{
					*dd++ = color[index & 0x03].b;
					*dd++ = color[index & 0x03].g;
					*dd++ = color[index & 0x03].r;
					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
					{
						*dd++ = ((index & 0x03) == 3 && c0 <= c1) ? 0 : 255;
					}
					else if (format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
					{
						*dd++ = (unsigned char)(alpha & 0x0f) << 4;
						alpha >>= 4;
					}
					index >>= 2;
				}
			}
		}
	}
}