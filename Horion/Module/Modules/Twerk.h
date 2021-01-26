#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class Twerk : public IModule {
private:
	int dDelay = 2;
	int uDelay = 3;
	int Odelay = 0;
	bool shouldSneak;
	C_MoveInputHandler* Pl = nullptr;

public:
	Twerk();
	~Twerk();

	// Inherited via IModule (override)
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};