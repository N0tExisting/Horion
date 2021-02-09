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
			this->pos = vec3_ti(x, y, z).toVec3t().add(.5f);
		} else {
			this->pos = vec3_ti(x, y, z).toVec3t();
		}
		this->power = power;
	}
	Explosion(vec3_ti block, int power, bool crystal = true) {
		if (crystal) {
			this->pos = block.toVec3t().add(.5f);
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
		auto samples = to->aabb.getSamples();
		size_t a = 0; // all
		size_t t = 0; // hits
		for (; a < samples.size(); a++) {
			if (isUnobstructed(pos, samples.at(a), reg)) t++;
		}
		return t / (float)a;
	}
	// TODO: Calculate actual value https://minecraft.gamepedia.com/Explosion#Interaction_with_entities
	static bool isUnobstructed(vec3_t pos, vec3_t to, C_BlockSource* region) {
		AABB bloks;
		return true;
	}
	static float getDamageMultiplied(const float& damage, int dif = 3/*-1*//*asume we are on hard until i figure out how to find that out*/) {
		int diff = dif;
		//if (diff == -1)
		//	diff = g_Data.getClientInstance()->getDifficulty();
		return damage * (diff / .5f);
	}
};
