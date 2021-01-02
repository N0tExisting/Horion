#pragma once
#include "../ModuleManager.h"
#include"Module.h"

class TpAura : public IModule {
private:
	int delay = 4;
	int oDelay = 0;
	float range = 4.25f;
	float aRange = 5.f;
	float yOffset = .75f;
	float teleDist = 4.1f;
	bool wasEnabled;

public:
	TpAura();
	~TpAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;

	C_Entity* getClosestEntity();
	float rng(float multpl = 1.f);
	float randomFunction(float num);
	void CheckKA();
};