#include "stdafx.h"

// Include 
#include "MapChunk.h"

// General
#include "maptile.h"

// Additional
#include "MapWaterChunk.h"
#include "world.h"
#include "shaders.h"
#include "WMO_Instance.h"

#include "ModelsManager.h"
#include "WMOsManager.h"

struct MH2O_Header {
	/*
	uint32_t offsetInstances;
	This is an Offset to the first Water Layer, it has tobe an offset because there can be multiple layers.
	*/
	uint32_t offsetInstances;

	/*
	uint32_t layersCount;
	This is the count of the water layers present for this m_TileExists
	if layerCount is 0 there is no water in this m_TileExists also ofsVisibilityMask is 0
	because that information is also not needed.
	*/
	uint32_t layersCount;

	/*
	uint32_t offsetAttributes;
	This field requires some explanation.
	Its an offset to a uint8_t[8] data block.
	the data block contains a bit field 8x8 = 64 bit every bit represents a quad between 4 verts.
	But the bits in this field are only set to 1 if the field is really visible this is not ment in a
	technical way. To make it clear all of the 4 vertices have an alpha value (i explain them later)
	if only one of these alpha values smaller then 10 the bit is not set

	if the first byte is 0x17h which is in binary 00010111b it has to look like this in a grid:

	0123...
	1+-----> bits
	2| 11101000
	3|
	.|
	.V
	bytes

	*/
	uint32_t offsetAttributes;
};

struct MH2O_Instance {
	/*
	uint16_t flags;
	known values:
	2
	-> Means no min and max values
	-> type == 2 ?
	-> ocean

	5
	-> Means min and max values are given
	-> type == 0 ?
	-> waterfalls, rivers, lakes (everything that is not on ocean level)

	[TODO] Find all other values
	*/
	uint16_t liquidType;

	/*
	uint16_t type;
	The type seems tobee depend on the flags ??
	Maybe its an uint32_t and in general -> flags

	[TODO] Find all other values
	*/
	uint16_t liquidObject;

	/*
	float min;
	The minimum height from all vertices if flags == 5
	*/
	float minHeightLevel;

	/*
	float max;
	The maximum height from all vertices if flags == 5
	*/
	float maxHeightLevel;

	/*
	uint8_t x, y, w, h;
	These 4 values are used to define the size of the sub mask and
	the count of height and alpha values needed.

	x and y can be 0-7
	w and h can be 1-8

	Lets say we have a VisibilityMask like this:
	00000000
	00000000
	00000000
	00000000
	00111000
	11111100
	11111111
	11111111

	And the given Values for x y w h are 0 4 8 4 it means there MAYBE more info for this part (marked with X):
	00000000
	00000000
	xy 00000000
	\00000000_
	XXXXXXXX|
	XXXXXXXX| h
	XXXXXXXX|
	XXXXXXXX_
	|---w--|
	*/
	uint8_t xOffset;
	uint8_t yOffset;
	uint8_t width;
	uint8_t height;

	/*
	uint32_t ofsDisplayMask;
	This is a more detailed version of the VisibilityMask. Its an uint8_t[w*h/8] data block.

	The grid is created as before but every line only contains w bits

	If the offset is 0 then every quad marked by x y w h is displayed
	*/
	uint32_t offsetExistsBitmap;


	/*
	uint32_t ofsHeigthAlpha;

	This offset points to an array of heights and after that there is an array of alpha values.

	the size of both arrays is (w+1)*(h+1)
	8*8 quads -> 9*9 vertices so if w or h is 8 we need 9 values this explains the "+1"

	the heights array float[(w+1)*(h+1)] is only present if the flags == 5 otherwise (2) its not required

	the alpha array uint8_t[(w+1)*(h+1)] seems tobe always present if the offset is given and comes always
	after the heights array and if heights are not given it is directly at ofsHeigthAlpha
	*/
	uint32_t offsetVertexData;
};

