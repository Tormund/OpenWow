#include "stdafx.h"

// Includes
#include "liquid.h"
#include "Wmo.h"

// General
#include "Wmo_Group.h"

// Additional
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"


void setGLColor(uint32_t col)
{
	GLubyte r, g, b, a;
	a = (col & 0xFF000000) >> 24;
	r = (col & 0x00FF0000) >> 16;
	g = (col & 0x0000FF00) >> 8;
	b = (col & 0x000000FF);
	glColor4ub(r, g, b, 1);
}

WMOGroup::WMOGroup()
{
	nTriangles = 0;
	materials = nullptr;

	indices = nullptr;

	nVertices = 0;
	vertices = nullptr;

	normals = nullptr;
	texcoords = nullptr;


	nBatches = 0;
	m_WMOBatchIndexes = nullptr;


	nLights = 0;
	m_WMOLightsIndexes = nullptr;


	nDoodads = 0;
	m_DoodadsIndexes = nullptr;

	hascv = false;
	m_VertexColors = nullptr;

	lq = nullptr;
}

WMOGroup::~WMOGroup()
{
	//if (dl) glDeleteLists(dl, 1);
	//if (dl_light) glDeleteLists(dl_light, 1);

	//for (size_t i=0; i<lists.size(); i++) {
	//	glDeleteLists(lists[i].first);
	//}
	if (nBatches && lists.size()) glDeleteLists(lists[0].first, nBatches);

	if (nDoodads) delete[] m_DoodadsIndexes;
	if (lq) delete lq;
}

void WMOGroup::init(WMO* wmo, File& f, int num, char* names)
{
	this->wmo = wmo;
	this->num = num;

	f.ReadBytes(&groupInfo, WMOGroupInfoDef::__size);

	indoor = (groupInfo.flags.FLAG_IS_INDOOR);

	if (groupInfo.nameoffset > 0)
	{
		name = string(names + groupInfo.nameoffset);
	}
	else
		name = "(no name)";

	m_DoodadsIndexes = 0;
	nDoodads = 0;

	lq = 0;
}

