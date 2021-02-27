#include "AutoRespawn.h"
#include "../../../Utils/Logger.h"

AutoRespawn::AutoRespawn() : IModule(0, Category::PLAYER, "Automaticaly respawns you after a set amount of ticks") {
	registerIntSetting("Delay", &this->delay, this->delay, 0, 200);
}
AutoRespawn::~AutoRespawn() {
}
const char* AutoRespawn::getModuleName() {
	return "Auto Respawn";
}

void AutoRespawn::onTick(C_GameMode*) {
	C_LocalPlayer* Pl = g_Data.getLocalPlayer();
	if (Pl == nullptr) return;
	if (Pl->timeSinceDeath < 0 || !Pl->isAlive()) {
		if (delay <= oDelay) {
			logF("%i", *((unsigned char*)Pl->respawn()));
			logF("%i", *((unsigned char*)Pl->resetPos(false)));
			logF("%i", *((unsigned char*)Pl->resetRot()));
			oDelay = 0;
		} else oDelay++;
	} else
			oDelay = 0;
}