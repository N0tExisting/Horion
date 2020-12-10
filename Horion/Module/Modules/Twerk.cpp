#include "Twerk.h"

Twerk::Twerk() : IModule(0, Category::PLAYER, "Crouches up and down") {
	this->registerIntSetting("up time", &this->uDelay, this->uDelay, 1, 20);
	this->registerIntSetting("down time", &this->dDelay, this->dDelay, 1, 20);
}
Twerk::~Twerk() {
}
const char* Twerk::getModuleName() {
	return ("Twerk");
}

void Twerk::onEnable() {
	if (Pl == nullptr)
		Pl = g_Data.getClientInstance()->getMoveTurnInput();
}

void Twerk::onTick(C_GameMode* gm) {
	Odelay++;
	if (Odelay >= dDelay && !shouldSneak){
		shouldSneak = true;
		Odelay = 0;
	}
	if (Odelay >= uDelay && shouldSneak) {
		shouldSneak = false;
		Odelay = 0;
	}
	if (Pl->isSneakDown != shouldSneak)
		Pl->isSneakDown = shouldSneak;
}

void Twerk::onDisable() {
	Pl->isSneakDown = false;
}