#pragma once
#include "Module.h"

class AutoMLG : public IModule {
private:
	float MLGdist = 5;
	bool inv = true;
	bool horn = false;
	bool tryMLG(vec3_t block);
	short isMLG(C_ItemStack*);
	bool findItem();
	int findSlot();

public:
	AutoMLG();
	~AutoMLG();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};