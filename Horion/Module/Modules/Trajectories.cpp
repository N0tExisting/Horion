#include "Trajectories.h"
#include "../../../Utils/Logger.h"
#include "../../../SDK/CCamera.h"

#pragma warning(push)
#pragma warning(disable : 4244)

#define SkipTicks (skipTicks+1)

// This needs to be here else an error gets thrown
inline float DegToRad(float degrees) {
	return DEG_RAD * degrees;
}
inline float RadToDeg(float radians) {
	return RAD_DEG * radians;
}

// I need to make sure this is right
inline float pow(float val, int pow) {
	for (int i = 0; i < pow; i++)
		val *= val;
	return val;
}
//inline int pow(int val, int pow) {
//	for (size_t i = 0; i < pow; i++)
//		val *= val;
//	return val;
//}


Trajectories::Trajectories() : IModule(0, Category::VISUAL, "Shows trajectories of projectiles") {
	registerBoolSetting("Flying Projetiles", &this->other, this->other);
	registerIntSetting("Calculate Ticks", &this->prevTicks, prevTicks, 10, 300);
	registerIntSetting("Skips Ticks", &this->skipTicks, this->skipTicks, 0, 20);
	PullStreght.AddEntry(GradientEntry(MC_Color(255, 255, 0), .5f));
}
const char* Trajectories::getModuleName() {
	return "Trajectories";
}

C_Entity* closest = nullptr;
void Trajectories::onTick(C_GameMode* gm) {
	if (GameData::isRightClickDown())
		clicks++;
	else {
		if (clicks != 0) {
			auto p = g_Data.getLocalPlayer();
			logF("Pitch: %.3f, Yaw: %.3f, Body Yaw: %.3f", p->pitch, p->yaw, p->bodyYaw);
			closest = nullptr;
			auto eye = p->eyePos0;
			g_Data.forEachEntity([eye, p](C_Entity* ent, bool) {
				if (ent == p) return;
				if (closest == nullptr)
					closest = ent;
				else if (ent->aabb.centerPoint().dist(eye) < closest->aabb.centerPoint().dist(eye))
					closest = ent;
			});
			if (closest != nullptr)
				Logger::WriteLogFileF((volatile char*)"%i %.3f(%.3f)\n%.3fx%.3f", clicks, closest->velocity.magnitude(),
					closest->currentPos.sub(closest->oldPos).magnitude(), closest->width, closest->height);
		} else if (closest != nullptr) {
			Logger::WriteLogFileF((volatile char*)"%.3f(%.3f)", closest->velocity.magnitude(),
				closest->currentPos.sub(closest->oldPos).magnitude());
			closest = nullptr;
		}
		clicks = 0;
	}
}
#undef closest // needs to be modifiable in the Lambada

