#include"TpAura.h"
#include "../../../Utils/Logger.h"

TpAura::TpAura() : IModule(0, Category::COMBAT, "Telepors you arround while using Killaura") {
	registerFloatSetting("Range", &this->range, this->range, 1, 10);
	registerFloatSetting("Distance", &this->teleDist, this->teleDist, 1.f, 7.5f);
	registerFloatSetting("Y-offset", &this->yOffset, this->yOffset, -3.f, 2.f);
	registerIntSetting("Delay", &this->delay, this->delay, 0, 20);
}
TpAura::~TpAura() {
}
const char* TpAura::getModuleName() {
	return "TpAura";
}

void TpAura::onTick(C_GameMode* gm) {
	CheckKA();
	//try {
	aRange = __min(moduleMgr->getModule<Killaura>()->range, this->range);
	if (oDelay >= delay) {
		C_Entity* entTarget = getClosestEntity();
		if (entTarget != nullptr) {
			vec3_t* target = new vec3_t(gm->player->currentPos);
			target->x = entTarget->getPos()->x + randomFunction((rng() + .5f) * teleDist);
			target->z = entTarget->getPos()->z + randomFunction((rng() + .5f) * teleDist);
			target->y = entTarget->getPos()->y + randomFunction(yOffset) + 1;
			gm->player->setPos(*target);
			delay = 0;
		}
	}
	oDelay++;
	//} catch (std::exception e) {
	//	Logger::WriteLogFileF((volatile char*)e.what());
	//	throw e;
	//}
	
}

void TpAura::onEnable() {
	wasEnabled = moduleMgr->getModule<Killaura>()->isEnabled();
	moduleMgr->getModule<Killaura>()->setEnabled(true);
}

void TpAura::onDisable() {
	moduleMgr->getModule<Killaura>()->setEnabled(wasEnabled);
}

void TpAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	CheckKA();
}

void TpAura::CheckKA() {
	if (moduleMgr->getModule<Killaura>()->isEnabled() && this->isEnabled()) {
		moduleMgr->getModule<Killaura>()->setEnabled(true);
		clientMessageF("%sTurning of Killaura defeats the whole point of TpAura", RED);
	}
}

float TpAura::rng(float multp) {
	return multp * ((float)rand() / RAND_MAX);
}

float TpAura::randomFunction(float num) {
	if (rng() < 0.5f)
		return (-1.f * num);
	return num;
}

C_Entity* ClosestEnt = nullptr;
float minDist;
C_Entity* TpAura::getClosestEntity() {
	ClosestEnt = nullptr;
	minDist = TpAura::range;
	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (ent == g_Data.getLocalPlayer() || !Target::isValidTarget(ent))
			return;
		float dist = ent->currentPos.dist(g_Data.getLocalPlayer()->currentPos);
		if (dist < minDist) {
			ClosestEnt = ent;
			minDist = dist;
		}
	});
	return ClosestEnt;
}