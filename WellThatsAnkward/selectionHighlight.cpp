#include "selectionHighlight.h"
#include "../Horion/Module/ModuleManager.h"
#include "../Utils/Target.h"
#include "../Utils/Logger.h"
#include "../Memory/Hooks.h"

selectionHighlight::selectionHighlight() : IModule(0x0, Category::VISUAL, "Custom selection box!") {
	FaceH.addEntry(EnumEntry("None", 0)).addEntry(EnumEntry("Selected", 1)).addEntry(EnumEntry("All", 3));
	ShowName.addEntry(EnumEntry("None", 0)).addEntry(EnumEntry("Simple", 1)).addEntry(EnumEntry("Detailed", 2));
	registerEnumSetting("Blockinfo", &ShowName, 2);
	//registerBoolSetting("Block Info", &this->ShowName, this->ShowName);
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
	if (/*ptr->entityPtr == nullptr && */ptr->rayHitType == 0) {
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(ptr->block);
		AABB h = block->toLegacy()->aabb;
		if (h.isFullBlock() || h.upper.x == h.lower.x || h.upper.y == h.lower.y || h.upper.z == h.lower.z)
			h = AABB(ptr->block.toVec3t(), ptr->block.add(1).toVec3t());
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
		BlockInfo Data = BlockInfo(block);
		if (ShowName.GetEntry()->GetValue() == 1) {
			DrawUtils::setColor(15 / 255.f, 30 / 255.f, 50 / 255.f, baOpacity);
			DrawUtils::fillRectangle(
				vec2_t((g_Data.getGuiData()->widthGame - DrawUtils::getTextWidth(&Data.name, 1.5f)) /2.f - 3.f, 0.f),
				vec2_t((g_Data.getGuiData()->widthGame + DrawUtils::getTextWidth(&Data.name, 1.5f)) / 2.f + 3.f,
					   DrawUtils::getFontHeight(1.5f) + 3.5f));
			moduleMgr->getModule<Compass>()->drawCenteredText(
				vec2_t(g_Data.getGuiData()->widthGame / 2, -0.f), Data.name, 1.5f, 1.f, col);
		} else if (ShowName.GetEntry()->GetValue() == 2) {
			DrawUtils::setColor(15 / 255.f, 30 / 255.f, 50 / 255.f, baOpacity);
			float with = DrawUtils::getTextWidth(&Data.name, 1.5f) +
						 std::fmaxf(DrawUtils::getTextWidth(&Data.GetLevel(), .75f),
									DrawUtils::getTextWidth(&Data.GetType(), .75f));
			DrawUtils::fillRectangle(vec2_t((g_Data.getGuiData()->widthGame - with) / 2.f - 3.f, 0.f),
									 vec2_t((g_Data.getGuiData()->widthGame + with) / 2.f + 3.f, 5.f));
		}
	}
	if (ptr->entityPtr != 0) {
		C_Entity* ent = ptr->entityPtr;
		Gradient dist = *Gradient(MC_Color(0.f, 1.f, 1.f, fOpacity), MC_Color(1.f, 0.f,0.f, fOpacity))
							 .AddEntry(GradientEntry(MC_Color(.75f, .75f, .0f), .5f)) // yellow color at the middle
							 ->AddEntry(GradientEntry(MC_Color(.0f, .875f, .0f), .75f));
		float avgBlob = ((ent->aabb.upper.x - ent->aabb.lower.x) + (ent->aabb.upper.y - ent->aabb.lower.y)
			+ (ent->aabb.upper.z - ent->aabb.lower.z)) / 3.f;
		float estimatedDist = (g_Data.getLocalPlayer()->eyePos0.dist(ent->aabb.centerPoint()) + avgBlob);
		MC_Color col = dist.GetColor(estimatedDist / moduleMgr->getModule<Reach>()->GetCurrentReach());
		AABB rendered = AABB(ent->getPos()->lerp(ent->getPosOld(), DrawUtils::getLerpTime()),
			ent->width, ent->height, ent->getPos()->y - ent->aabb.lower.y);
		//rendered.upper.y = rendered.upper.y + moduleMgr->getModule<TestModule>()->float1;
		DrawUtils::drawAABB(ent->aabb, col, fOpacity, 1, -1);
		//std::string name = ent->getNameTag()->getText();
		char n[0xf];
		sprintf_s(n, 0xf, "%.3f / %.3f", estimatedDist, moduleMgr->getModule<Reach>()->GetCurrentReach());
		std::string name = n;
		if (ShowName.GetEntry()->GetValue() > 0/* && name.length() > 1*/) {
			DrawUtils::setColor(15 / 255.f, 30 / 255.f, 50 / 255.f, baOpacity);
			DrawUtils::fillRectangle(
				vec2_t((g_Data.getGuiData()->widthGame - DrawUtils::getTextWidth(&name, 1.5f)) / 2.f - 3.f, 0.f),
				vec2_t((g_Data.getGuiData()->widthGame + DrawUtils::getTextWidth(&name, 1.5f)) / 2.f + 3.f,
					   DrawUtils::getFontHeight(1.5f) + 3.5f));
			moduleMgr->getModule<Compass>()->drawCenteredText(
				vec2_t(g_Data.getGuiData()->widthGame / 2, -0.f), name, 1.5f, 1.f, col);
		}
	}
}

