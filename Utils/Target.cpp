#include "Target.h"

#include <regex>

#include "../Horion/Module/ModuleManager.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::AntiLag(C_Entity* ent) {
	int id = ent->getEntityTypeId();
	if (id == 64 || id == 66 || id == 69)
		return false;
	return true;
}

bool Target::VanillaAttac(C_Entity* ent, bool inclLeash) {
	return VanillaAttac(ent->getEntityTypeId(), inclLeash);
}

bool Target::VanillaAttac(int id, bool inclLeash) {
	if ((id == 64 || id == 65 || id == 66 || id == 67 || id == 68 || id == 69 || id == 70 || id == 72
	|| id == 73 || id == 77 || id == 79 || id == 80 || id == 81 || id == 82 || id == 83 || id == 84
	|| id == 85 || id == 86 || id == 87 || id == 88 || id == 89 || id == 90 || id == 91 || id == 93
	|| id == 94 || id == 95 || id == 96 || id == 97 || id == 98 || id == 100 || id == 101 || id == 102
	|| id == 103 || id == 104 || id == 105)|| id == 106/*Icebomb*/ || id == 117/*shield*/||(id == 88 && inclLeash))
		return false;
	return true;
}

bool Target::isValidTarget(C_Entity* ent) {
	if (ent == NULL)
		return false;
	if (ent == g_Data.getLocalPlayer())
		return false;
	if (!(*localPlayer)->canAttack(ent, false))
		return false;

	static auto antibot = moduleMgr->getModule<AntiBot>();
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	static auto teams = moduleMgr->getModule<Teams>();
	int id = ent->getEntityTypeId();

	if (!ent->isAlive())
		return false;

	// Temporarily removed from gui, tons of false negatives
	if	(!hitboxMod->isEnabled() && antibot->isHitboxCheckEnabled() &&
		(ent->height < 1.5f || ent->width < 0.49f || ent->height > 2.1f || ent->width > 0.9f))
			return false;

	if (!Target::containsOnlyASCII(ent->getNameTag()->getText()) && antibot->isNameCheckEnabled())
		return false;

	if (ent->isInvisible() && antibot->isInvisibleCheckEnabled())
		return false;

	if (id <= 122 && id != 63 && antibot->isEntityIdCheckEnabled())
		return false;

	if (antibot->isExtraCheckEnabled() && (!ent->canShowNameTag() || ent->getNameTag()->getTextLength() < 1 || std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos))
		return false;

	if ((ent->isSilent() || ent->isImmobile()) && antibot->isOtherCheckEnabled())
		return false;

	if (AntiLag(ent) && antibot->isLagCheckEnabled())
		return false;

	if (FriendList::findPlayer(ent->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled())
		return false;

	if (ent->getEntityTypeId() == 63) {
		if (teams->isColorCheckEnabled()) {
			std::string targetName = ent->getNameTag()->getText();
			std::string localName = g_Data.getLocalPlayer()->getNameTag()->getText();
			if (targetName.length() > 2 && localName.length() > 2) {
				targetName = std::regex_replace(targetName, std::regex("\\§r"), "");
				localName = std::regex_replace(localName, std::regex("\\§r"), "");
				if (targetName.at(0) == localName.at(0))
					return false;
			}
		}
		if (teams->isAlliedCheckEnabled()) {
			C_LocalPlayer* p = g_Data.getLocalPlayer();
			if (p->isAlliedTo(ent))
				return false;
		}
	}

	return true;
}

bool Target::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}
