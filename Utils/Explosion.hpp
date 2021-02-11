#include "HMath.h"
#include "../SDK/CBlockLegacy.h"
#include "../SDK/CEntity.h"
#include "../Memory/GameData.h"

struct Explosion {
	int power;
	vec3_t pos;
	Explosion(float x, float y, float z, int power) {
		pos = vec3_t(x, y, z);
		this->power = power;
	}
	Explosion(vec3_t pos, int power) {
		this->pos = pos;
		this->power = power;
	}
	Explosion(int x, int y, int z, int power, bool crystal = true) {
		if (crystal) {
			this->pos = vec3_ti(x, y, z).toVec3t().add(.5f, 1.f, .5f);
		} else {
			this->pos = vec3_ti(x, y, z).toVec3t();
		}
		this->power = power;
	}
	Explosion(vec3_ti block, int power, bool crystal = true) {
		if (crystal) {
			this->pos = block.toVec3t().add(.5f, 1.f, .5f);
		} else {
			this->pos = block.toVec3t();
		}
		this->power = power;
	}
	Explosion(C_Entity source) {
		pos = source.aabb.centerPoint();
		switch (source.getEntityTypeId()) {
			case 65:
				power = 4;
				break;
			case 71:
				power = 6;
				break;
			case 97:
				power = 4;//???
				break;
			case 89:
			case 91:
				power = 1;
				break;
		}
	}
	inline float getExplosionDamage(C_Entity* ent, bool getReduced = false) const {
		float dist = ent->aabb.DistToAABB(pos);
		int DamageRad = 2 * power;
		float impact = (1 - (dist / DamageRad)) * getExposure(ent);
		float damage = ((impact*impact+impact) * 7 * power + 1);

		// https://minecraft.gamepedia.com/Armor/Bedrock_Edition
		if (getReduced) {
			int EPF = 0, totArm = 0;
			for (int i = 0; i < 4; i++) {
				C_ItemStack* arm = ent->getArmor(i);
				if (!arm->isValid()) continue;

				totArm += arm->getItem()->getArmorValue();

				if (arm->isEnchanted() && EPF < 25) {
#pragma warning(push)
#pragma warning(disable : 4244)
					EPF += floor(((6 + arm->getEnchantValue(0) * arm->getEnchantValue(0)) * 0.75f) / 3)
						+  floor(((6 + arm->getEnchantValue(0) * arm->getEnchantValue(0)) * 1.5f) / 3);
#pragma warning(pop)
				}
			}
			damage = damage * (totArm / 100.f);
			EPF = std::min((int)ceilf(std::min(EPF, 25) * .75f), 20);
			damage = damage * (EPF * 4) / 100.f;
		}
		return damage;
	}
	inline float getExposure(C_Entity* to) const {
		return getExposure(pos, to);
	}
	static float getExposure(vec3_t pos, C_Entity* to) {
		C_BlockSource* reg = to->region;
		auto samples = to->aabb.sort().getSamples();
		size_t a = 0; // all
		size_t h = 0; // hits
		auto blocks = getBlocks(pos, samples.back(), reg);
		for (; a < samples.size(); a++) {
			if (!isObstructed(pos, samples.at(a), blocks)) h++;
		}
		return h / (float)a;
	}
	// TODO: Calculate actual value https://minecraft.gamepedia.com/Explosion#Interaction_with_entities
	static bool isObstructed(vec3_t pos, vec3_t to, const std::vector<AABB>& block) {
		bool ret = false;
		//Ray ray = Ray(pos, to);
		auto m = block.size();
		for (size_t i = 0; i < m; i++) {
			AABB current = block.at(i);
			if (!current.slabs(pos, to) && !current.Line_AABB(pos, to))
				return true;
		}
		return ret;
	}
	static std::vector<AABB> getBlocks(vec3_t pos, vec3_t to, C_BlockSource* region) {
		std::vector<AABB> blocks;
		AABB bloks = AABB(pos.floor(), to.floor()).sort();
		for (size_t x = bloks.lower.x; x < bloks.upper.x; x++) {
			for (size_t y = bloks.lower.y; y < bloks.upper.y; y++) {
				for (size_t z = bloks.lower.z; z < bloks.upper.z; z++) {
					vec3_ti curPos = {x, y, z};
					C_Block* block = region->getBlock(curPos);
					if (block != nullptr) {
						C_BlockLegacy* legacy = block->toLegacy();
						if (legacy != nullptr) {
							if (legacy->material->isSolid || legacy->material->isLiquid) {
								AABB h = legacy->aabb;
								if (h.upper.x == h.lower.x || h.upper.y == h.lower.y || h.upper.z == h.lower.z)
									h = AABB(curPos.toVec3t(), curPos.add(1).toVec3t());
								blocks.push_back(h);
							}
						}
					}
				}
			}
		}
		return blocks;
	}
	static float getDamageMultiplied(const float& damage, int dif = 3/*-1*//*asume we are on hard until i figure out how to find that out*/) {
		int diff = dif;
		//if (diff == -1)
		//	diff = g_Data.getClientInstance()->getDifficulty();
		return damage * (diff / .5f);
	}
};
