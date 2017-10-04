#include "stdafx.h"

// General
#include "TexturesMgr.h"

// Additional
#include "ddslib.h"
#include <SOIL.h>
#include "Render.h"

// Header

#include "../shared/pack_begin.h"
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
};
#include "../shared/pack_end.h"

//

bool TexturesMgr::Init()
{
	ADDCONSOLECOMMAND_CLASS("tm_info", TexturesMgr, PrintAllInfo);

	RefManager1DimAssync::Init();

	black = CreateAction("black.png");
	white = CreateAction("white.png");

	bool supportCompression = glewIsSupported("GL_ARB_texture_compression GL_ARB_texture_cube_map GL_EXT_texture_compression_s3tc");
	if (!supportCompression)
	{
		fail1();
	}

	return true;
}

void TexturesMgr::Destroy()
{
	RefManager1DimAssync::Destroy();
	
	DeleteAll();

	Debug::Info("TexturesMgr[]: All textures destroyed.");
}

//

bool TexturesMgr::LoadSoilTexture(File& _file, Texture* _texture)
{
	_texture->Bind();

	// Read data
	int32_t sizeX, sizeY;
	uint8_t* image = SOIL_load_image_from_memory(_file.GetData(), static_cast<int32_t>(_file.GetSize()), &sizeX, &sizeY, 0, SOIL_LOAD_RGBA);

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

	//Debug::Print("Texture[]: compression=[%d], alphaBitDepth=[%d], preferredFormat=[%d], hasMips=[%d] [%s]", header.compression, header.alphaBitDepth, header.preferredFormat, header.hasMips, _file.Path_Name().c_str());

	assert1(header.magic[0] == 'B' && header.magic[1] == 'L' && header.magic[2] == 'P' && header.magic[3] == '2');
	assert1(header.formatVersion == 1);

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

			//_texture = _TexturesMgr->Black();
			//return true;
		}

		if (header.alphaBitDepth == 8 && header.preferredFormat == 7)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blocksize = 16;

		}

		/*if (header.alphaBitDepth == 72)
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blocksize = 16;
		}*/

		uint8_t* buf = new uint8_t[header.mipSizes[0]];
		uint8_t* ucbuf = new uint8_t[header.height * header.width * 4];

		// do every mipmap level
		for (int i = 0; i < mipmax; i++)
		{
			if (header.width == 0) header.width = 1;
			if (header.height == 0)	header.height = 1;

			if (header.mipOffsets[i] && header.mipSizes[i])
			{
				_file.Seek(header.mipOffsets[i]);
				_file.ReadBytes(buf, header.mipSizes[i]);

				uint32_t size = ((header.width + 3) / 4) * ((header.height + 3) / 4) * blocksize;

				glCompressedTexImage2DARB(GL_TEXTURE_2D, i, format, header.width, header.height, 0, size, buf);

				//decompressDXTC(format, header.width, header.height, size, buf, ucbuf);
				//glTexImage2D(GL_TEXTURE_2D, (GLint)i, GL_RGBA8, header.width, header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucbuf);
			}
			else
			{
				break;
			}

			header.width /= 2;
			header.height /= 2;
		}


		delete[] buf;
		delete[] ucbuf;
	}
	else if (header.compression == 1)
	{
		unsigned int pal[256];
		_file.ReadBytes(pal, 1024);

		unsigned char* buf = new unsigned char[header.mipSizes[0]];
		unsigned int* buf2 = new unsigned int[header.width * header.height];
		unsigned int* p;
		unsigned char* c, *a;

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

			header.width /= 2;
			header.height /= 2;
		}

		delete[] buf2;
		delete[] buf;
	}
	else
	{
		Debug::Warn("Texture[%s]: compression=[%d]", _file.Path_Name().c_str(), header.compression);
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
		Debug::Error("TexturesMgr[%s]: Error while open texture.", f.Path_Name().c_str());
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
		Debug::Error("TexturesMgr[%s]: Error while loading texture data.", f.Path_Name().c_str());
		delete _texture;
		_texture = _TexturesMgr->Black();
		return;
	}

	//Debug::Info("TexturesMgr[%s]: Texture loaded. Size [%0.0fx%0.0f].", f.Path_Name().c_str(), _texture->GetSize().x, _texture->GetSize().y);
}

