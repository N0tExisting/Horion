#include "HMath.h"
#include "../SDK/CBlockLegacy.h"
#include "../SDK/CEntity.h"

struct Explosion {
	int power;
	vec3_t pos;
	//C_BlockSource* region;
	Explosion(/*C_BlockSource* reg, */float x, float y, float z, int power) {
		pos = vec3_t(x, y, z);
		this->power = power;
	}
	Explosion(/*C_BlockSource* reg, */vec3_t pos, int power) {
		this->pos = pos;
		this->power = power;
	}
	Explosion(/*C_BlockSource* reg, */int x, int y, int z, int power, bool crystal = true) {
		if (crystal) {
			this->pos = vec3_ti(x, y, z).toVec3t().add(.5f);
		} else {
			this->pos = vec3_ti(x, y, z).toVec3t();
		}
		this->power = power;
	}
	Explosion(/*C_BlockSource* reg, */vec3_ti block, int power, bool crystal = true) {
		if (crystal) {
			this->pos = block.toVec3t().add(.5f);
		} else {
			this->pos = block.toVec3t();
		}
		this->power = power;
	}
	Explosion(/*C_BlockSource* reg, */C_Entity source) {
		//region = reg;
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
	float getExplosionDamage(C_Entity* ent, bool getReduced = false) const {
		float dist = ent->aabb.DistToAABB(pos);
		int DamageRad = 2 * power;
		float impact = (1 - (dist / DamageRad)) * getExposure(pos, ent);
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
	static float getExposure(vec3_t pos, C_Entity* to) {
		C_BlockSource* reg = to->region;
		// TODO: Calculate actual value https://minecraft.gamepedia.com/Explosion
		return .75f; // temp retval until i figure out how the fuck to calculate that bs somewhat efficiently
	}
	static float getDamageMultiplied(const float& damage, int dif = 3) {
		int diff = dif;
		//diff = mc.world.getDifficulty().getId();
		return damage * ((diff == 0) ? 0.0f : ((diff == 2) ? 1.0f : ((diff == 1) ? 0.5f : 1.5f)));
	}
};