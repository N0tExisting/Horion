#include "ESP.h"

#include "../../../Utils/Target.h"

ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you") {
	this->registerBoolSetting("rainbow", &this->doRainbow, this->doRainbow);
	this->registerBoolSetting("Mobs", &this->isMobEsp, this->isMobEsp);
	this->registerBoolSetting("2d", &this->is2d, this->is2d);
	//this->registerBoolSetting("Show invis", &this->showInvis, this->showInvis);
}
ESP::~ESP() {
}
const char* ESP::getModuleName() {
	return ("Hitboxes");
}

static float rcolors[4];

void doRenderStuff(C_Entity* ent, bool isRegularEntitie) {
	static auto espMod = moduleMgr->getModule<ESP>();
	
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (ent != localPlayer) {
		if (ent->timeSinceDeath > 0)
			return;
		float a = (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1)));
		if (ent->isInvisible() && !espMod->showInvis)
			a = 0;
		else if (ent->isInvisible())
			a = (float)fmax(0.1f, (float)fmin(0.5f, 15 / (ent->damageTime + 1)));
		if (FriendList::findPlayer(ent->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled()) {
			DrawUtils::setColor(0.1f, 0.9f, 0.1f, a);
		} else if (Target::isValidTarget(ent)) {
			if (espMod->doRainbow)
				DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], a);
			else
				DrawUtils::setColor(0.9f, 0.9f, 0.9f, a);
		} else if (espMod->isMobEsp) {
			if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
				return;
			else if (!g_Data.getLocalPlayer()->canAttack(ent, false))
				DrawUtils::setColor(0.9f, 0.2f, 0.2f, a);
			else
				DrawUtils::setColor(0.2f, 0.2f, 0.9f, a);
		} else
			return;
		if (espMod->is2d)
			DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, (*localPlayer->getPos()).dist(*ent->getPos()) * 3.f)));
		else 
			DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, (*localPlayer->getPos()).dist(*ent->getPos()))));
	}
	
}

void ESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() != nullptr && GameData::canUseMoveKeys()) {

		// Rainbow colors
		if (rcolors[3] < 1) {
			rcolors[0] = 0.2f;
			rcolors[1] = 0.2f;
			rcolors[2] = 1.f;
			rcolors[3] = 1;
		}
		Utils::ApplyRainbow(rcolors, 0.0015f);

		g_Data.forEachEntity(doRenderStuff);
	}
}