bool getBitH2L(uint8_t* data, unsigned bit) {
	uint8_t byte = data[bit / 8];
	uint8_t pos = bit % 8;

	uint8_t mask = 0x80;
	mask >>= pos;

	return (byte & mask) == mask;
}

bool getBitL2H(uint8_t* data, unsigned bit) {
	uint8_t byte = data[bit / 8];
	uint8_t pos = bit % 8;

	uint8_t mask = 0x1;
	mask <<= pos;

	return (byte & mask) == mask;
}

///////////////////////////////////////////////////////////

struct MapTileHeader {
	uint32_t flags;

	uint32_t MCIN;
	uint32_t MTEX;
	uint32_t MMDX;
	uint32_t MMID;
	uint32_t MWMO;
	uint32_t MWID;
	uint32_t MDDF;
	uint32_t MODF;
	uint32_t MFBO;
	uint32_t MH2O;
	uint32_t MTXF;

	uint8_t mamp_value;             // Cata+, explicit MAMP chunk overrides data
	uint8_t padding[3];
	uint32_t unused[3];
};

MapTile::MapTile(int x0, int z0, cstring basename) : indexX(x0), indexZ(z0), wmoCount(0), mdxCount(0) {
	xbase = x0 * C_TileSize;
	zbase = z0 * C_TileSize;

	for(size_t j = 0; j < C_ChunksInTile; j++) {
		for(size_t i = 0; i < C_ChunksInTile; i++) {

			chunks[j][i] = new MapWaterChunk;
			//waterChunks[j][i] = nullptr;

		}
	}

	Debug::Info("Loading tile %d,%d", x0, z0);

	for(int fileindex = 0; fileindex < 3; fileindex++) {
		char name[256];
		load_phases phase;
		switch(fileindex) {
			case 0:
			phase = main_file;
			sprintf(name, "World\\Maps\\%s\\%s_%d_%d.adt", basename.c_str(), basename.c_str(), x0, z0);
			break;
			case 1:
			phase = tex;
			sprintf(name, "World\\Maps\\%s\\%s_%d_%d_tex0.adt", basename.c_str(), basename.c_str(), x0, z0);
			break;
			/*case 2:
				sprintf(name,"World\\Maps\\%s\\%s_%d_%d_tex1.adt", basename.c_str(), basename.c_str(), x0, z0);
				break;*/
			case 2:
			phase = obj;
			sprintf(name, "World\\Maps\\%s\\%s_%d_%d_obj0.adt", basename.c_str(), basename.c_str(), x0, z0);
			break;
		}
		parse_adt(name, phase);
	}

	// init quadtree
	//topnode = new MapNode(x, z, 16);
	//topnode->setup(this);

	// Verteces
	//technique = new LightTechnique();
	//technique->Init();
}

