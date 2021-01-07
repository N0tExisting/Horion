#pragma once

#include "Module.h"
#include "../ModuleManager.h"

class ESP : public IModule {
public:
	bool isMobEsp = true;
	bool doRainbow = true;
	bool is2d = false;
	bool showInvis = false;
	//bool 

	ESP();
	~ESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
