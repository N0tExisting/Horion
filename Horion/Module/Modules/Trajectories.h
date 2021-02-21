#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
//#include "../ModuleManager.h"

#define Egg_Drag .01f
#define EggAccel .03f
#define Proj_Drag .01f
#define ProjAccel .05f

class Trajectories : public IModule {
private:
	bool other = true;
	int clicks = 0; // Right click ticks
	int prevTicks = 200;
	int skipTicks = 5;
	Gradient PullStreght = Gradient(MC_Color(255, 0, 0), MC_Color(0, 255, 0));
	//std::map<long long, std::vector<vec3_t>> Trajs;

public:
	Trajectories();
	~Trajectories(){};

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onLevelRender() override;
	//virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;

	float getUpVelOnT(float upVel, int tick, float accel, float drag);
	vec3_t GetVel();
	bool couldThrow();
	bool isBow();
	bool isProj(C_Entity* e, bool other = false);
	//inline bool getOther(){return other;}
};