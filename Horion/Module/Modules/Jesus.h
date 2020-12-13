#pragma once
#include "Module.h"
class Jesus : public IModule {
private:
	bool wasInWater = false;
	bool Solid = true;

public:
	Jesus();
	~Jesus();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

