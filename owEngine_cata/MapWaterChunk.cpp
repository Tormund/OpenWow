#include "stdafx.h"

// General
#include "MapWaterChunk.h"

// Additional
#include "World.h"

void MapWaterChunk::initTextures(const char* basename, int first, int last) {
	char buf[256];
	for(int i = first; i <= last; i++) {
		sprintf(buf, "%s.%d.blp", basename, i);
		wTextures.push_back(_TexturesMgr->Add(buf));
	}
}

void MapWaterChunk::drawWater() {
	if(wTextures.size() == 0)
		return;

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	size_t texidx = (size_t)(_World->animtime / 60.0f) % wTextures.size();
	wTextures[texidx]->Bind();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const float wr = 1.0f;

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	glEnable(GL_TEXTURE_2D);


	for(unsigned l = 0; l < waterLayer.size(); l++) {
		SWaterLayer& layer = waterLayer[l];

		for(unsigned y = layer.y; y < layer.h + layer.y; y++) {
			for(unsigned x = layer.x; x < layer.w + layer.x; x++) {

				unsigned tx = x - layer.x;
				unsigned ty = y - layer.y;

				// p1--p4
				// |    |  // this is GL_QUADS 
				// p2--p3
				unsigned p1 = tx + ty           * (layer.w + 1);
				unsigned p2 = tx + (ty + 1)     * (layer.w + 1);
				unsigned p3 = tx + 1 + (ty + 1) * (layer.w + 1);
				unsigned p4 = tx + 1 + ty       * (layer.w + 1);

				// alpha values helper
				float a1, a2, a3, a4;
				a1 = a2 = a3 = a4 = 1.0f;
				if(layer.depths.size() != 0) {
					a1 = (float)layer.depths[p1] / 255.f * 1.5f + 0.3f; // whats the magic formular here ???
					a2 = (float)layer.depths[p2] / 255.f * 1.5f + 0.3f;
					a3 = (float)layer.depths[p3] / 255.f * 1.5f + 0.3f;
					a4 = (float)layer.depths[p4] / 255.f * 1.5f + 0.3f;
				}

				// height values helper
				float h1, h2, h3, h4;
				h1 = h2 = h3 = h4 = 0.0f;
				if(layer.heights.size() != 0) {
					h1 = layer.heights[p1];
					h2 = layer.heights[p2];
					h3 = layer.heights[p3];
					h4 = layer.heights[p4];
				}

				if(layer.renderTiles.size() != 0)
					if(!layer.renderTiles[tx + ty * layer.w])
						continue;

				glColor4f(1.0, 1.0, 1.0, 1.0);

				glBegin(GL_QUADS);
				{
					glTexCoord2f(0, 0);
					glNormal3f(0, 1, 0);
					glVertex3f(xbase + C_UnitSize * x, h1, zbase + C_UnitSize * y);


					glTexCoord2f(0, wr);
					glNormal3f(0, 1, 0);
					glVertex3f(xbase + C_UnitSize * x, h2, zbase + C_UnitSize + C_UnitSize * y);


					glTexCoord2f(wr, wr);
					glNormal3f(0, 1, 0);
					glVertex3f(xbase + C_UnitSize + C_UnitSize * x, h3, zbase + C_UnitSize + C_UnitSize * y);

					glTexCoord2f(wr, 0);
					glNormal3f(0, 1, 0);
					glVertex3f(xbase + C_UnitSize + C_UnitSize * x, h4, zbase + C_UnitSize * y);
				}
				glEnd();
			}
		}
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}