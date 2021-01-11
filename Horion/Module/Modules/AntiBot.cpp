#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0, Category::PLAYER, "Enable this module to filter bots!") {
	registerBoolSetting("Hitbox Check", &this->hitboxCheck, this->hitboxCheck);
	//registerBoolSetting("Name Check", &this->nameCheck, this->nameCheck);
	registerBoolSetting("Invisible Check", &this->invisibleCheck, this->invisibleCheck);
	registerBoolSetting("EntityID Check", &this->entityIdCheck, this->entityIdCheck);
	registerBoolSetting("Nametag Checks", &this->NametagCheck, this->NametagCheck);
	registerBoolSetting("Newline Check", &this->NewlineCheck, this->NewlineCheck);
	registerBoolSetting("Text Check", &this->TextCheck, this->TextCheck);
	registerBoolSetting("silent Check", &this->silentCheck, this->silentCheck);
	registerBoolSetting("Imobile Check", &this->ImobileCheck, this->ImobileCheck);
	registerBoolSetting("AntiLag Check", &this->LagCheck, this->LagCheck);
}
AntiBot::~AntiBot() {
}
const char* AntiBot::getModuleName() {
	return ("AntiBot");
}