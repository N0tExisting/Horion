#pragma once
#pragma once
#include "../Horion/Module/ModuleManager.h"
#include "../Horion/Module/Modules/Module.h"
class selectionHighlight : public IModule {
private:
	bool selectRainbow = true;
	float thickness = .3f;
	float rSelect = 1.f;
	float gSelect = 1.f;
	float bSelect = 1.f;
	//bool faceH = true;
	SettingEnum FaceH;
	bool doOutline = true;
	float opacityVal = 1.f;
	float fOpacity = 0.5f;

public:
	selectionHighlight();
	~selectionHighlight();

	// Inherited via IModule
	//virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};