void MapTile::parse_adt(char* name, load_phases phase) {
	File f(name);
	ok = f.Open();
	if(!ok) {
		Debug::Info("MapTile[%s]: Error loading.", name);
		return;
	}

	char fourcc[5];
	uint32_t size;

	int chunkI = 0;
	int chunkJ = 0;

	while(!f.IsEof()) {
		memset(fourcc, 0, 4);
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);

		flipcc(fourcc);
		fourcc[4] = 0;

		if(size == 0)
			continue;

		size_t nextpos = f.GetPos() + size;

		if(strncmp(fourcc, "MVER", 4) == 0) {
			uint32_t version;
			f.ReadBytes(&version, 4);
			assert4(version == 18, "Version mismatch != 18", std::to_string(version).c_str(), name);
		}
		else if(strncmp(fourcc, "MHDR", 4) == 0) {
			// Contains offsets relative to &MHDR.data in the file for specific chunks.
		}
		else if(strncmp(fourcc, "MTEX", 4) == 0) {
			// List of textures used for texturing the terrain in this map m_TileExists.
			char* buf = new char[size + 1];
			f.ReadBytes(buf, size);
			buf[size] = 0;

			char* p = buf;
			while(p < buf + size) {
				string texpath(p);
				p += strlen(p) + 1;

				if(supportShaders) {
					string texshader = texpath;
					// load the specular texture instead
					texshader.insert(texshader.length() - 4, "_s");
					if(File::exists(texshader.c_str()))
						texpath = texshader;
				}

				// Preload texture
				textures.push_back(_TexturesMgr->Add(texpath));
			}
			delete[] buf;
		}
		else if(strncmp(fourcc, "MMDX", 4) == 0) {
			// List of filenames for M2 models that appear in this map m_TileExists.
			char* buf = new char[size + 1];
			f.ReadBytes(buf, size);
			buf[size] = 0;
			char *p = buf;
			while(p < buf + size) {
				string path(p);
				p += strlen(p) + 1;

				// Preload m2 model
				_ModelsMgr->Add(path);
				mdxNames.push_back(path);
			}
			delete[] buf;
		}
		else if(strncmp(fourcc, "MMID", 4) == 0) {
			// List of offsets of model filenames in the MMDX chunk.
		}
		else if(strncmp(fourcc, "MWMO", 4) == 0) {
			// List of filenames for WMOs (world map objects) that appear in this map m_TileExists.
			char *buf = new char[size + 1];
			f.ReadBytes(buf, size);
			buf[size] = 0;
			char* p = buf;
			while(p < buf + size) {
				string path(p);
				p += strlen(p) + 1;
	
				// Preload WMO
				_WMOsMgr->Add(path);
				wmoNames.push_back(path);
			}
			delete[] buf;
		}
		else if(strncmp(fourcc, "MWID", 4) == 0) {
			// List of offsets of WMO filenames in the MWMO chunk.
		}
		else if(strncmp(fourcc, "MDDF", 4) == 0) {
			// Placement information for doodads (M2 models).
			mdxCount = (int)size / 36;
			for(size_t i = 0; i < mdxCount; i++) {
				uint32_t mdxIndex;
				f.ReadBytes(&mdxIndex, 4);

				Model *model = (Model*)_ModelsMgr->items[_ModelsMgr->GetIDByName(mdxNames[mdxIndex])];
				ModelInstance inst(model, f);
				mdxInstances.push_back(inst);
			}
		}
		else if(strncmp(fourcc, "MODF", 4) == 0) {
			// Placement information for WMOs.
			wmoCount = size / 64;
			for(size_t i = 0; i < wmoCount; i++) {
				WMOPlacementInfo* placementInfo = new WMOPlacementInfo;
				f.ReadBytes(placementInfo, placementInfo->__size);

				WMO* wmo = (WMO*)_WMOsMgr->GetItemByName(wmoNames[placementInfo->nameIndex]);
				WMOInstance inst(wmo, placementInfo);
				wmoInstances.push_back(inst);
			}
		}
		else if(strncmp(fourcc, "MH2O", 4) == 0) {
			uint8_t* abuf = f.GetDataFromCurrent();

			for(size_t i = 0; i < C_ChunksInTile * C_ChunksInTile; i++) { // 256*12=3072 bytes
				MH2O_Header* mh2o_Header = (MH2O_Header*)abuf;

				//Debug::Green("MH2O: layers = %d", mh2o_Header->layersCount);

				for(size_t j = 0; j < mh2o_Header->layersCount; j++) {
					MH2O_Instance* mh2o_instance = new MH2O_Instance;
					mh2o_instance = (MH2O_Instance*)(f.GetDataFromCurrent() + mh2o_Header->offsetInstances + sizeof(MH2O_Instance) * j);

					auto LTYPE = gLiquidTypeDB.getByID(mh2o_instance->liquidType);
					auto VERTEX_FMT = gLiquidMaterialDB.getByID(LTYPE->Get_LiquidMaterialID());



					if(mh2o_instance->minHeightLevel - mh2o_instance->maxHeightLevel > 0.001f) {
						Debug::Green("MinHeight %f:", mh2o_instance->minHeightLevel);
						Debug::Green("MaxHeight %f:", mh2o_instance->maxHeightLevel);
						Debug::Error("MIN WATER != MAX_WATER!!!!");
					}

					//Debug::Green("---------");
					//Debug::Green("Chunk [%d][%d]", i / C_ChunksInTile, i % C_ChunksInTile);
					//Debug::Green("Layer %d:", j);
					
					//Debug::Green("MATERIAL = %d", LTYPE->Get_LiquidMaterialID());
					//Debug::Green("VERTEX FMT = %d", VERTEX_FMT->Get_LiquidVertexFormat());

					SWaterLayer waterLayer;

					waterLayer.x = mh2o_instance->xOffset;
					waterLayer.y = mh2o_instance->yOffset;
					waterLayer.w = mh2o_instance->width;
					waterLayer.h = mh2o_instance->height;
					waterLayer.flags = mh2o_instance->liquidType;
					waterLayer.levels[0] = mh2o_instance->minHeightLevel;
					waterLayer.levels[1] = mh2o_instance->maxHeightLevel;
					waterLayer.type = mh2o_instance->liquidObject;

					waterLayer.hasmask = mh2o_instance->offsetExistsBitmap != 0;
					if(waterLayer.hasmask) {
						unsigned co = mh2o_instance->width * mh2o_instance->height / 8;

						if(mh2o_instance->width * mh2o_instance->height % 8 != 0)
							co++;

						memcpy(waterLayer.mask, f.GetDataFromCurrent() + mh2o_instance->offsetExistsBitmap, co);

						for(size_t k = 0; k < (size_t)(waterLayer.w * waterLayer.h); k++) {
							if(getBitL2H(waterLayer.mask, (uint32_t)k)) {
								waterLayer.renderTiles.push_back(true);
							}
							else {
								waterLayer.renderTiles.push_back(false);
							}
						}
					}

					// Check exists vertex data
					if(mh2o_instance->offsetVertexData == 0) {
						Debug::Error("Liquid instance NOT offset Vertex data!!!");
						continue;
					}

					const size_t vertexDataSize = (mh2o_instance->width + 1) * (mh2o_instance->height + 1);

					if(VERTEX_FMT->Get_LiquidVertexFormat() == 0) {
						//Debug::Info("Case 0, Height and Depth data");

						float* pHeights = (float*)(f.GetDataFromCurrent() + mh2o_instance->offsetVertexData);
						uint8_t* pDepths = (uint8_t*)f.GetDataFromCurrent() + mh2o_instance->offsetVertexData + (sizeof(float) * vertexDataSize);

						for(size_t g = 0; g < vertexDataSize; g++) {
							waterLayer.heights.push_back(mh2o_instance->maxHeightLevel);
							//waterLayer.heights.push_back(pHeights[g]);
							waterLayer.depths.push_back(pDepths[g]);
							//Debug::Info("Height [%f]", pHeights[g]);
							//Debug::Info("Depth [%d]", pDepths[g]);
						}
					}
					else if(VERTEX_FMT->Get_LiquidVertexFormat() == 1) {
						//Debug::Info("Case 1, Height and Texture Coordinate data");

						struct uv_map_entry {
							uint16_t x;                      // divided by 8 for shaders
							uint16_t y;
						};

						float* pHeights = (float*)(f.GetDataFromCurrent() + mh2o_instance->offsetVertexData);
						uv_map_entry* pUVMap = (uv_map_entry*)f.GetDataFromCurrent() + mh2o_instance->offsetVertexData + (sizeof(float) * vertexDataSize);
						
						for(size_t g = 0; g < vertexDataSize; g++) {
							waterLayer.heights.push_back(pHeights[g]);
							//waterLayer.alphas.push_back( pUnknowns[g] );
							//Debug::Info("Height [%f]", pHeights[g]);
							//Debug::Info("Textures [%d] Y[%d]", pUVMap[g].x, pUVMap[g].y);
						}
					}
					else
						Debug::Error("Case %d : UNKNOWN!!!", mh2o_instance->liquidObject);


					// Add water
					chunks[i / C_ChunksInTile][i % C_ChunksInTile]->waterLayer.push_back(waterLayer);
				}

				abuf += sizeof(MH2O_Header);
			}
		}
		else if(strncmp(fourcc, "MCNK", 4) == 0) {
			chunks[chunkI][chunkJ]->init(&textures, f, phase);
			chunkJ++;
			if(chunkJ == 16) {
				chunkJ = 0;
				chunkI++;
			}
		}
		else if(strncmp(fourcc, "MFBO", 4) == 0) {
			// A bounding box for flying.
			/*
			struct plane {
				short[3][3] height;
			};
			struct {
				plane maximum;
				plane minimum;
			} MFBO;
			*/
		}
		else if(strncmp(fourcc, "MTFX", 4) == 0) {
			/*
			This chunk is an array of integers that are 1 or 0. 1 means that the texture at the same position in the MTEX array has to be handled differentely. The size of this chunk is always the same as there are entries in the MTEX chunk.
			Simple as it is:
			struct MTFX
			{
			uint32_t mode[nMTEX];
			}
			The textures with this extended rendering mode are no normal ones, but skyboxes. These skyboxes are getting added as a reflection layer on the terrain. This is used for icecubes reflecting clouds etc. The layer being the reflection one needs to have the 0x400 flag in the MCLY chunk.
			*/
		}
		else
		{
			Debug::Info("MapTile[%s]: No implement chunk %s [%d].", name, fourcc, size);
		}

		f.Seek(nextpos);
	}
}

