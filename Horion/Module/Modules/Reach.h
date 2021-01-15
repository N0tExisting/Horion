#pragma once
#include "Module.h"

class Reach : public IModule {
private:
	float reachValue = 4.1f;
	float originalReach = 0;
	float* reachPtr = nullptr;
	unsigned long oldProtect = 0;

public:
	Reach();
	~Reach();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;

	inline float GetCurrentReach() {
		if (reachPtr != nullptr)
			return *reachPtr;
		return 4.f;
	}
};