bool TexturesMgr::DeleteAction(cstring name)
{
	return true;
}

//

bool TexturesMgr::loadBLP(File& _file, Texture* _texture)
{
	// Vars
	int offsets[16], sizes[16], type = 0;
	uint32_t width = 0, height = 0;
	GLint format = 0;
	char attr[4];

	// bind the texture
	_texture->Bind();

	//if (!file || !_file.open() || _file.isEof())
	//{
	//	id = 0;
	//	return;
	//}
	//else
	//{
		//tex->id = id; // I don't see the id being set anywhere,  should I set it now?
	//}

	_file.Seek(4);
	_file.ReadBytes(&type, 4);
	_file.ReadBytes(attr, 4);
	_file.ReadBytes(&width, 4);
	_file.ReadBytes(&height, 4);
	_file.ReadBytes(offsets, 4 * 16);
	_file.ReadBytes(sizes, 4 * 16);

	bool hasmipmaps = (attr[3] > 0);
	size_t mipmax = hasmipmaps ? 16 : 1;

	uint32_t w = width;
	uint32_t h = height;

	_texture->SetSize(vec2(width, height));

	/*
	reference: http://en.wikipedia.org/wiki/.BLP
	*/
	if (type == 0) // JPEG compression
	{
		/*
		* DWORD JpegHeaderSize;
		* BYTE[JpegHeaderSize] JpegHeader;
		* struct MipMap[16]
		* {
		*     BYTE[???] JpegData;
		* }
		*/
		/*LOG_ERROR << __FILE__ << __FUNCTION__ << __LINE__ << "type=" << type;

		BYTE *buffer = NULL;
		unsigned char *buf = new unsigned char[sizes[0]];

		_file.seek(offsets[0]);
		_file.ReadBytes(buf, sizes[0]);

		QImage image;

		if (!image.loadFromData(buf, sizes[0], "jpg"))
		{
			LOG_ERROR << __FUNCTION__ << __LINE__ << "Failed to load texture";
		}

		image = image.mirrored();
		image = image.convertToFormat(QImage::Format_RGBA8888);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

		delete buffer;
		buffer = 0;
		delete buf;
		buf = 0;*/
	}
	else if (type == 1)
	{
		if (attr[0] == 2)
		{
			/*
			Type 1 Encoding 2 AlphaDepth 0 (DXT1 no alpha)
			The image data is formatted using DXT1 compression with no alpha channel.

			Type 1 Encoding 2 AlphaDepth 1 (DXT1 one bit alpha)
			The image data is formatted using DXT1 compression with a one-bit alpha channel.

			Type 1 Encoding 2 AlphaDepth 8 (DXT3)
			The image data is formatted using DXT3 compression.

			Type 1 Encoding 2 AlphaDepth 8 AlphaEncoding 7 (DXT5)
			The image data are formatted using DXT5 compression.
			*/

			// encoding 2, directx compressed
			unsigned char *ucbuf = NULL;
			//if (!video.supportCompression)
				ucbuf = new unsigned char[width*height * 4];

			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			int blocksize = 8;

			// guesswork here :(
			// new alpha bit depth == 4 for DXT3, alfred 2008/10/11
			if (attr[1] == 8 || attr[1] == 4)
			{
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				blocksize = 16;
			}

			// Fix to the BLP2 format required in WoW 2.0 thanks to Linghuye (creator of MyWarCraftStudio)
			if (attr[1] == 8 && attr[2] == 7)
			{
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				blocksize = 16;
			}

			//compressed = true;

			unsigned char *buf = new unsigned char[sizes[0]];

			// do every mipmap level
			for (size_t i = 0; i < mipmax; i++)
			{
				if (width == 0) width = 1;
				if (height == 0) height = 1;
				if (offsets[i] && sizes[i])
				{
					_file.Seek(offsets[i]);
					_file.ReadBytes(buf, sizes[i]);

					int size = ((width + 3) / 4) * ((height + 3) / 4) * blocksize;

					//if (video.supportCompression)
					//{
					//	glCompressedTexImage2DARB(GL_TEXTURE_2D, (GLint)i, format, width, height, 0, size, buf);
					//}
					//else
					//{
					decompressDXTC(format, width, height, size, buf, ucbuf);
					glTexImage2D(GL_TEXTURE_2D, (GLint)i, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucbuf);
					//}

				}
				else
				{
					break;
				}
				width >>= 1;
				height >>= 1;
			}

			delete buf;
			buf = 0;
			//if (!video.supportCompression)
			//{
			delete ucbuf;
			ucbuf = 0;
			//}

		}
		else if (attr[0] == 1)
		{
			/*
			Type 1 Encoding 0 AlphaDepth 0 (uncompressed paletted image with no alpha)
			Each by of the image data is an index into Palette which contains the actual RGB value for the pixel. Although the palette entries are 32-bits, the alpha value of each Palette entry may contain garbage and should be discarded.

			Type 1 Encoding 1 AlphaDepth 1 (uncompressed paletted image with 1-bit alpha)
			This is the same as Type 1 Encoding 1 AlphaDepth 0 except that immediately following the index array is a second image array containing 1-bit alpha values for each pixel. The first byte of the array is for pixels 0 through 7, the second byte for pixels 8 through 15 and so on. Bit 0 of each byte corresponds to the first pixel (leftmost) in the group, bit 7 to the rightmost. A set bit indicates the pixel is opaque while a zero bit indicates a transparent pixel.

			Type 1 Encoding 1 AlphaDepth 8(uncompressed paletted image with 8-bit alpha)
			This is the same as Type 1 Encoding 1 AlphaDepth 0 except that immediately following the index array is a second image array containing the actual 8-bit alpha values for each pixel. This second array starts at BLP2Header.Offset[0] + BLP2Header.Width * BLP2Header.Height.
			*/

			// encoding 1, uncompressed
			unsigned int pal[256];
			_file.ReadBytes(pal, 1024);

			unsigned char *buf = new unsigned char[sizes[0]];
			unsigned int *buf2 = new unsigned int[width*height];
			unsigned int *p = NULL;
			unsigned char *c = NULL, *a = NULL;

			int alphabits = attr[1];
			bool hasalpha = (alphabits != 0);

			//compressed = false;

			for (size_t i = 0; i < mipmax; i++)
			{
				if (width == 0) width = 1;
				if (height == 0) height = 1;
				if (offsets[i] && sizes[i])
				{
					_file.Seek(offsets[i]);
					_file.ReadBytes(buf, sizes[i]);

					int cnt = 0;
					int alpha = 0;

					p = buf2;
					c = buf;
					a = buf + width*height;
					for (uint32_t y = 0; y < height; y++)
					{
						for (uint32_t x = 0; x < width; x++)
						{
							uint32_t k = pal[*c++];

							k = ((k & 0x00FF0000) >> 16) | ((k & 0x0000FF00)) | ((k & 0x000000FF) << 16);

							if (hasalpha)
							{
								if (alphabits == 8)
								{
									alpha = (*a++);
								}
								else if (alphabits == 4)
								{
									alpha = (*a & (0xf << cnt++)) * 0x11;
									if (cnt == 2)
									{
										cnt = 0;
										a++;
									}
								}
								else if (alphabits == 1)
								{
									//alpha = (*a & (128 >> cnt++)) ? 0xff : 0;
									alpha = (*a & (1 << cnt++)) ? 0xff : 0;
									if (cnt == 8)
									{
										cnt = 0;
										a++;
									}
								}
							}
							else alpha = 0xff;

							k |= alpha << 24;
							*p++ = k;
						}
					}

					glTexImage2D(GL_TEXTURE_2D, (GLint)i, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf2);

				}
				else break;

				width >>= 1;
				height >>= 1;
			}

			delete buf2;
			buf2 = 0;
			delete buf;
			buf = 0;
		}
		else
		{
			//LOG_ERROR << __FILE__ << __FUNCTION__ << __LINE__ << "type=" << type << "attr[0]=" << attr[0];
			return false;
		}
	}

	if (hasmipmaps)
	{
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Original
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void TexturesMgr::decompressDXTC(GLint format, int w, int h, size_t size, unsigned char *src, unsigned char *dest)
{
	// DXT1 Textures, currently being handles by our routine below
	if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		DDSDecompressDXT1(src, w, h, dest);
		return;
	}

	// DXT3 Textures
	if (format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
	{
		DDSDecompressDXT3(src, w, h, dest);
		return;
	}

	// DXT5 Textures
	if (format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
	{
		DDSDecompressDXT5(src, w, h, dest);
		return;
	}
}