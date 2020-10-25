#include "AutoGapple.h"

int currHealth = 20;

AutoGapple::AutoGapple() : IModule(0x0, Category::PLAYER, "Auto eat gapples if you're low health") {
	registerIntSetting("health", &this->health, 10, 1, 20);
	registerBoolSetting("health", &this->SmolEqual, true);
	registerBoolSetting("Eat stuf outside Hotbar", &this->NotInHotbar, false);
	//Get player health offset(0x??...??C4)
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

void AutoGapple::onTick(C_GameMode* gm) {
	//replace 20 w/ value from Memory(0x??...??C4)
	currHealth == 20;
	if (SmolEqual) {
		if (health >= currHealth)
			eat();
	} else if (health > currHealth)
		eat();
;}

//eat a gapple
void eat() {
	//loop Hotbar
	if (AutoGapple::NotInHotbar) {
	}
	//? log Couldnt find gapple
}
