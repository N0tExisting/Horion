#include "DiscordRPC.h"

DiscordRPC::DiscordRPC() : IModule(0, Category::MISC, "Enables Discord Rich Presence") {
	//discord::CreateFlags::NoRequireDiscord
	auto result = discord::Core::Create(797072627134496778, DiscordCreateFlags_NoRequireDiscord, &core);
	activity.SetState("Loading");
	activity.SetType(discord::ActivityType::Playing);
	//activity.SetDetails("Fruit Loops");
	Timestamps.SetStart(1507665886);
	Assets.SetLargeImage("icon");
	Assets.SetLargeText("HORE0N");
	Assets.SetSmallImage("mcgrass");
	Assets.SetSmallText("Minecraft Bedrock");
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
	//core->ImageManager().Fetch()
}
DiscordRPC::~DiscordRPC() {
	core->~Core();
}
const char* DiscordRPC::getModuleName() {
	return "Discord";
}

void DiscordRPC::onTick(C_GameMode* gm) {
	UpdateRPC();
}

void DiscordRPC::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	UpdateRPC();
}

void DiscordRPC::UpdateRPC() {
	activity.SetState(scene.c_str());
	if (Server && g_Data.isInGame() && g_Data.getRakNetInstance()->serverIp.getTextLength() > 1) {
		char server[128]{};
		sprintf_s(server, 128, "%s : %i",
			g_Data.getRakNetInstance()->serverIp.getText(),
			g_Data.getRakNetInstance()->getPort());
		activity.SetDetails(server);
	} else
		activity.SetDetails("");
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}