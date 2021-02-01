#pragma once
#include "../Horion/Module/Modules/Module.h"
#include "../Horion/DrawUtils.h"

enum ToolType {
	_Unknown = -1,
	hand,
	picaxe,
	axe,
	shovel,
	hoe,
	sword,
	shears,
};
enum ToolLevel {
	_unknown = -2,
	noLvl = -1,
	all = 0,
	wood,
	stone,
	gold,
	iron,
	diamond,
	netherite,
};
enum SilkTouch {
	no = -1,
	maybe = 0,
	yes = 1,
};

struct BlockInfo {
public:
	std::string name;
	bool force = false;
	ToolType Type = ToolType::_Unknown;
	ToolLevel min = _unknown;
	SilkTouch silktouch = no;
	BlockInfo(C_Block*);
	std::string GetType();
	std::string GetLevel();
	//std::string GetSilkTouch();
};

class selectionHighlight : public IModule {
private:
	SettingEnum FaceH;
	SettingEnum ShowName;
	bool doOutline = false;
	bool selectRainbow = true;
	float bestPoint = .5f;
	float thickness = .3f;
	float rSelect = 1.f;
	float gSelect = 1.f;
	float bSelect = 1.f;
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
	std::string static inline GetShownName(C_Block*);
	std::string static inline GetData(C_Block* block);
	//BlockInfo inline GetInfo(C_Block* block);
};