#include "selectionHighlight.h"
#include "../Utils/Target.h"
#include "../Utils/Logger.h"
#include "../Memory/Hooks.h"

selectionHighlight::selectionHighlight() : IModule(0x0, Category::VISUAL, "Custom selection box!") {
	FaceH.addEntry(EnumEntry("None", 0)).addEntry(EnumEntry("Selected", 1)).addEntry(EnumEntry("All", 3));
	//ShowName.addEntry(EnumEntry("No", 0)).addEntry(EnumEntry("Yes", 1)).addEntry(EnumEntry("w/ Backround", 2));
	registerBoolSetting("Block Info", &this->ShowName, this->ShowName);
	registerFloatSetting("Backround opacity", &this->baOpacity, this->baOpacity, 0.f, 1.f);
	registerEnumSetting("Highlight Faces", &FaceH, 1);
	registerFloatSetting("Face Opacity", &this->fOpacity, this->fOpacity, 0.05f, 1.f);
	registerFloatSetting("Thickness", &this->thickness, this->thickness, 0.05f, 1.f);
	registerFloatSetting("Opacity", &this->opacityVal, this->opacityVal, 0.05f, 1.f);
	registerBoolSetting("Outline", &this->doOutline, this->doOutline);
	registerBoolSetting("Rainbow", &this->selectRainbow, this->selectRainbow);
	registerFloatSetting("Red", &this->rSelect, this->rSelect, 0.f, 1.f);
	registerFloatSetting("Green", &this->gSelect, this->gSelect, 0.f, 1.f);
	registerFloatSetting("Blue", &this->bSelect, this->bSelect, 0.f, 1.f);
	//registerBoolSetting("FaceHighlight", &this->faceH, this->faceH);
}

selectionHighlight::~selectionHighlight() {
}

const char* selectionHighlight::getModuleName() {
	return ("Highlight Selection");
}

//void selectionHighlight::onEnable() {
//	if ()
//}

void selectionHighlight::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	//logF("%s", g_Hooks.GetScreenName());
	static float rainbowColors[4] = {0.2f, 0.2f, 1.f, opacityVal};
	Utils::ApplyRainbow(rainbowColors, 0.0015f);
	if (g_Data.getClientInstance() == nullptr ||
		g_Data.getPtrLocalPlayer() == nullptr ||
		g_Data.getLocalPlayer() == nullptr ||
		!GameData::canUseMoveKeys() ||
		!g_Data.isInGame())
		return;
	float Color[4] = {rSelect, gSelect, bSelect, opacityVal};
	if (selectRainbow) {
		Color[0] = rainbowColors[0];
		Color[1] = rainbowColors[1];
		Color[2] = rainbowColors[2];
	}
	PointingStruct* ptr = g_Data.getClientInstance()->getPointerStruct();
	if (ptr == nullptr) return;
	if (ptr->entityPtr == nullptr && ptr->rayHitType == 0) {
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(ptr->block);
		AABB h = AABB(ptr->block.toVec3t(), ptr->block.add(1).toVec3t());
		// outline
		DrawUtils::setColor(Color[0], Color[1], Color[2], opacityVal);
		//if (doOutline) {
		//	vec3_t angles[3] = {
		//		vec3_t(ptr->rayHitVec.x, 0.f, 0.f),
		//		vec3_t(0.f, ptr->rayHitVec.y, 0.f),
		//		vec3_t(0.f, 0.f, ptr->rayHitVec.z)
		//	};
		//	g_Data.getLocalPlayer()->region->getBlock(angles[0])->toLegacy()->blockId == 0
		//} else {
		DrawUtils::drawBox(ptr->block.toVec3t(), ptr->block.add(1).toVec3t(), thickness, doOutline);
		//}
		// Faces
		if (selectRainbow)
			Utils::ApplyRainbow(Color, 0.5f);
		MC_Color col = MC_Color(Color[0], Color[1], Color[2], fOpacity);
		switch (FaceH.GetEntry()->GetValue()) {
			case 0:
				break;
			case 1:
				DrawUtils::drawAABB(h, col, fOpacity, 1, ptr->blockSide);
				break;
			case 2:
				DrawUtils::drawAABB(h, col, fOpacity, 2, ptr->blockSide);
				break;
			case 3:
				DrawUtils::drawAABB(h, col, fOpacity, 3, ptr->blockSide);
				break;
		}
		// Info
		char n[0x41];
		sprintf_s(n, 0x41, "%s", block->toLegacy()->name.getText());
		if (n[0] != 0)
			n[0] = toupper(n[0]);
		std::string name = n;
		std::replace<std::string::iterator, char>(name.begin(), name.end(), '_', ' ');
		if (ShowName) {
			DrawUtils::setColor(15 / 255.f, 30 / 255.f, 50 / 255.f, baOpacity);
			DrawUtils::fillRectangle(
				vec2_t((g_Data.getGuiData()->widthGame - DrawUtils::getTextWidth(&name, 1.5f)) /2.f - 3.f, 0.f),
				vec2_t((g_Data.getGuiData()->widthGame + DrawUtils::getTextWidth(&name, 1.5f)) /2.f + 3.f,
					   DrawUtils::getFontHeight(1.5f) + 3.5f));
			moduleMgr->getModule<Compass>()->drawCenteredText(
				vec2_t(g_Data.getGuiData()->widthGame / 2, -0.f), name , 1.5f, 1.f, col);
		}
	}
}