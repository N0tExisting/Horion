#pragma once
#include "Module.h"
#include "../../../include/Discord/cpp/discord.h"

class DiscordRPC : public IModule {
private:
	bool Server = false;

	discord::Core* core;
	discord::Activity activity{};
	discord::ImageHandle Handle{};
	discord::ActivityTimestamps Timestamps;
	discord::ActivityAssets Assets;

	void UpdateRPC();
public:
	DiscordRPC();
	~DiscordRPC();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	
	std::string scene;
};