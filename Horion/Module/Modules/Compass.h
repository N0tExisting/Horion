#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
class Compass : public IModule {
private:
	float opacity = 0.5;
	int range = 135;
	bool showWaypoints = true;

public:
	Compass();
	~Compass();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;

	void drawCenteredText(vec2_t pos, std::string text, float size = 1.f, float textOpacity = 1.f, MC_Color col = MC_Color(255, 255, 255));
};
