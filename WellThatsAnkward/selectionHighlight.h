#pragma once
#include "../Horion/Module/Modules/Module.h"

class selectionHighlight : public IModule {
private:
	SettingEnum FaceH;
	bool ShowName;
	//bool faceH = true;
	bool doOutline = false;
	//bool InfoBackround = true;
	bool selectRainbow = true;
	float thickness = .3f;
	float rSelect = 1.f;
	float gSelect = 1.f;
	float bSelect = 1.f;
	//float yOffset = 0.f;
	float opacityVal = 1.f;
	float fOpacity = 0.15f;
	float baOpacity = 0.f;

public:
	selectionHighlight();
	~selectionHighlight();

	// Inherited via IModule
	//virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	std::string inline GetShownName(C_Block*);
	std::string inline GetInfo(C_Block* block);
};