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
	switch (id) {
		case 64: // Item
		case 65: // TNT
		case 66: // Falling Block
		case 67: // Moving Block (piston)
		case 68: // Thrown XP Bottle
		case 69: // xp orb
		case 70: // Thrown Eye of Ender
		case 72: // Fireworks Rocket
		case 73: // Thown Trident
		case 77: // Fishing Bobber
		case 79: // Dragon Fireball
		case 80: // Arrow
		case 81: // Thrown Snowball
		case 82: // Thrown Egg
		case 86: // Thrown splash Potion
		case 87: // Thrown Ender Pearl
		case 88: // Leash knot
			if (!inclLeash)
				return;
		case 89: // Wither skull
		case 91: // Blue Wither skull
		case 93: // Lightning Bolt
		case 94: // Blaze fireball
		case 95: // Area Effect Cloud
		case 101:// Thrown Lingering potion
		case 102:// Lama Spit
		case 103:// Evocation Fangs
		case 106:// Icebomb
		//case 107:// Ballon
		case 117:// Shield
			return false;
		default:
			return true;
	}
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
	// name tag Checks
	if (antibot->isNametagCheckEnabled() && !ent->canShowNameTag())
		return false;

	if (antibot->isNewlineCheckEnabled() && std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos)
		return false;

	if (antibot->isTextCheckEnabled() && ent->getNameTag()->getTextLength() < 1)
		return false;
	// other Checks
	if (ent->isSilent() && antibot->isSilentCheckEnabled())
		return false;

	if (ent->isImmobile() && antibot->isImobileCheckEnabled())
		return false;

	if (!AntiLag(ent) && antibot->isLagCheckEnabled())
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
