#pragma once
#include "Module.h"

class AutoGapple : public IModule {
private:
	int currHealth = 20;
	int slot = 0;
	int prevSlot = 0;
	int health = 10;
	bool SmolEqual = true;
	bool PrefEnchant = true;
	bool eating = false;
	int PrepareEat();
	void Eat();

public:
	C_MoveInputHandler* inputHandler = nullptr;
	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	void onDisable() override;
	virtual void onEnable() override;
};
