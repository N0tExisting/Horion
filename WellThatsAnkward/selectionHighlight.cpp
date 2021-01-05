#include "selectionHighlight.h"
#include "../Utils/Target.h"
#include "../Utils/Logger.h"
#include "../Memory/Hooks.h"

selectionHighlight::selectionHighlight() : IModule(0x0, Category::VISUAL, "Custom selection box!") {
	FaceH.addEntry(EnumEntry("None", 0)).addEntry(EnumEntry("Selected", 1))
	 .addEntry(EnumEntry("All", 2)).addEntry(EnumEntry("Not Selected", 3));
	registerEnumSetting("FaceHighlight", &FaceH, 0);
	registerFloatSetting("FaceOpacity", &this->fOpacity, this->fOpacity, 0.f, 1.f);
	registerFloatSetting("Thickness", &this->thickness, this->thickness, 0.f, 1.f);
	registerFloatSetting("Opacity", &this->opacityVal, this->opacityVal, 0.f, 1.f);
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
	return ("SelectionHighlight");
}

//void selectionHighlight::onEnable() {
//	if ()
//}

void selectionHighlight::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	//logF("%s", g_Hooks.GetScreenName());
	//float customHighlight[4];
	//
	//customHighlight[0] = rSelect;
	//customHighlight[1] = rSelect;
	//customHighlight[2] = rSelect;
	//customHighlight[3] = 1.f;
	//
	//static float rainbowColors[4];
	//
	//{
	//	if (rainbowColors[3] < 1) {
	//		rainbowColors[0] = 0.2f;
	//		rainbowColors[1] = 0.2f;
	//		rainbowColors[2] = 1.f;
	//		rainbowColors[3] = 1;
	//	}
	//
	//	Utils::ApplyRainbow(rainbowColors, 0.0015f);
	//}  //0.0015f
	//
	//if (g_Data.getLocalPlayer() != nullptr) {
	//	float mC = thickness / 2;
	//	auto pStruct = g_Data.getClientInstance()->getPointerStruct();
	//
	//	vec3_ti hLower = pStruct->block;
	//	vec3_ti hUpper = pStruct->block;
	//	hUpper.x += 1.f;
	//	hUpper.y += 1.f;
	//	hUpper.z += 1.f;
	//
	//	if (selectRainbow) {
	//		DrawUtils::setColor(rainbowColors[0], rainbowColors[1], rainbowColors[2], opacityVal);
	//		DrawUtils::drawBox(hLower.toVec3t(), hUpper.toVec3t(), thickness, doOutline);
	//
	//		if (faceH) {
	//			int face = pStruct->blockSide;
	//			int rayType = pStruct->rayHitType;
	//
	//			if (face == 1) {
	//				DrawUtils::setColor(rainbowColors[0], rainbowColors[1], rainbowColors[2], fOpacity);
	//
	//				vec2_t c1 = DrawUtils::worldToScreen(hUpper);
	//				vec2_t c2 = DrawUtils::worldToScreen(vec3_t(hUpper.x, hUpper.y, hUpper.z - 1.f));
	//				vec2_t c3 = DrawUtils::worldToScreen(vec3_t(hUpper.x - 1.f, hUpper.y, hUpper.z - 1.f));
	//				vec2_t c4 = DrawUtils::worldToScreen(vec3_t(hUpper.x - 1.f, hUpper.y, hUpper.z));
	//				//	DrawUtils::drawQuad(c1, c2, c3, c4);
	//			}
	//
	//			//DrawUtils::drawText(vec2_t(100.f, 100.f), &std::string(std::to_string(face) + " " + std::to_string(rayType)), MC_Color(255, 255, 0));
	//		}
	//
	//	} else {
	//		DrawUtils::setColor(rSelect, gSelect, bSelect, opacityVal);
	//		DrawUtils::drawBox(hLower, hUpper, thickness, doOutline);
	//
	//		if (faceH) {
	//		}
	//	}
	//}
	static float rainbowColors[4] = {0.2f, 0.2f, 1.f, opacityVal};
	Utils::ApplyRainbow(rainbowColors, 0.0015f);
	float Color[4] = {rSelect, gSelect, bSelect, opacityVal};
	if (selectRainbow) {
		Color[0] = rainbowColors[0];
		Color[1] = rainbowColors[1];
		Color[2] = rainbowColors[2];
	}
	if (g_Data.getClientInstance() == nullptr ||
		g_Data.getPtrLocalPlayer() == nullptr ||
		g_Data.getLocalPlayer() == nullptr)
		return;
	PointingStruct* ptr = g_Data.getClientInstance()->getPointerStruct();
	if (ptr == nullptr) return;
	if (ptr->entityPtr == nullptr && ptr->rayHitType == 0) {
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
		if (selectRainbow)
			Utils::ApplyRainbow(Color, 0.5f);
		//AABB col = g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->aabb;
		AABB h = AABB(ptr->block.toVec3t(), ptr->block.add(1).toVec3t());
		DrawUtils::setColor(Color[0], Color[1], Color[2], fOpacity);
		bool igndVal[6]{};
		switch (FaceH.GetEntry()->GetValue()) {
			case 0:
				break;
			case 1:
				for (size_t i = 0; i < 6; i++)
					igndVal[i] = true;
				igndVal[ptr->blockSide] = false;
				DrawUtils::drawAABB(h, MC_Color((const float*)Color), fOpacity, igndVal);
				break;
			case 2:
				for (size_t i = 0; i < 6; i++)
					igndVal[i] = false;
				DrawUtils::drawAABB(h, MC_Color((const float*)Color), fOpacity, igndVal);
				break;
			case 3:
				for (size_t i = 0; i < 6; i++)
					igndVal[i] = false;
				igndVal[ptr->blockSide] = true;
				DrawUtils::drawAABB(h, MC_Color((const float*)Color), fOpacity, igndVal);
				break;
		}
	}
}