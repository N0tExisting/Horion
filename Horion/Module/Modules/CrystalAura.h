#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class CrystalAura : public IModule {
private:
	int delay = 0;
	bool autoplace = false;

public:
	CrystalAura();
	~CrystalAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int range = 10;
};