MapTile::~MapTile() {
	if(!ok)
		return;

	Debug::Info("Unloading tile %d,%d", indexX, indexZ);

	for(size_t j = 0; j < C_ChunksInTile; j++) {
		for(size_t i = 0; i < C_ChunksInTile; i++) {
			chunks[j][i]->destroy();
		}
	}

	for(auto it = textures.begin(); it != textures.end(); ++it) {
		//video.textures.delbyname(*it);
	}

	for(vector<string>::iterator it = wmoNames.begin(); it != wmoNames.end(); ++it) {
		_WMOsMgr->Delete(*it);
	}

	for(vector<string>::iterator it = mdxNames.begin(); it != mdxNames.end(); ++it) {
		_ModelsMgr->Delete(*it);
	}
}

void MapTile::draw() {
	if(!ok)
		return;

	for(size_t j = 0; j < C_ChunksInTile; j++) {
		for(size_t i = 0; i < C_ChunksInTile; i++) {
			chunks[j][i]->visible = false;
			chunks[j][i]->draw();
		}
	}
}

void MapTile::drawWater() {
	if(!ok)
		return;

	for(size_t j = 0; j < C_ChunksInTile; j++)
		for(size_t i = 0; i < C_ChunksInTile; i++)
			if(chunks[j][i] != nullptr)
				chunks[j][i]->drawWater();
}

void MapTile::drawObjects() {
	if(!ok)
		return;

	for(auto it = wmoInstances.begin(); it != wmoInstances.end(); ++it)
		(*it).draw();
}

void MapTile::drawSky() {
	if(!ok) return;

	/*for(size_t i = 0; i < wmoCount; i++) {
		wmois[i].wmo->drawSkybox();
		if(_World->hadSky)
			break;
	}*/
}

void MapTile::drawModels() {
	if(!ok)
		return;

	for(auto it = mdxInstances.begin(); it != mdxInstances.end(); ++it)
		(*it).draw();
}

int indexMapBuf(int x, int y) {
	return ((y + 1) / 2) * 9 + (y / 2) * 8 + x;
}

MapChunk* MapTile::getChunk(uint32_t x, uint32_t z) {
	assert1(x < C_ChunksInTile && z < C_ChunksInTile);
	return chunks[z][x];
}
