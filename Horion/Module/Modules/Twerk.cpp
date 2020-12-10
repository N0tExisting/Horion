#include "Twerk.h"

/// <summary>
/// Load after AutoSneak
/// </summary>
Twerk::Twerk() : IModule(0, Category::COMBAT, "Crouches up and down") {
	this->registerIntSetting("up time", &this->uDelay, this->uDelay, 1, 20);
	this->registerIntSetting("down time", &this->dDelay, this->dDelay, 1, 20);
	sneak = moduleMgr->getModule<AutoSneak>();
}
Twerk::~Twerk() {
}

const char* Twerk::getModuleName() {
	return ("Twerk");
}

void Twerk::onEnable() {
	WasSneaking = sneak->isEnabled();
}

void Twerk::onTick(C_GameMode* gm) {
	Odelay++;
	if (sneak->isEnabled()) {
		if (Odelay >= dDelay) {
			sneak->setEnabled(false);
			Odelay = 0;
		}
	} else {
		if (Odelay >= uDelay) {
			sneak->setEnabled(false);
			Odelay = 0;
		}
	}
}

void Twerk::onDisable() {
	sneak->setEnabled(WasSneaking);
}