void Trajectories::onLevelRender() {
	if (other) {
		//std::vector<long long> noDel = std::vector<long long>();
		g_Data.forEachEntity([this/*, noDel*/](C_Entity* ent, bool) {
			if (isProj(ent, true)) {
				//noDel.push_back((long long)ent->entityRuntimeId);
				if (/*!Trajs.count(ent->entityRuntimeId*/true) {
					vec3_t drawP = ent->aabb.centerPoint();
					vec3_t calcVel = ent->currentPos.sub(ent->oldPos);
					vec3_t vel = calcVel.iszero() || ent->oldPos.iszero() ? calcVel : ent->velocity;
					std::vector<vec3_t> positions;
					//C_BlockSource* region = g_Data.getLocalPlayer()->region;
					positions.push_back(drawP);
					for (int i = 0; i < prevTicks; i += SkipTicks) {
							if (ent->getEntityTypeId() == 73 || ent->getEntityTypeId() == 80)
								vel.y -= getUpVelOnT(vel.y, SkipTicks, ProjAccel, Proj_Drag);
							else
								vel.y = getUpVelOnT(vel.y, SkipTicks, EggAccel, Egg_Drag);
						drawP = drawP.add(vel.mul(SkipTicks));
						positions.push_back(drawP);
						//AABB cur = AABB(ent->width, ent->height, drawP);
						//AABB checkRange = cur.sort();
						//for (size_t x = 0; x < length; x++) {
						//	if (cur.colides thing) goto done:
						//}
					}
					//done:
					DrawUtils::drawLinestrip3d(positions);
				}
				//std::sort(noDel.begin(), noDel.end());
				//for (auto it = Trajs.cbegin(), next_it = it; it != Trajs.cend(); it = next_it) {
				//	++next_it;
				//	if (it->first) {
				//		m.erase(it);
				//	}
				//}
			}
		});
	}
	auto col = PullStreght.GetColor(clicks / 20);
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	if (couldThrow()) {
		auto cameraMgr = g_Data.getClientInstance()->getCameraManager();
		if (cameraMgr == nullptr)
			return;
		auto cam = cameraMgr->getCameraOrDebugCamera();
		if (cam == nullptr)
			return;
		vec3_t forward{};
		cam->getForwardVector(&forward);
		vec3_t drawP = g_Data.getClientInstance()->levelRenderer->origin.add(forward.normalize().mul(0.2f)); //place the start of the line slightly forward so it won't get clipped

		vec3_t vel = GetVel();
		if (vel.iszero()) return;
		std::vector<vec3_t> positions;
		//C_BlockSource* region = g_Data.getLocalPlayer()->region;
		positions.push_back(drawP);
		for (int i = 0; i < prevTicks; i += SkipTicks) {
			drawP = drawP.add(vel.mul(SkipTicks));
			positions.push_back(drawP);

			AABB cur = AABB(0.25f, 0.25f, drawP);
			AABB checkRange = cur.sort();
			bool colides = false;
			if (colides) goto done;

			if (isBow())
				vel.y = getUpVelOnT(vel.y, SkipTicks, ProjAccel, Proj_Drag);
			else
				vel.y = getUpVelOnT(vel.y, SkipTicks, EggAccel, Egg_Drag);
			//vel = vel.mul(.99);
		}
		done:
		DrawUtils::drawLinestrip3d(positions);
	}
}

inline float Trajectories::getUpVelOnT(float upVel, int tick, float accel, float drag) {
	return ((1 - drag) * upVel) - (accel * ((1 - pow(1 - drag, tick)) / drag));
}

vec3_t Trajectories::GetVel() {
	auto player = g_Data.getLocalPlayer();
	//vec2_t rot = {DegToRad(player->viewAngles.x), DegToRad(player->viewAngles.y)};
	auto cameraMgr = g_Data.getClientInstance()->getCameraManager();
	if (cameraMgr == nullptr)
		return vec3_t();
	auto cam = cameraMgr->getCameraOrDebugCamera();
	if (cam == nullptr)
		return vec3_t();
	vec3_t forward{};
	cam->getForwardVector(&forward);
	int id = player->getSelectedItemId();
	static int log = 0;
	log++;
	if (id == 261) {
		float vel = NAN;
		if (clicks > 2)
			vel = 0;
		if (clicks > 4)
			vel = .5f;
		if (log % 20 == 0)
			logF("%.3f", vel);
		return vec3_t();
	} else if (id == 332 || id == 344) {
		float vel = 1.52f;
		if (log % 20 == 0)
			logF("%.3f", vel);
		//vec3_t forward = vec3_t{
		//	sinf(rot.x) * cos(rot.y),
		//	sinf(rot.x) * cos(rot.y),
		//	cosf(rot.y)
		//}.normalize().mul(vel); // i think this is already normalized but whatever
		return forward.normalize().mul(vel);
	
	}
	return vec3_t(0.f, 0.f, 0.f);
}

bool Trajectories::isBow() {
	int id = g_Data.getLocalPlayer()->getSelectedItemId(); // Crossbow id: 472
	return id == 261 || id == 455;
}

bool Trajectories::couldThrow() {
	int id = g_Data.getLocalPlayer()->getSelectedItemId();
	return (isBow() && clicks > 1) || id == 332 || id == 344 || id == 368 || id == 384 || id == 438 || id == 451;
}

inline bool Trajectories::isProj(C_Entity* e, bool other) {
	int id = e->getEntityTypeId();
	return id == 68 || id == 73 || id == 80 || id == 81 || id == 82 || id == 86 || id == 87 || id == 101 ||
		   ((id == 79 || id == 85 || id == 102) && other);
}
//void Trajectories::onPreRender(C_MinecraftUIRenderContext* renderCtx) {}

#pragma warning(pop)