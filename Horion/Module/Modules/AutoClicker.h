#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class AutoClicker : public IModule {
private:
	int delay = 4;
	int Odelay = 0;
	bool weapons = false;
	bool rightclick = false;
	bool hold = false;

public:
	AutoClicker();
	~AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
