#include "Jesus.h"
#include "../../../Utils/Logger.h"
Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Allows you to walk on water and lava. Also you shouldn't be on fire") {
	registerBoolSetting("Solid", &this->Solid, Solid);
}
Jesus::~Jesus() {
}
const char* Jesus::getModuleName() {
	return "Jesus";
}

int jesusChrist = 0;

void Jesus::onTick(C_GameMode* gm) {
	float sub1 = 0.62f;
	float add2 = 0.495f;
	if (gm->player->isSneaking())
		return;
	if (Solid) {
		if (gm->player->isOnFire()) {
			gm->player->setOnFire(0);
			g_Data.getLocalPlayer()->setOnFire(0);
		}
		if (gm->player->isOnFire() && (gm->player->gamemode == 0 || gm->player->gamemode == 2) && !gm->player->isFireImmune())
			return;
		vec3_t pos = *gm->player->getPos();
		vec3_ti _pos1 = {(int)pos.x, (int)pos.y - 1, (int)pos.z};
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(_pos1);
		if (gm->player->isInLava())
			add2 = 0.62f;
		else if ((*block->blockLegacy)->blockId == 10)
			add2 = 0.62f;
		else if ((*block->blockLegacy)->blockId == 11)
			add2 = 0.62f;
		AABB ColShape;
		(*block->blockLegacy)->getCollisionShape(&ColShape, block, g_Data.getLocalPlayer()->region, &_pos1, gm->player);
		if (gm->player->aabb.intersects(ColShape))  // inside of Block
			gm->player->setPos({pos.x, pos.y + .5f, pos.z});
		//clientMessageF("%s", (float)((1.0 / 8.0) * ((double)(*block->blockLegacy)->liquidGetDepth(gm->player->region, &_pos1))));
		pos.y -= 1.62f;

		pos.z = gm->player->aabb.upper.z;
		pos.x = gm->player->aabb.upper.x;  // upper upper
		
		block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(pos));
		C_BlockLegacy* blockLegacy = *(block->blockLegacy);

		if (blockLegacy->material->isLiquid && gm->player->velocity.y <= 0) {
			vec3_t pos = *gm->player->getPos();
			pos.y -= sub1;
			pos.y = ceilf(pos.y);
			pos.y += add2;
			gm->player->setPos(pos);
			gm->player->onGround = true;
			gm->player->velocity.y = 0.f;
		}

		pos.x = gm->player->aabb.lower.x;
		pos.z = gm->player->aabb.lower.z;  // lower lower

		block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(pos));
		blockLegacy = *(block->blockLegacy);

		if (blockLegacy->material->isLiquid && gm->player->velocity.y <= 0) {
			vec3_t pos = *gm->player->getPos();
			pos.y -= sub1;
			pos.y = ceilf(pos.y);
			pos.y += add2;
			gm->player->setPos(pos);
			gm->player->onGround = true;
			gm->player->velocity.y = 0.f;
		}

		pos.x = gm->player->aabb.upper.x;  // upper x and lower z

		block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(pos));
		blockLegacy = *(block->blockLegacy);

		if (blockLegacy->material->isLiquid && gm->player->velocity.y <= 0) {
			vec3_t pos = *gm->player->getPos();
			pos.y -= sub1;
			pos.y = ceilf(pos.y);
			pos.y += add2;
			gm->player->setPos(pos);
			gm->player->onGround = true;
			gm->player->velocity.y = 0.f;
		}

		pos.x = gm->player->aabb.lower.x;
		pos.z = gm->player->aabb.upper.z;  // lower x and upper z

		block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(pos));
		blockLegacy = *(block->blockLegacy);

		if (blockLegacy->material->isLiquid && gm->player->velocity.y <= 0) {
			vec3_t pos = *gm->player->getPos();
			pos.y -= sub1;
			pos.y = ceilf(pos.y);
			pos.y += add2;
			gm->player->setPos(pos);
			gm->player->onGround = true;
			gm->player->velocity.y = 0.f;
		}

		if (gm->player->isInWater() || gm->player->isInLava()) {
			gm->player->velocity.y = 0.1f;
			gm->player->onGround = true;
		}
	} else {
		if (gm->player->hasEnteredWater()) {
			gm->player->velocity.y = 0.06f;
			gm->player->onGround = true;
			wasInWater = true;
		} else if (gm->player->isInWater() || gm->player->isInLava()) {
			gm->player->velocity.y = 0.1f;
			gm->player->onGround = true;
			wasInWater = true;
		} else if (wasInWater) {
			wasInWater = false;
			gm->player->velocity.x *= 1.2f;
			gm->player->velocity.x *= 1.2f;
		}
	}
}