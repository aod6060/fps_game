#include "sys.h"


void TerrainData::init()
{
	this->seed = rand() % UINT_MAX;

	// Resizing Maps
	this->elevation.resize(this->size * this->size);
	this->mask.resize(this->size * this->size);
	this->maskedElevation.resize(this->size * this->size);
	this->moister.resize(this->size * this->size);
	this->blend.resize(this->size * this->size);
	this->biomes.resize(this->size * this->size);
	this->terrainType.resize(this->size * this->size);

	// Surface
	this->elevationSurf = SDL_CreateRGBSurfaceWithFormat(0, size, size, 1, SDL_PIXELFORMAT_BGR888);
	this->maskSurf = SDL_CreateRGBSurfaceWithFormat(0, size, size, 1, SDL_PIXELFORMAT_BGR888);
	this->maskedElevationSurf = SDL_CreateRGBSurfaceWithFormat(0, size, size, 1, SDL_PIXELFORMAT_BGR888);
	this->moisterSurface = SDL_CreateRGBSurfaceWithFormat(0, size, size, 1, SDL_PIXELFORMAT_BGR888);
	this->blendSurf = SDL_CreateRGBSurfaceWithFormat(0, size, size, 1, SDL_PIXELFORMAT_BGR888);
	this->biomesSurf = SDL_CreateRGBSurfaceWithFormat(0, size, size, 1, SDL_PIXELFORMAT_BGR888);

	for (int y = 0; y < this->size; y++)
	{
		for (int x = 0; x < this->size; x++)
		{
			int i = y * this->size + x;

			// Elevation
			float n1 = (glm::perlin(glm::vec3(x / this->hm_wave1, y / this->hm_wave1, seed)) + 1.0f) * 0.5f;
			float n2 = (glm::perlin(glm::vec3(x / this->hm_wave2, y / this->hm_wave2, seed + 1)) + 1.0f) * 0.5f;
			float n3 = (glm::perlin(glm::vec3(x / this->hm_wave3, y / this->hm_wave3, seed + 2)) + 1.0f) * 0.5f;
			float n =
				n1 * this->hm_weight1 +
				n2 * this->hm_weight2 +
				n3 * this->hm_weight3;

			this->elevation[i] = glm::clamp(
				n,
				0.0f,
				1.0f
			);

			// Island Mask
			float m = this->toMask(x, y, 10.0f);
			mask[i] = m;

			// Masked Elevation
			float me = this->elevation[i] * mask[i];
			maskedElevation[i] = me;

			glm::vec2 mo;
			mo.x = (me > 0.25f) ? me : 0.0f;
			float t1 = ((glm::perlin(glm::vec3(x / 32.0f, y / 32.0f, seed + 3)) + 1.0f) * 0.5f) * me;
			float t2 = ((glm::perlin(glm::vec3(x / 16.0f, y / 16.0f, seed + 4)) + 1.0f) * 0.5f) * me;
			float t3 = ((glm::perlin(glm::vec3(x / 8.0f, y / 8.0f, seed + 5)) + 1.0f) * 0.5f) * me;
			float temp = t1 * 0.5f + t2 * 0.35f + t3 * 0.15f;

			mo.y = (me > 0.3f) ? temp : 0.0f;

			moister[i] = mo;

			// Blend Map
			if (me <= 0.25f)
			{
				this->blend[i] = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			else if (me > 0.25f)
			{
				float t = glm::length(moister[i]);

				if (t >= 0.25 && t < 0.35)
				{
					this->blend[i] = glm::vec3(1.0f, 0.0f, 0.0f);
				}
				else if (t >= 0.35 && t < 0.45)
				{
					this->blend[i] = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				else if (t >= 0.45) {
					this->blend[i] = glm::vec3(0.0f, 0.0f, 1.0f);
				}
			}

			// Biomes
			if (me <= 0.25f)
			{
				this->biomes[i] = glm::vec3(0.0f, 0.0f, 1.0f);
				this->terrainType[i] = TT_OCEAN;
			}
			else if (me >= 0.25f)
			{
				float t = glm::length(moister[i]);

				if (t >= 0.25 && t < 0.35)
				{
					this->biomes[i] = glm::vec3(194 / 256.0f, 178 / 256.0f, 128 / 256.0f);
					this->terrainType[i] = TT_BEACH;
				}
				else if (t >= 0.35 && t < 0.45)
				{
					this->biomes[i] = glm::vec3(0.3f, 1.0f, 0.3f);
					this->terrainType[i] = TT_GRASS;
				}
				else if (t >= 0.45)
				{
					this->biomes[i] = glm::vec3(139 / 256.0f, 69 / 256.0f, 19 / 256.0f);
					this->terrainType[i] = TT_SNOW;
				}
			}
		}
	}


	// Surface
	uint32_t* epixel = (uint32_t*)this->elevationSurf->pixels;
	uint32_t* mpixel = (uint32_t*)this->maskSurf->pixels;
	uint32_t* mepixel = (uint32_t*)this->maskedElevationSurf->pixels;
	uint32_t* mopixel = (uint32_t*)this->moisterSurface->pixels;
	uint32_t* bpixel = (uint32_t*)this->blendSurf->pixels;
	uint32_t* bmpixel = (uint32_t*)this->biomesSurf->pixels;

	for (int y = 0; y < this->size; y++)
	{
		for (int x = 0; x < this->size; x++)
		{
			int i = y * this->size + x;

			// Elevation
			uint8_t e = this->elevation[i] * 255;
			epixel[i] = SDL_MapRGB(this->elevationSurf->format, e, e, e);

			// Mask
			uint8_t m = this->mask[i] * 255;
			mpixel[i] = SDL_MapRGB(this->maskSurf->format, m, m, m);

			// Masked Elevation
			uint8_t me = this->maskedElevation[i] * 255;
			mepixel[i] = SDL_MapRGB(this->maskedElevationSurf->format, me, me, me);

			// Moister 
			uint8_t mr = this->moister[i].r * 255;
			uint8_t mg = this->moister[i].g * 255;
			mopixel[i] = SDL_MapRGB(this->moisterSurface->format, mr, 0, mg);

			// Blend Map
			uint8_t br = this->blend[i].r * 255;
			uint8_t bg = this->blend[i].g * 255;
			uint8_t bb = this->blend[i].b * 255;
			bpixel[i] = SDL_MapRGB(this->blendSurf->format, br, bg, bb);

			// Biomes
			uint8_t bmr = this->biomes[i].r * 255;
			uint8_t bmg = this->biomes[i].g * 255;
			uint8_t bmb = this->biomes[i].b * 255;
			bmpixel[i] = SDL_MapRGB(this->biomesSurf->format, bmr, bmg, bmb);

		}
	}

	// Textures...
	this->elevationTex.init(
		this->elevationSurf->pixels, 
		size, 
		size);

	this->maskTex.init(
		this->maskSurf->pixels,
		size,
		size);

	this->maskedElevationTex.init(
		this->maskedElevationSurf->pixels,
		size,
		size
	);

	this->moisterTex.init(
		this->moisterSurface->pixels,
		size,
		size
	);

	this->blendMap.init(
		this->blendSurf->pixels,
		size,
		size,
		GL_LINEAR,
		GL_LINEAR
	);

	this->biomesMap.init(
		this->biomesSurf->pixels,
		size,
		size
	);

}

void TerrainData::release()
{
	this->biomesMap.release();
	SDL_FreeSurface(this->biomesSurf);
	this->blendMap.release();
	SDL_FreeSurface(this->blendSurf);
	this->moisterTex.release();
	SDL_FreeSurface(this->moisterSurface);
	this->maskedElevationTex.release();
	SDL_FreeSurface(this->maskedElevationSurf);
	this->maskTex.release();
	SDL_FreeSurface(this->maskSurf);
	this->elevationTex.release();
	SDL_FreeSurface(this->elevationSurf);
}

float TerrainData::toMask(float x, float y, float radius)
{
	float distX = fabs(x - size * 0.5f);
	float distY = fabs(y - size * 0.5f);
	float dist = sqrt(distX * distX + distY * distY);

	float max_width = size * 0.5f - radius;
	float delta = dist / max_width;
	float gradient = delta * delta;

	return fmax(0.0f, 1.0f - gradient);
}