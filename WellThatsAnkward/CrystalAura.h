#pragma once
#include "../Horion/Module/ModuleManager.h"
#include "../Horion/Module/Modules/Module.h"

class CrystalAura2 : public IModule {
private:
	int delay = 0;
	bool autoplace = true;
	bool crystalCheck = true;
	bool yLock = false;
	bool isClick = false;
	bool doMultiple = true;

public:
	CrystalAura2();
	~CrystalAura2();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int range = 5;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	vec3_t espPosUpper;
	vec3_t espPosLower;
};
