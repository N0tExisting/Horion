#pragma once
#include "Module.h"

class AutoGapple : public IModule {
private:
	int health = 10;
	bool SmolEqual = true;
	bool PrefEnchant = true;
	bool Selected = false;
	bool CheckInv = false;
	int currHealth = 20;
	void eat();

public:
	C_MoveInputHandler* inputHandler = nullptr;
	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