std::string inline selectionHighlight::GetShownName(C_Block* block) {
	std::string Name = "";
	std::string N;
	int RawData = block->data;
	switch (block->toLegacy()->blockId) {
		case 1:
			switch (block->data) {
				case 0:
					Name = "Stone";
					break;
				case 1:
					Name = "Granite";
					break;
				case 2:
					Name = "Polished Granite";
					break;
				case 3:
					Name = "Diorite";
					break;
				case 4:
					Name = "Polished Diorite";
					break;
				case 5:
					Name = "Andesite";
					break;
				case 6:
					Name = "Polished Andesite";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 2:
			Name = "Grass Block";
			break;
		case 3:
			switch (block->data) {
				case 0:
					Name = "Dirt";
					break;
				case 1:
					Name = "Coarse Dirt";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 5:
			switch (block->data) {
				case 0:
					Name = "Oak Planks";
					break;
				case 1:
					Name = "Spruce Planks";
					break;
				case 2:
					Name = "Birch Planks";
					break;
				case 3:
					Name = "Jungle Planks";
					break;
				case 4:
					Name = "Acacia Planks";
					break;
				case 5:
					Name = "Dark Oak Planks";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 6:
			switch (block->data) {
				case 0:
					Name = "Oak Sapling";
					break;
				case 1:
					Name = "Spruce Sapling";
					break;
				case 2:
					Name = "Birch Sapling";
					break;
				case 3:
					Name = "Jungle Sapling";
					break;
				case 4:
					Name = "Acacia Sapling";
					break;
				case 5:
					Name = "Dark Oak Sapling";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 12:
			switch (block->data) {
				case 0:
					Name = "Sand";
					break;
				case 1:
					Name = "Red Sand";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 17:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					Name = "Oak Log";
					break;
				case 1:
					Name = "Spruce Log";
					break;
				case 2:
					Name = "Birch Log";
					break;
				case 3:
					Name = "Jungle Log";
					break;
				//case 4:
				//	Name = "Acacia Log";
				//case 5:
				//	Name = "Dark Oak Log";
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 162:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					Name = "Acacia Log";
					break;
				case 1:
					Name = "Dark Oak Log";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 18:
			// Data contains Pesistency & check for decay (bool)
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					Name = "Oak Leaves";
					break;
				case 1:
					Name = "Spruce Leaves";
					break;
				case 2:
					Name = "Birch Leaves";
					break;
				case 3:
					Name = "Jungle Leaves";
					break;
				//case 4:
				//	Name = "Acacia Leaves";
				//case 5:
				//	Name = "Dark Oak Leaves";
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 161:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					Name = "Acacia Leaves";
					break;
				case 1:
					Name = "Dark Oak Leaves";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 19:
			switch (block->data) {
				case 0:
					Name = "Dry Sponge";
					break;
				case 1:
					Name = "Wet Sponge";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 24:
			switch (block->data) {
				case 0:
					Name = "Sandstone";
					break;
				case 1:
					Name = "Chiseled Sandstone";
					break;
				case 2:
					Name = "Cut Sandstone";
					break;
				case 3:
					Name = "Smooth Sandstone";
					break;
				default:
					break;
			}
			break;
		case 27:
			Name = "Powered Rail";
			break;
		case 31:
			Name = "Grass";
			break;
		case 35:
			Name = "Wool";
			break;
		case 38:
			switch (block->data) {
				case 0:
					Name = "Poppy";
					break;
				case 1:
					Name = "Blue Orchid";
					break;
				case 2:
					Name = "Allium";
					break;
				case 3:
					Name = "Azure Bluet";
					break;
				case 4:
					Name = "Red Tulip";
					break;
				case 5:
					Name = "Orange Tulip";
					break;
				case 6:
					Name = "White Tulip";
					break;
				case 7:
					Name = "Pink Tulip";
					break;
				case 8:
					Name = "Oxeye Daisy";
					break;
				case 9:
					Name = "Cornflower";
					break;
				case 10:
					Name = "Lily of the Valley";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 43:
			switch (block->data) {
				case 0:
					Name = "Double Smooth Stone Slab";
					break;
				case 1:
					Name = "Double Sandstone Slab";
					break;
				case 2:
					Name = "Double Alpha-Wood Slab";
					break;
				case 3:
					Name = "Double Cobblestone Slab";
					break;
				case 4:
					Name = "Double Brick Slab";
					break;
				case 5:
					Name = "Double Stone Brick Slab";
					break;
				case 6:
					Name = "Double Quartz Slab";
					break;
				case 7:
					Name = "Double Nether Brick Slab";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 44:
			switch (block->data) {
				case 0:
					Name = "Lower Smooth Stone Slab";
					break;
				case 1:
					Name = "Lower Sandstone Slab";
					break;
				case 2:
					Name = "Lower Alpha Wood Slab";
					break;
				case 3:
					Name = "Lower Cobblestone Slab";
					break;
				case 4:
					Name = "Lower Brick Slab";
					break;
				case 5:
					Name = "Lower Stone Brick Slab";
					break;
				case 6:
					Name = "Lower Quartz Slab";
					break;
				case 7:
					Name = "Lower Nether Brick Slab";
					break;
				case 8:
					Name = "Upper Smooth Stone Slab";
					break;
				case 9:
					Name = "Upper Sandstone Slab";
					break;
				case 10:
					Name = "Upper Alpha Wood Slab";
					break;
				case 11:
					Name = "Upper Cobblestone Slab";
					break;
				case 12:
					Name = "Upper Brick Slab";
					break;
				case 13:
					Name = "Upper Stone Brick Slab";
					break;
				case 14:
					Name = "Upper Quartz Slab";
					break;
				case 15:
					Name = "Upper Nether Brick Slab";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 83:
			Name = "Sugarcane";
			break;
		case 85:
			switch (block->data) {
				case 0:
					Name = "Oak Fence";
					break;
				case 1:
					Name = "Spruce Fence";
					break;
				case 2:
					Name = "Birch Fence";
					break;
				case 3:
					Name = "Jungle Fence";
					break;
				case 4:
					Name = "Acacia Fence";
					break;
				case 5:
					Name = "Dark Oak Fence";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 97:
			switch (block->data) {
				case 0:
					Name = "Infested Stone";
					break;
				case 1:
					Name = "Infested Cobblestone";
					break;
				case 2:
					Name = "Infested Stone Bricks";
					break;
				case 3:
					Name = "Infested Mossy Stone Bricks";
					break;
				case 4:
					Name = "Infested Cracked Stone Bricks";
					break;
				case 5:
					Name = "Infested Chiseled Stone Bricks";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 98:
			switch (block->data) {
				case 0:
					Name = "Stone Bricks";
					break;
				case 1:
					Name = "Mossy Stone Bricks";
					break;
				case 2:
					Name = "Cracked Stone Bricks";
					break;
				case 3:
					Name = "Chiseled Stone Bricks";
					break;
				case 4:
					Name = "Smooth Stone Bricks";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 100:
			if (!(block->data == 15 || block->data == 10))
				break;
			__fallthrough;
		case 99:
			if (block->data == 15 || block->data == 10) {
				Name = "Mushroom Stem";
				break;
			}
			Name = GetData(block);
			break;
		case 155:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					Name = "Quartz Block";
					break;
				case 1:
					Name = "Chiseled Quartz Block";
					break;
				case 2:
					Name = "Pillar Quartz Block";
					break;
				case 3:
					Name = "Smooth Quartz Block";
					break;
				default:
					break;
			}
			break;
		case 157:
			switch (block->data) {
				case 0:
					Name = "Oak Wood Double Slab";
					break;
				case 1:
					Name = "Spruce Wood Double Slab";
					break;
				case 2:
					Name = "Birch Wood Double Slab";
					break;
				case 3:
					Name = "Jungle Wood Double Slab";
					break;
				case 4:
					Name = "Acacia Wood Double Slab";
					break;
				case 5:
					Name = "Dark Oak Wood Double Slab";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 158:
			switch (block->data) {
				case 0:
					Name = "Lower Oak slab";
					break;
				case 1:
					Name = "Lower Spruce slab";
					break;
				case 2:
					Name = "Lower Birch slab";
					break;
				case 3:
					Name = "Lower Jungle slab";
					break;
				case 4:
					Name = "Lower Acacia slab";
					break;
				case 5:
					Name = "Lower Dark Oak slab";
					break;
				case 8:
					Name = "Upper Oak slab";
					break;
				case 9:
					Name = "Upper Spruce slab";
					break;
				case 10:
					Name = "Upper Birch slab";
					break;
				case 11:
					Name = "Upper Jungle slab";
					break;
				case 12:
					Name = "Upper Acacia slab";
					break;
				case 13:
					Name = "Upper Dark Oak slab";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 159:
			Name = "Stained Clay";
			break;
		case 160:
			Name = "Stained Glass Pane";
			break;
		case 168:
			switch (block->data) {
				case 0:
					Name = "Prismarine";
					break;
				case 1:
					Name = "Dark Prismarine";
					break;
				case 2:
					Name = "Prismarine Bricks";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 171:
			Name = "Carpet";
			break;
		case 175:
			while (RawData > 7)
				RawData -= 8;
			switch (RawData) {
				case 0:
					Name = "Sunflower";
					break;
				case 1:
					Name = "Lilac";
					break;
				case 2:
					Name = "Double Tallgrass";
					break;
				case 3:
					Name = "Large Fern";
					break;
				case 4:
					Name = "Rose Bush";
					break;
				case 5:
					Name = "Peony";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
		case 179:
			switch (block->data) {
				case 0:
					Name = "Red Sandstone";
					break;
				case 1:
					Name = "Chiseled Red Sandstone";
					break;
				case 2:
					Name = "Cut Red Sandstone";
					break;
				case 3:
					Name = "Smooth Red Sandstone";
					break;
				default:
					break;
			}
			break;
		case 199:
			Name = "Item Frame";
			break;
		case 202:
			Name = "Red Torch";
			break;
		case 204:
			Name = "Blue Torch";
			break;
		case 205:
			Name = "Shulker Box";
			break;
		case 218:
			Name = "Colred Shulker Box";
			break;
		case 236:
			Name = "Concrete";
			break;
		case 237:
			Name = "Concrete Powder";
			break;
		case 241:
			Name = "Stained Glass";
			break;
		case 246:
			Name = "Glowing Obsidian";
			break;
		case 247:
			switch (block->data) {
				case 0:
					Name = "Nether Reactor Core";
					break;
				case 1:
					Name = "Initialized Nether Reactor Core";
					break;
				case 2:
					Name = "Finished Nether Reactor Core";
					break;
				default:
					Name = GetData(block);
					break;
			}
			break;
	// slabs
		case 182: // Stone Slab 2
			N = "Lower ";
			if (RawData > 7)
				N = "Upper ";
			__fallthrough;
		case 181: // Double Stone Slab 2
			if (block->toLegacy()->blockId == 422)
				N = "Double ";
			if (RawData > 7)
				RawData -= 8;
			switch (RawData) {
				case 0:
					N += "Red Sandstone Slab";
					break;
				case 1:
					N += "Purpur Slab";
					break;
				case 2:
					N += "Prismarine Slab";
					break;
				case 3:
					N += "Prismarine Brick Slab";
					break;
				case 4:
					N += "Dark Prismarine Slab";
					break;
				case 5:
					N += "Mossy Cobblestone Slab";
					break;
				case 6:
					N += "Smooth Sandstone Slab";
					break;
				case 7:
					N += "Red Nether Brick Slab";
					break;
				default:
					N = GetData(block);
					break;
			}
			Name = N;
			break;
		case 417: // Stone Slab 3
			N = "Lower ";
			if (RawData > 7)
				N = "Upper ";
			__fallthrough;
		case 422: // Double Stone Slab 3
			if (block->toLegacy()->blockId == 422)
				N = "Double ";
			if (RawData > 7)
				RawData -= 8;
			switch (RawData) {
				case 0:
					N += "End Stone Brick Slab";
					break;
				case 1:
					N += "Smooth Red Sandstone Slab";
					break;
				case 2:
					N += "Polished Andesite Slab";
					break;
				case 3:
					N += "Andesite Slab";
					break;
				case 4:
					N += "Diorite Slab";
					break;
				case 5:
					N += "Polished Diorite Slab";
					break;
				case 6:
					N += "Granite Slab";
					break;
				case 7:
					N += "Polished Granite Slab";
					break;
				default:
					N = GetData(block);
					break;
			}
			Name = N;
			break;
		case 421: // Stone Slab 4
			N = "Lower ";
			if (RawData > 7)
				N = "Upper ";
			__fallthrough;
		case 423: // Double Stone Slab 4
			if (block->toLegacy()->blockId == 422)
				N = "Double ";
			if (RawData > 7)
				RawData -= 8;
			switch (RawData) {
				case 0:
					N += "Mossy Stone Brick Slab";
					break;
				case 1:
					N += "Smooth Quartz Slab";
					break;
				case 2:
					N += "Stone Slab";
					break;
				case 3:
					N += "Cut Sandstone Slab";
					break;
				case 4:
					N += "Cut Red Sandstone Slab";
					break;
				default:
					N = GetData(block);
					break;
			}
			Name = N;
			break;
		case 465:
			N = "";
			if (RawData > 7) {
				N += "Stripped ";
				RawData -= 8;
			}
			switch (RawData) {
				case 0:
					N += "Oak";
					break;
				case 1:
					N += "Spruce";
					break;
				case 2:
					N += "Birch";
					break;
				case 3:
					N += "Jungle";
					break;
				case 4:
					N += "Acacia";
					break;
				case 5:
					N += "Dark Oak";
					break;
				default:
					N = GetData(block);
					break;
			}
			N += " Bark Wood";
			Name = N;
			break;
		default:
#ifdef _DEBUG
			Name = GetData(block);
#else
			Name = block->toLegacy()->name.getText();
#endif
			break;
	}
	char n[0x65];
	sprintf_s(n, 0x65, "%s", Name.c_str());
	if (n[0] != 0)
		n[0] = toupper(n[0]);
	N = std::string(n);
	std::replace<std::string::iterator, char>(N.begin(), N.end(), '_', ' ');
	return N;
}

std::string inline selectionHighlight::GetData(C_Block* block) {
	char N[0x41];
	char n[0x65];
	std::string _N = block->toLegacy()->name.getText();
#ifdef _DEBUG
	// logs(Wood)
	// old/new Wood
	if (block->toLegacy()->blockId == 17 || block->toLegacy()->blockId == 18)
		_N.push_back('1');
	else if (block->toLegacy()->blockId == 161 || block->toLegacy()->blockId == 162)
		_N.push_back('2');
#endif
	sprintf_s(N, 0x41, "ID:%i (%i)", block->toLegacy()->blockId, block->data);
	sprintf_s(n, 0x65, "%s %s", _N.c_str(), N);
	if (n[0] != 0)
		n[0] = toupper(n[0]);
	return std::string(n);
}

BlockInfo::BlockInfo(C_Block* block) {
	name = selectionHighlight::GetShownName(block);
	switch (block->toLegacy()->blockId) {
		default:
			Type = ToolType::hand;
			force = false;
			min = ToolLevel::noLvl;
			silktouch = no;
			break;
	}
}
std::string BlockInfo::GetType() {
	switch (Type) {
		case _Unknown:
			return "?Unknown?";
		case hand:
			return "Hand";
		case picaxe:
			return "Picaxe";
		case axe:
			return "Axe";
		case shovel:
			return "Shovel";
		case hoe:
			return "Hoe";
		case sword:
			return "Sword";
		case shears:
			return "Shears";
	}
}
std::string BlockInfo::GetLevel() {
	switch (min) {
		case _unknown:
			return "?Unknown?";
		case noLvl:
			return "None";
		case all:
			return "All";
		case wood:
			return "Wood";
		case gold:
			return "Gold";
		case stone:
			return "Stone";
		case iron:
			return "Irond";
		case diamond:
			return "Diamond";
		case netherite:
			return "Netherite";
	}
}
//std::string BlockInfo::GetSilkTouch() {
//	switch (silktouch) {
//		case no:
//			break;
//		case maybe:
//			break;
//		case yes:
//			break;
//	}
//}