void WMOGroup::initDisplayList()
{



	// open group file
	char temp[256];
	strcpy_s(temp, wmo->GetName().c_str());
	temp[wmo->GetName().length() - 4] = 0;

	char fname[256];
	sprintf_s(fname, "%s_%03d.wmo", temp, num);

	File f(fname);
	if (!f.Open())
	{
		return;
	}
	f.Seek(0x14); // a header at 0x14

				   // read MOGP chunk header
	f.ReadBytes(&wmoGroupHeader, WMOGroupHeader::__size);
	WMOFog &wf = wmo->fogs[wmoGroupHeader.fogs[0]];
	
	if (wf.fogDef.largerRadius <= 0)
		fog = -1; // default outdoor fog..?
	else
		fog = wmoGroupHeader.fogs[0];

	b1 = vec3(wmoGroupHeader.boundingBox.min.x, wmoGroupHeader.boundingBox.min.z, -wmoGroupHeader.boundingBox.min.y);
	b2 = vec3(wmoGroupHeader.boundingBox.max.x, wmoGroupHeader.boundingBox.max.z, -wmoGroupHeader.boundingBox.max.y);

	f.Seek(0x58); // first chunk at 0x58




	char fourcc[5];
	uint32_t size = 0;
	while (!f.IsEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0)	continue;
		size_t nextpos = f.GetPos() + size;

		if (strcmp(fourcc, "MOPY") == 0)                 // Material info for triangles
		{
			nTriangles = size / WMOGroup_MaterialInfo::__size;
			materials = (WMOGroup_MaterialInfo*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVI") == 0)            // Vertex indices for triangles
		{
			indices = (uint16_t*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVT") == 0)            // Vertices chunk.
		{
			nVertices = size / 12;

			vertices = (vec3*)f.GetDataFromCurrent();

			vmin = vec3(9999999.0f, 9999999.0f, 9999999.0f);
			vmax = vec3(-9999999.0f, -9999999.0f, -9999999.0f);

			rad = 0;
			for (uint32_t i = 0; i < nVertices; i++)
			{
				vec3 v(vertices[i].x, vertices[i].z, -vertices[i].y);
				if (v.x < vmin.x) vmin.x = v.x;
				if (v.y < vmin.y) vmin.y = v.y;
				if (v.z < vmin.z) vmin.z = v.z;
				if (v.x > vmax.x) vmax.x = v.x;
				if (v.y > vmax.y) vmax.y = v.y;
				if (v.z > vmax.z) vmax.z = v.z;
			}
			center = (vmax + vmin) * 0.5f;
			rad = glm::length(vmax - center);
		}
		else if (strcmp(fourcc, "MONR") == 0)          // Normals
		{
			normals = (vec3*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOTV") == 0)          // Texture coordinates
		{
			texcoords = (vec2*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOBA") == 0)          // Render m_WMOBatchIndexes.
		{
			nBatches = size / WMOBatch::__size;
			m_WMOBatchIndexes = (WMOBatch*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOLR") == 0)          // Light references
		{
			nLights = size / sizeof(uint16_t);
			m_WMOLightsIndexes = (uint16_t*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODR") == 0)          // Doodad references
		{
			nDoodads = size / sizeof(uint16_t);
			m_DoodadsIndexes = new uint16_t[nDoodads];
			f.ReadBytes(m_DoodadsIndexes, size);
		}
		else if (strcmp(fourcc, "MOBN") == 0)
		{
		}
		else if (strcmp(fourcc, "MOBR") == 0)
		{
		}
		else if (strcmp(fourcc, "MOCV") == 0) // Vertex colors
		{
			hascv = true;
			m_VertexColors = (uint32_t*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MLIQ") == 0) // Liquid
		{
			WMOLiquidHeader hlq;
			f.ReadBytes(&hlq, WMOLiquidHeader::__size);

			lq = new Liquid(hlq.A, hlq.B, vec3(hlq.pos.x, hlq.pos.z, -hlq.pos.y));
			//lq->initFromWMO(f, wmo->mat[hlq.type], (flags & 0x2000) != 0);
		}
		else if (strcmp(fourcc, "MORI") == 0) 
		{
		}
		else if (strcmp(fourcc, "MORB") == 0)
		{
		}
		else if (strcmp(fourcc, "MOTA") == 0)
		{
		}
		else if (strcmp(fourcc, "MOBS") == 0)
		{
		}
		else
		{
			Debug::Info("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}

		f.Seek(nextpos);
	}

	initLighting();

	// assume that texturing is on, for unit 1
	GLuint listbase = glGenLists(nBatches);

	for (uint32_t b = 0; b < nBatches; b++)
	{

		GLuint list = listbase + b;

		WMOBatch* batch = &m_WMOBatchIndexes[b];
		WMOMaterial* mat = wmo->mat[batch->material_id];

		bool overbright = (mat->IsUnshaded() && !hascv);
		bool spec_shader = (mat->GetShader() && !hascv && !overbright);

		pair<GLuint, int> currentList;
		currentList.first = list;
		currentList.second = spec_shader ? 1 : 0;

		glNewList(list, GL_COMPILE);

		// setup texture
		mat->setup();

		if (mat->GetBlendMode())
		{
			glEnable(GL_ALPHA_TEST);

			if (mat->IsTesClampT())
				glAlphaFunc(GL_GREATER, 0.3f);

			if (mat->IsLightingDisabled())
				glAlphaFunc(GL_GREATER, 0.0f);
		}

		if (mat->IsTwoSided())
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		if (spec_shader)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(fromARGB(mat->GetDiffuseColor())));
		}
		else
		{
			vec4 nospec(0, 0, 0, 1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(nospec));
		}

		if (overbright)
		{
			GLfloat em[4] = {mat->EmissiveColor().r, mat->EmissiveColor().g, mat->EmissiveColor().b, mat->EmissiveColor().a};
			glMaterialfv(GL_FRONT, GL_EMISSION, em);
		}

		// render
		glBegin(GL_TRIANGLES);
		for (int t = 0, i = batch->indexStart; t < batch->indexCount; t++, i++)
		{
			int a = indices[i];

			if (indoor && hascv)
			{
				setGLColor(m_VertexColors[a]);
			}

			glNormal3f(normals[a].x, normals[a].z, -normals[a].y);
			glTexCoord2fv(glm::value_ptr(texcoords[a]));
			glVertex3f(vertices[a].x, vertices[a].z, -vertices[a].y);
		}
		glEnd();

		if (overbright)
		{
			GLfloat em[4] = {0,0,0,1};
			glMaterialfv(GL_FRONT, GL_EMISSION, em);
		}

		if (mat->GetBlendMode())
		{
			glDisable(GL_ALPHA_TEST);
		}

		glEndList();



		lists.push_back(currentList);
	}
}

void WMOGroup::initLighting()
{
	if (indoor && hascv)
	{
		vec3 dirmin(1, 1, 1);
		float lenmin;
		int lmin;

//#ifdef MDX_INCL
		for (uint32_t i = 0; i < nDoodads; i++)
		{
			lenmin = 999999.0f*999999.0f;
			lmin = 0;
			DoodadInstance& mi = wmo->m_MDXInstances[m_DoodadsIndexes[i]];
			for (uint32_t j = 0; j < wmo->header.nLights; j++)
			{
				WMOLight &l = wmo->lights[j];
				vec3 dir = l.lightDef.pos - mi.pos;
				float ll = glm::length2(dir);
				if (ll < lenmin)
				{
					lenmin = ll;
					dirmin = dir;
					lmin = j;
				}
			}
			mi.light = lmin;
			mi.ldir = dirmin;
	}
//#endif

		m_EnableOutdoorLights = false;
}
	else
	{
		m_EnableOutdoorLights = true;
	}
}

//

void WMOGroup::draw(cvec3 ofs, const float roll)
{
	visible = false;

	// view frustum culling
	vec3 pos = center + ofs;
	rotate(ofs.x, ofs.z, &pos.x, &pos.z, roll * PI / 180.0f);
	if (!_Render->frustum.intersectsSphere(pos, rad))
	{
		return;
	}

	float dist = glm::length(pos - vec3(_Camera->Position.x, _Camera->Position.y, _Camera->Position.z)) - rad;
	if (dist >= _WowSettings->culldistance)
	{
		return;
	}

	visible = true;

	/*if (hascv)
	{
		glDisable(GL_LIGHTING);
		_World->SetAmbientLights(false);
	}
	else
	{
		if (_WowSettings->lighting)
		{
			if (_World->skies->hasSkies())
			{
				_World->SetAmbientLights(true);
			}
			else
			{
				// Default lights
				glEnable(GL_LIGHT0);
				glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(vec4(0.4f, 0.4f, 0.4f, 1)));
				glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(vec4(0.8f, 0.8f, 0.8f, 1)));
				glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(vec4(1, 1, 1, 0)));

				glDisable(GL_LIGHT1);
			}
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}*/
	setupFog();


	//glCallList(dl);
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);
	for (uint32_t i = 0; i < nBatches; i++)
	{
		bool useshader = (supportShaders && _WowSettings->useshaders && lists[i].second);
		if (useshader) wmoShader->bind();
		glCallList(lists[i].first);
		if (useshader) wmoShader->unbind();
	}

	glColor4f(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);

	if (hascv)
	{
		if (_WowSettings->lighting)
		{
			glEnable(GL_LIGHTING);
			//glCallList(dl_light);
		}
	}


}

void WMOGroup::drawDoodads(int doodadset, cvec3 ofs, const float roll)
{
	if (!visible)
	{
		return;
	}

	if (nDoodads == 0)
	{
		return;
	}

	//_World->SetAmbientLights(m_EnableOutdoorLights);
	setupFog();

	// draw doodads
	glColor4f(1, 1, 1, 1);
	for (uint32_t i = 0; i < nDoodads; i++)
	{
		short dd = m_DoodadsIndexes[i];
		bool inSet;
		// apparently, doodadset #0 (defaultGlobal) should always be visible
		inSet = (
			((dd >= wmo->doodadsets[doodadset].start) && (dd < (wmo->doodadsets[doodadset].start + wmo->doodadsets[doodadset].size))) ||
			((dd >= wmo->doodadsets[0].start) && (dd < (wmo->doodadsets[0].start + wmo->doodadsets[0].size)))
			);
#ifdef MDX_INCL
		if (inSet)
		{
			//if ((dd >= wmo->doodadsets[doodadset].start) && (dd < (wmo->doodadsets[doodadset].start+wmo->doodadsets[doodadset].size))) {

			DoodadInstance &mi = wmo->m_MDXInstances[dd];

			if (!m_EnableOutdoorLights)
			{
				WMOLight::setupOnce(GL_LIGHT2, mi.ldir, mi.lcol);
			}

			wmo->m_MDXInstances[dd].Draw(ofs, roll);
	}
#endif
}

	glDisable(GL_LIGHT2);

	glColor4f(1, 1, 1, 1);

}

void WMOGroup::drawLiquid()
{
	if (!visible) return;

	// draw liquid
	// TODO: culling for liquid boundingbox or something
	if (lq)
	{
		setupFog();
		/*if (m_EnableOutdoorLights)
		{
			_World->SetAmbientLights(true);
		}
		else
		{
			// TODO: setup some kind of indoor lighting... ?
			_World->SetAmbientLights(false);
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, glm::value_ptr(vec4(0.1f, 0.1f, 0.1f, 1)));
			glLightfv(GL_LIGHT2, GL_DIFFUSE, glm::value_ptr(vec4(0.8f, 0.8f, 0.8f, 1)));
			glLightfv(GL_LIGHT2, GL_POSITION, glm::value_ptr(vec4(0, 1, 0, 0)));
		}*/
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glDepthMask(GL_TRUE);
		glColor4f(1, 1, 1, 1);
		lq->draw();
		glDisable(GL_LIGHT2);
	}
}

void WMOGroup::setupFog()
{
	/*if (m_EnableOutdoorLights || fog == -1)
	{
		_World->setupFog();
	}
	else
	{*/
		wmo->fogs[fog].setup();
	/*}*/
}



