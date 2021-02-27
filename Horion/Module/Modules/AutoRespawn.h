#pragma once
#include "Module.h"


class AutoRespawn : public IModule {
private:
	int oDelay = 0;
	int delay = 10;

public:

	AutoRespawn();
	~AutoRespawn();
	
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode*) override;
};