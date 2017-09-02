#include "stdafx.h"

// Includes
#include "liquid.h"
#include "Wmo.h"

// General
#include "Wmo_Group.h"

// Additional
#include "world.h"
#include "shaders.h"
#include "quaternion.h"
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"


void setGLColor(uint32_t col) {
	GLubyte r, g, b, a;
	a = (col & 0xFF000000) >> 24;
	r = (col & 0x00FF0000) >> 16;
	g = (col & 0x0000FF00) >> 8;
	b = (col & 0x000000FF);
	glColor4ub(r, g, b, 1);
}

Vec4D colorFromInt(uint32_t col) {
	GLubyte r, g, b, a;
	a = (col & 0xFF000000) >> 24;
	r = (col & 0x00FF0000) >> 16;
	g = (col & 0x0000FF00) >> 8;
	b = (col & 0x000000FF);
	return Vec4D(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

/*
The fields referenced from the MOPR chunk indicate portals leading out of the WMO group in question.
For the "Number of batches" fields, A + B + C == the total number of batches in the WMO group (in the MOBA chunk). This might be some kind of LOD thing, or just separating the batches into different types/groups...?
Flags: always contain more information than flags in MOGI. I suppose MOGI only deals with topology/culling, while flags here also include rendering info.
Flag		Meaning
0x1 		something with bounding
0x4 		Has vertex colors (MOCV chunk)
0x8 		Outdoor
0x40
0x200 		Has lights  (MOLR chunk)
0x400
0x800 		Has doodads (MODR chunk)
0x1000 	        Has water   (MLIQ chunk)
0x2000		Indoor
0x8000
0x20000		Parses some additional chunk. No idea what it is. Oo
0x40000	        Show skybox
0x80000		isNotOcean, LiquidType related, see below in the MLIQ chunk.
*/
struct WMOGroupHeader {
	int32_t groupName;
	int32_t descriptiveGroupName;
	int32_t flags;
	CAaBox boundingBox;
	uint16_t portalStart; // Index into the MOPR chunk
	uint16_t portalCount; // Number of items used from the MOPR chunk
	int16_t batches[4];
	uint8_t fogs[4]; // Up to four indices into the WMO fog list
	int32_t liquidType; // LiquidType related, see below in the MLIQ chunk.
	int32_t wmoGroupId; // WMO group ID (column 4 in WMOAreaTable.dbc)
	int32_t unk2; // Always 0?
	int32_t unk3; // Always 0?
};

struct WMOBatch {
	uint32_t color0;
	uint32_t color1;
	uint32_t color2;
	uint32_t indexStart;
	uint16_t indexCount;
	uint16_t vertexStart;
	uint16_t vertexEnd;
	uint8_t flags;
	uint8_t texture; // index in MOMT
};

struct MOPY {
	struct {
		uint8_t F_UNK_0x01 : 1;
		uint8_t F_NOCAMCOLLIDE : 1;
		uint8_t F_DETAIL : 1;
		uint8_t F_COLLISION : 1; // Turns off rendering of water ripple effects. May also do more. Should be used for ghost material triangles.
		uint8_t F_HINT : 1;
		uint8_t F_RENDER : 1;
		uint8_t F_UNK_0x40 : 1;
		uint8_t F_COLLIDE_HIT : 1;

		bool isTransFace() { return F_UNK_0x01 && (F_DETAIL || F_RENDER); }
		bool isColor() { return !F_COLLISION; }
		bool isRenderFace() { return F_RENDER && !F_DETAIL; }
		bool isCollidable() { return F_COLLISION || isRenderFace(); }
	} flags;

	//uint8_t flags;
	uint8_t materialId;
};

struct WMOLiquidHeader {
	int X, Y, A, B;
	vec3 pos;
	short type;
};

WMOGroup::WMOGroup() :nBatches(0) { }

WMOGroup::~WMOGroup() {
	//if (dl) glDeleteLists(dl, 1);
	//if (dl_light) glDeleteLists(dl_light, 1);

	//for (size_t i=0; i<lists.size(); i++) {
	//	glDeleteLists(lists[i].first);
	//}
	if(nBatches && lists.size()) glDeleteLists(lists[0].first, nBatches);

	if(nDoodads) delete[] ddr;
	if(lq) delete lq;
}

void WMOGroup::init(WMO* wmo, File& f, int num, char* names) {
	this->wmo = wmo;
	this->num = num;

	WMOGroupInfo* groupInfo = new WMOGroupInfo;
	f.ReadBytes(groupInfo, groupInfo->__size);

	indoor = (groupInfo->flags & WMOGroupInfo::FLAG_IS_INDOOR);

	if(groupInfo->nameoffset > 0) {
		name = string(names + groupInfo->nameoffset);
	}
	else
		name = "(no name)";

	ddr = 0;
	nDoodads = 0;

	lq = 0;
}

void WMOGroup::initDisplayList() {
	vec3* vertices;
	vec3* normals;
	vec2* texcoords;
	unsigned short* indices;
	MOPY* materials;
	WMOBatch* batches;

	WMOGroupHeader gh;

	short *useLights = 0;
	int nLights = 0;

	// open group file
	char temp[256];
	strcpy(temp, wmo->GetName().c_str());
	temp[wmo->GetName().length() - 4] = 0;

	char fname[256];
	sprintf(fname, "%s_%03d.wmo", temp, num);

	File gf(fname);
	if(!gf.Open()) {
		return;
	}
	gf.Seek(0x14); // a header at 0x14

				   // read MOGP chunk header
	gf.ReadBytes(&gh, sizeof(WMOGroupHeader));
	WMOFog &wf = wmo->fogs[gh.fogs[0]];
	if(wf.largerRadius <= 0) fog = -1; // default outdoor fog..?
	else fog = gh.fogs[0];

	b1 = vec3(gh.boundingBox.min.x, gh.boundingBox.min.z, -gh.boundingBox.min.y);
	b2 = vec3(gh.boundingBox.max.x, gh.boundingBox.max.z, -gh.boundingBox.max.y);

	gf.Seek(0x58); // first chunk at 0x58

	char fourcc[5];
	uint32_t size = 0;

	uint32_t *cv;
	hascv = false;

	while(!gf.IsEof()) {
		memset(fourcc, 0, 4);
		size = 0;
		gf.ReadBytes(fourcc, 4);
		gf.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;

		if(size == 0)
			continue;

		size_t nextpos = gf.GetPos() + size;

		if(strcmp(fourcc, "MOPY") == 0) {
			// Material info for triangles, two bytes per triangle. So size of this chunk in bytes is twice the number of triangles in the WMO group.
			nTriangles = (int)size / 2;
			materials = (MOPY*)gf.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MOVI") == 0) {
			// Vertex indices for triangles. count = size / sizeof(unsigned short). Three 16-bit integers per triangle, that are indices into the vertex list.
			indices = (uint16_t*)gf.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MOVT") == 0) {
			// Vertices chunk., count = size / (sizeof(float) * 3). 3 floats per vertex, the coordinates are in (X,Z,-Y) order.
			nVertices = (int)size / 12;

			vertices = (vec3*)gf.GetDataFromCurrent();

			vmin = vec3(9999999.0f, 9999999.0f, 9999999.0f);
			vmax = vec3(-9999999.0f, -9999999.0f, -9999999.0f);

			rad = 0;
			for(int i = 0; i < nVertices; i++) {
				vec3 v(vertices[i].x, vertices[i].z, -vertices[i].y);
				if(v.x < vmin.x) vmin.x = v.x;
				if(v.y < vmin.y) vmin.y = v.y;
				if(v.z < vmin.z) vmin.z = v.z;
				if(v.x > vmax.x) vmax.x = v.x;
				if(v.y > vmax.y) vmax.y = v.y;
				if(v.z > vmax.z) vmax.z = v.z;
			}
			center = (vmax + vmin) * 0.5f;
			rad = glm::length (vmax - center);
		}
		else if(strcmp(fourcc, "MONR") == 0) {
			// Normals. 3 floats per vertex normal, in (X,Z,-Y) order.
			normals = (vec3*)gf.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MOTV") == 0) {
			// Texture coordinates, 2 floats per vertex in (X,Y) order. The values range from 0.0 to 1.0. Vertices, normals and texture coordinates are in corresponding order, of course.
			texcoords = (vec2*)gf.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MOBA") == 0) {
			// Render batches. Records of 24 bytes.

			nBatches = size / 24;
			batches = (WMOBatch*)gf.GetDataFromCurrent();

			/*
			// batch logging
			Debug::Info("\nWMO group #%d - %s\nVertices: %d\nTriangles: %d\nIndices: %d\nBatches: %d",
			this->num, this->name.c_str(), nVertices, nTriangles, nTriangles*3, nBatches);
			WMOBatch *ba = batches;
			for (int i=0; i<nBatches; i++) {
			Debug::Info("Batch %d:\t", i);

			for (int j=0; j<12; j++) {
			if ((j%4)==0 && j!=0) Debug::Info("| ");
			Debug::Info("%d\t", ba[i].bytes[j]);
			}

			Debug::Info("| %d\t%d\t| %d\t%d\t", ba[i].indexStart, ba[i].indexCount, ba[i].vertexStart, ba[i].vertexEnd);
			Debug::Info("%d\t%d\t%s", ba[i].flags, ba[i].texture, wmo->textures[ba[i].texture].c_str());

			}
			int l = nBatches-1;
			Debug::Info("Max index: %d", ba[l].indexStart + ba[l].indexCount);
			*/

		}
		else if(strcmp(fourcc, "MOLR") == 0) {
			// Light references, one 16-bit integer per light reference.
			// This is basically a list of lights used in this WMO group, the numbers are indices into the WMO root file's MOLT table.
			//For some WMO groups there is a large number of lights specified here, more than what a typical video card will handle at once. I wonder how they do lighting properly. Currently, I just turn on the first GL_MAX_LIGHTS and hope for the best. :(
			
			nLights = (int)size / 2;
			useLights = (short*)gf.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MODR") == 0) {
			// Doodad references, one 16-bit integer per doodad.
			// The numbers are indices into the doodad instance table (MODD chunk) of the WMO root file. These have to be filtered to the doodad set being used in any given WMO instance.
			
			nDoodads = size / 2;
			ddr = new short[nDoodads];
			gf.ReadBytes(ddr, size);
		}
		else if(strcmp(fourcc, "MOBN") == 0) {
			//Nodes of the BSP tree, used for collision(along with bounding boxes ? ). Array of t_BSP_NODE. / CAaBspNode. 0x10 bytes.
			/*struct t_BSP_NODE {
				uint16_t planeType;    // 4: leaf, 0 for YZ-plane, 1 for XZ-plane, 2 for XY-plane
				int16_t  children[2];  // index of bsp child node (right in this array)
				uint16_t numFaces;     // num of triangle faces in MOBR
				uint32_t firstFace;    // index of the first triangle index(in MOBR)
				float    fDist;
			};*/
		}
		else if(strcmp(fourcc, "MOBR") == 0) {
			// Triangle indices (in MOVI which define triangles) to describe polygon planes defined by MOBN BSP nodes.
		}
		else if(strcmp(fourcc, "MOCV") == 0) {
			/*
			Vertex colors, 4 bytes per vertex (BGRA), for WMO groups using indoor lighting.
			I don't know if this is supposed to work together with, or replace, the lights referenced in MOLR. But it sure is the only way for the ground around the goblin smelting pot to turn red in the Deadmines. (but some corridors are, in turn, too dark - how the hell does lighting work anyway, are there lightmaps hidden somewhere?)
			- I'm pretty sure WoW does not use lightmaps in it's WMOs...
			After further inspection, this is it, actual pre-lit vertex colors for WMOs - vertex lighting is turned off. This is used if flag 0x2000 in the MOGI chunk is on for this group. This pretty much fixes indoor lighting in Ironforge and Undercity. The "light" lights are used only for M2 models (doodads and characters). (The "too dark" corridors seemed like that because I was looking at it in a window - in full screen it looks pretty much the same as in the game) Now THAT's progress!!!
			*/
			hascv = true;
			cv = (uint32_t*)gf.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MLIQ") == 0) {
			/*
			Specifies liquids inside WMOs.
			This is where the water from Stormwind and BFD etc. is hidden. (slime in Undercity, pool water in the Darnassus temple, some lava in IF)
			Chunk header:
			Offset	Type 		Description
			0x00 	uint32_t 		number of X vertices (xverts)
			0x04 	uint32_t 		number of Y vertices (yverts)
			0x08 	uint32_t 		number of X tiles (xtiles = xverts-1)
			0x0C 	uint32_t 		number of Y tiles (ytiles = yverts-1)
			0x10 	float[3] 	base coordinates?
			0x1C 	uint16_t 		material ID
			The liquid data contains the vertex height map (xverts * yverts * 8 bytes) and the m_TileExists flags (xtiles * ytiles bytes) as descripbed in ADT files (MCLQ chunk). The length and width of a liquid m_TileExists is the same as on the map, that is, 1/8th of the length of a map chunk. (which is in turn 1/16th the length of a map m_TileExists).

			the real deal

			The LiquidType in the DBC is determined as follows:
			If var_0x3C in the root's MOHD has the 4 bit (&4), it will take the variable in MOGP's var_0x34. If not, it checks var_0x34 for 15 (green lava). If that is set, it will take 0, else it will take var_0x34 + 1.
			The result of this (0,var_0x34 or var_0x34+1) will be checked if above 21 (naxxramas slime). If yes, the entry is stored as given. Else, it will be checked for the type (ocean, water, magma, slime). Ocean might be overwritten by MOGP flags being & 0x80000.

			tl;dr

			MOGP.var_0x34 is LiquidType. This will be overwritten with the "WMO *" liquid types in case, this is below 21 (naxxramas slime). Additionally, it will be taken +1 if the flag in the root's header is not set.

			old:

			The material ID often refers to what seems to be a "special" material in the root WMO file (special because it often has a solid color/placeholder texture, or a texture from XTextures\*) - but sometimes the material referenced seems to be not special at all, so I'm not really sure how the liquid material is obtained - such as water/slime/lava.
			*/
			// liquids
			WMOLiquidHeader hlq;
			gf.ReadBytes(&hlq, 0x1E);

			//Debug::Info("WMO Liquid: %dx%d, %dx%d, (%f,%f,%f) %d", hlq.X, hlq.Y, hlq.A, hlq.B, hlq.pos.x, hlq.pos.y, hlq.pos.z, hlq.type);

			lq = new Liquid(hlq.A, hlq.B, vec3(hlq.pos.x, hlq.pos.z, -hlq.pos.y));
			//lq->initFromWMO(gf, wmo->mat[hlq.type], (flags & 0x2000) != 0);
		}
		else {
			Debug::Info("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}

		gf.Seek(nextpos);
	}

	initLighting(nLights, useLights);

	//dl = glGenLists(1);
	//glNewList(dl, GL_COMPILE);
	//glDisable(GL_BLEND);
	//glColor4f(1,1,1,1);

	// generate lists for each batch individually instead
	

	/*
	float xr=0,xg=0,xb=0;
	if (flags & 0x0040) xr = 1;
	if (flags & 0x2000) xg = 1;
	if (flags & 0x8000) xb = 1;
	glColor4f(xr,xg,xb,1);
	*/

	// assume that texturing is on, for unit 1
	GLuint listbase = glGenLists(nBatches);

	for(uint32_t b = 0; b < nBatches; b++) {

		GLuint list = listbase + b;

		WMOBatch* batch = &batches[b];
		WMOMaterial* mat = wmo->mat[batch->texture];

		bool overbright = (mat->IsUnshaded() && !hascv);
		bool spec_shader = (mat->GetShader() && !hascv && !overbright);

		pair<GLuint, int> currentList;
		currentList.first = list;
		currentList.second = spec_shader ? 1 : 0;

		glNewList(list, GL_COMPILE);

		// setup texture
		mat->setup();

		if(mat->GetBlendMode()) {
			glEnable(GL_ALPHA_TEST);

			if(mat->IsTesClampT()) 
				glAlphaFunc(GL_GREATER, 0.3f);

			if(mat->IsLightingDisabled()) 
				glAlphaFunc(GL_GREATER, 0.0f);
		}

		if(mat->IsTwoSided())
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		if(spec_shader) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorFromInt(mat->GetDiffuseColor()));
		}
		else {
			Vec4D nospec(0, 0, 0, 1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, nospec);
		}

		if(overbright) {
			GLfloat em[4] = {mat->EmissiveColor().r, mat->EmissiveColor().g, mat->EmissiveColor().b, mat->EmissiveColor().a};
			glMaterialfv(GL_FRONT, GL_EMISSION, em);
		}

		// render
		glBegin(GL_TRIANGLES);
		for(int t = 0, i = batch->indexStart; t < batch->indexCount; t++, i++) {
			int a = indices[i];

			if(indoor && hascv) {
				setGLColor(cv[a]);
			}

			glNormal3f(normals[a].x, normals[a].z, -normals[a].y);
			glTexCoord2fv(glm::value_ptr(texcoords[a]));
			glVertex3f(vertices[a].x, vertices[a].z, -vertices[a].y);
		}
		glEnd();

		if(overbright) {
			GLfloat em[4] = {0,0,0,1};
			glMaterialfv(GL_FRONT, GL_EMISSION, em);
		}

		if(mat->GetBlendMode()) {
			glDisable(GL_ALPHA_TEST);
		}

		glEndList();
		lists.push_back(currentList);
	}
}

void WMOGroup::initLighting(int nLR, short *useLights) {
	//dl_light = 0;
	// "real" lighting?
	if(indoor && hascv) {

		vec3 dirmin(1, 1, 1);
		float lenmin;
		int lmin;

		for(int i = 0; i < nDoodads; i++) {
			lenmin = 999999.0f*999999.0f;
			lmin = 0;
			ModelInstance &mi = wmo->modelis[ddr[i]];
			for(int j = 0; j < wmo->header.nLights; j++) {
				WMOLight &l = wmo->lights[j];
				vec3 dir = l.pos - mi.pos;
				float ll = glm::length2(dir);
				if(ll < lenmin) {
					lenmin = ll;
					dirmin = dir;
					lmin = j;
				}
			}
			mi.light = lmin;
			mi.ldir = dirmin;
		}

		outdoorLights = false;
	}
	else {
		outdoorLights = true;
	}
}

void WMOGroup::draw(cvec3 ofs, const float rot) {
	visible = false;
	// view frustum culling
	vec3 pos = center + ofs;
	rotate(ofs.x, ofs.z, &pos.x, &pos.z, rot*PI / 180.0f);
	if(!_World->frustum.intersectsSphere(pos, rad)) return;
	float dist = glm::length(pos - vec3(_Camera->Position.x, _Camera->Position.y, _Camera->Position.z)) - rad;
	if(dist >= _World->culldistance) return;
	visible = true;

	if(hascv) {
		glDisable(GL_LIGHTING);
		_World->outdoorLights(false);
	}
	else {
		if(_World->lighting) {
			if(_World->skies->hasSkies()) {
				_World->outdoorLights(true);
			}
			else {
				// set up some kind of default outdoor light... ?
				glEnable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				glLightfv(GL_LIGHT0, GL_AMBIENT, Vec4D(0.4f, 0.4f, 0.4f, 1));
				glLightfv(GL_LIGHT0, GL_DIFFUSE, Vec4D(0.8f, 0.8f, 0.8f, 1));
				glLightfv(GL_LIGHT0, GL_POSITION, Vec4D(1, 1, 1, 0));
			}
		}
		else glDisable(GL_LIGHTING);
	}
	setupFog();


	//glCallList(dl);
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);
	for(uint32_t i = 0; i < nBatches; i++) {
		bool useshader = (supportShaders && _World->useshaders && lists[i].second);
		if(useshader) wmoShader->bind();
		glCallList(lists[i].first);
		if(useshader) wmoShader->unbind();
	}

	glColor4f(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);

	if(hascv) {
		if(_World->lighting) {
			glEnable(GL_LIGHTING);
			//glCallList(dl_light);
		}
	}


}

void WMOGroup::drawDoodads(int doodadset, cvec3 ofs, const float rot) {
	if(!visible)
		return;

	if(nDoodads == 0)
		return;

	_World->outdoorLights(outdoorLights);
	setupFog();

	// draw doodads
	glColor4f(1, 1, 1, 1);
	for(int i = 0; i < nDoodads; i++) {
		short dd = ddr[i];
		bool inSet;
		// apparently, doodadset #0 (defaultGlobal) should always be visible
		inSet = (((dd >= wmo->doodadsets[doodadset].start) && (dd < (wmo->doodadsets[doodadset].start + wmo->doodadsets[doodadset].size)))
				 || ((dd >= wmo->doodadsets[0].start) && ((dd < (wmo->doodadsets[0].start + wmo->doodadsets[0].size)))));
		if(inSet) {
			//if ((dd >= wmo->doodadsets[doodadset].start) && (dd < (wmo->doodadsets[doodadset].start+wmo->doodadsets[doodadset].size))) {

			ModelInstance &mi = wmo->modelis[dd];

			if(!outdoorLights) {
				WMOLight::setupOnce(GL_LIGHT2, mi.ldir, mi.lcol);
			}

			wmo->modelis[dd].DrawAsDoodad(ofs, rot);
		}
	}

	glDisable(GL_LIGHT2);

	glColor4f(1, 1, 1, 1);

}

void WMOGroup::drawLiquid() {
	if(!visible) return;

	// draw liquid
	// TODO: culling for liquid boundingbox or something
	if(lq) {
		setupFog();
		if(outdoorLights) {
			_World->outdoorLights(true);
		}
		else {
			// TODO: setup some kind of indoor lighting... ?
			_World->outdoorLights(false);
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, Vec4D(0.1f, 0.1f, 0.1f, 1));
			glLightfv(GL_LIGHT2, GL_DIFFUSE, Vec4D(0.8f, 0.8f, 0.8f, 1));
			glLightfv(GL_LIGHT2, GL_POSITION, Vec4D(0, 1, 0, 0));
		}
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glDepthMask(GL_TRUE);
		glColor4f(1, 1, 1, 1);
		lq->draw();
		glDisable(GL_LIGHT2);
	}
}

void WMOGroup::setupFog() {
	if(outdoorLights || fog == -1) {
		_World->setupFog();
	}
	else {
		wmo->fogs[fog].setup();
	}
}



