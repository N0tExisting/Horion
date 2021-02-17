#pragma once
#include "Module.h"

class AutoGapple : public IModule {
private:
	int slot = -1;
	int health = 10;
	int lagComp = 1;
	int prevSlot = -1;
	int timeEating = 0;
	int currHealth = 20;
	int eatingTime = 100;
	bool shouldDisable = false;
	bool PrefEnchant = true;
	bool wasClick = false;
	bool eating = false;
	C_PlayerInventoryProxy* supplies = nullptr;
	C_Inventory* inv = nullptr;
	C_ItemStack* stack = nullptr;
	C_HIDController* click = nullptr;

	void PrepareEat();
	void Eat();

public:
	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	
	void preTick(C_GameMode* gm);
};