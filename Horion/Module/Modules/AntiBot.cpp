#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0, Category::PLAYER, "Enable this module to filter bots!") {
	registerBoolSetting("Hitbox Check", &this->hitboxCheck, this->hitboxCheck);
	//registerBoolSetting("Name Check", &this->nameCheck, this->nameCheck);
	registerBoolSetting("Invisible Check", &this->invisibleCheck, this->invisibleCheck);
	registerBoolSetting("EntityID Check", &this->entityIdCheck, this->entityIdCheck);
	registerBoolSetting("Nametag Checks", &this->extraCheck, this->extraCheck);
	registerBoolSetting("Other Checks", &this->otherCheck, this->otherCheck);
	registerBoolSetting("AntiLag Check", &this->LagCheck, this->LagCheck);
}

AntiBot::~AntiBot() {
}

const char* AntiBot::getModuleName() {
	return ("AntiBot");
}
