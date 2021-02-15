#include "Freecam.h"

Freecam::Freecam() : IModule('V', Category::MISC, "Move your cam without moving the player") {
						 }
Freecam::~Freecam() {
}
const char* Freecam::getModuleName() {
	return ("Freecam");
}

void Freecam::onTick(C_GameMode* gm) {
	if (!this->isEnabled()) return;
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
	gm->player->canFly = true;
	gm->player->fallDistance = 0.f;
}

//void Freecam::onSendPacket(C_Packet* P) {
//	if (this->isEnabled()) return;
//	if (P->isInstanceOf<C_MovePlayerPacket>())
//		last = *reinterpret_cast<C_MovePlayerPacket*>(P);
//	else if (P->isInstanceOf<PlayerAuthInputPacket>())
//		Last = *reinterpret_cast<PlayerAuthInputPacket*>(P);
//}

void Freecam::onEnable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		auto p = g_Data.getLocalPlayer();
		//if (old == nullptr)
		//	old = new C_LocalPlayer(*p);
		oldPos = *p->getPos();
		//g_Data.getEntityList()->add(old);
	}
}

void Freecam::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setPos(oldPos);
		if (g_Data.getLocalPlayer()->gamemode != 1)
			g_Data.getLocalPlayer()->canFly = false;
	}
	//if (old != nullptr) {
	//	delete old;
	//	old = nullptr;
	//}
}