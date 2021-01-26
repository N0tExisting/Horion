#pragma once
#include "../Horion/Module/Modules/Module.h"

class HudModule : public IModule {
public:
	HudModule();
	~HudModule();
	bool tabgui = false;
	bool arraylist = true;
	bool clickToggle = true;
	bool watermark = true;
	bool coordinates = true;
	bool keybinds = true;
	bool displayArmor = true;
	bool keystrokes = false;
	bool fps = true;
	bool cps = false;
	bool alwaysShow = false;
	bool welcome = true;
	bool invRender = true;
	bool doList = true;
	float xVal = 28.f;
	//bool editHud = false;
	float scale = 1.f;
	bool pvpR = true;


	// Inherited via IModule
	virtual bool isThereCrystal();
	virtual bool isThereGapples();
	virtual bool isThereTotem();
	virtual bool isThereXp();
	int pullSlot(int itemIDPog);
	virtual int countItem(int itemIDCount);
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable();
	virtual void onTick(C_GameMode* gm) override;
};
