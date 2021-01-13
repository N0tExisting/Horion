#include "selectionHighlight.h"
#include "../Horion/Module/ModuleManager.h"
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
		std::string name = /*->*/GetShownName(block);
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

std::string inline selectionHighlight::GetShownName(C_Block* block) {
	std::string Name = "";
	std::string N;
	int RawData = block->data;
	switch (block->toLegacy()->blockId) {
		case 1:
			switch (block->data) {
				case 0:
					return "Stone";
				case 1:
					return "Granite";
				case 2:
					return "Polished Granite";
				case 3:
					return "Diorite";
				case 4:
					return "Polished Diorite";
				case 5:
					return "Andesite";
				case 6:
					return "Polished Andesite";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 2:
			return "Grass Block";
			break;
		case 3:
			switch (block->data) {
				case 0:
					return "Dirt";
				case 1:
					return "Coarse Dirt";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 5:
			switch (block->data) {
				case 0:
					return "Oak Planks";
				case 1:
					return "Spruce Planks";
				case 2:
					return "Birch Planks";
				case 3:
					return "Jungle Planks";
				case 4:
					return "Acacia Planks";
				case 5:
					return "Dark Oak Planks";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 6:
			switch (block->data) {
				case 0:
					return "Oak Sapling";
				case 1:
					return "Spruce Sapling";
				case 2:
					return "Birch Sapling";
				case 3:
					return "Jungle Sapling";
				case 4:
					return "Acacia Sapling";
				case 5:
					return "Dark Oak Sapling";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 12:
			switch (block->data) {
				case 0:
					return "Sand";
				case 1:
					return "Red Sand";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 17:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					return "Oak Log";
				case 1:
					return "Spruce Log";
				case 2:
					return "Birch Log";
				case 3:
					return "Jungle Log";
				//case 4:
				//	return "Acacia Log";
				//case 5:
				//	return "Dark Oak Log";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 162:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					return "Acacia Log";
				case 1:
					return "Dark Oak Log";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 18:
			// Data contains Pesistency & check for decay (bool)
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					return "Oak Leaves";
				case 1:
					return "Spruce Leaves";
				case 2:
					return "Birch Leaves";
				case 3:
					return "Jungle Leaves";
				//case 4:
				//	return "Acacia Leaves";
				//case 5:
				//	return "Dark Oak Leaves";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 161:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					return "Acacia Leaves";
				case 1:
					return "Dark Oak Leaves";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 19:
			switch (block->data) {
				case 0:
					return "Dry Sponge";
				case 1:
					return "Wet Sponge";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 24:
			switch (block->data) {
				case 0:
					return "Sandstone";
				case 1:
					return "Chiseled Sandstone";
				case 2:
					return "Cut Sandstone";
				case 3:
					return "Smooth Sandstone";
				default:
					break;
			}
			break;
		case 27:
			return "Powered Rail";
			break;
		case 31:
			return "Grass";
			break;
		case 35:
			return "Wool";
			break;
		case 38:
			switch (block->data) {
				case 0:
					return "Poppy";
				case 1:
					return "Blue Orchid";
				case 2:
					return "Allium";
				case 3:
					return "Azure Bluet";
				case 4:
					return "Red Tulip";
				case 5:
					return "Orange Tulip";
				case 6:
					return "White Tulip";
				case 7:
					return "Pink Tulip";
				case 8:
					return "Oxeye Daisy";
				case 9:
					return "Cornflower";
				case 10:
					return "Lily of the Valley";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 43:
			switch (block->data) {
				case 0:
					return "Double Smooth Stone Slab";
				case 1:
					return "Double Sandstone Slab";
				case 2:
					return "Double Alpha Wood Slab";
				case 3:
					return "Double Cobblestone Slab";
				case 4:
					return "Double Brick Slab";
				case 5:
					return "Double Stone Brick Slab";
				case 6:
					return "Double Quartz Slab";
				case 7:
					return "Double Nether Brick Slab";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 44:
			switch (block->data) {
				case 0:
					return "Lower Smooth Stone Slab";
				case 1:
					return "Lower Sandstone Slab";
				case 2:
					return "Lower Alpha Wood Slab";
				case 3:
					return "Lower Cobblestone Slab";
				case 4:
					return "Lower Brick Slab";
				case 5:
					return "Lower Stone Brick Slab";
				case 6:
					return "Lower Quartz Slab";
				case 7:
					return "Lower Nether Brick Slab";
				case 8:
					return "Upper Smooth Stone Slab";
				case 9:
					return "Upper Sandstone Slab";
				case 10:
					return "Upper Alpha Wood Slab";
				case 11:
					return "Upper Cobblestone Slab";
				case 12:
					return "Upper Brick Slab";
				case 13:
					return "Upper Stone Brick Slab";
				case 14:
					return "Upper Quartz Slab";
				case 15:
					return "Upper Nether Brick Slab";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 83:
			return "Sugarcane";
		case 85:
			switch (block->data) {
				case 0:
					return "Oak Fence";
				case 1:
					return "Spruce Fence";
				case 2:
					return "Birch Fence";
				case 3:
					return "Jungle Fence";
				case 4:
					return "Acacia Fence";
				case 5:
					return "Dark Oak Fence";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 97:
			switch (block->data) {
				case 0:
					return "Infested Stone";
				case 1:
					return "Infested Cobblestone";
				case 2:
					return "Infested Stone Bricks";
				case 3:
					return "Infested Mossy Stone Bricks";
				case 4:
					return "Infested Cracked Stone Bricks";
				case 5:
					return "Infested Chiseled Stone Bricks";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 98:
			switch (block->data) {
				case 0:
					return "Stone Bricks";
				case 1:
					return "Mossy Stone Bricks";
				case 2:
					return "Cracked Stone Bricks";
				case 3:
					return "Chiseled Stone Bricks";
				case 4:
					return "Smooth Stone Bricks";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 100:
			if (!(block->data == 15 || block->data == 10))
				break;
			__fallthrough;
		case 99:
			if (block->data == 15 || block->data == 10)
				return "Mushroom Stem";
			Name = GetInfo(block);
			break;
		case 155:
			while (RawData > 4)
				RawData -= 4;
			switch (RawData) {
				case 0:
					return "Quartz Block";
				case 1:
					return "Chiseled Quartz Block";
				case 2:
					return "Pillar Quartz Block";
				case 3:
					return "Smooth Quartz Block";
				default:
					break;
			}
			break;
		case 157:
			switch (block->data) {
				case 0:
					return "Oak Wood Double Slab";
				case 1:
					return "Spruce Wood Double Slab";
				case 2:
					return "Birch Wood Double Slab";
				case 3:
					return "Jungle Wood Double Slab";
				case 4:
					return "Acacia Wood Double Slab";
				case 5:
					return "Dark Oak Wood Double Slab";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 158:
			switch (block->data) {
				case 0:
					return "Lower Oak slab";
				case 1:
					return "Lower Spruce slab";
				case 2:
					return "Lower Birch slab";
				case 3:
					return "Lower Jungle slab";
				case 4:
					return "Lower Acacia slab";
				case 5:
					return "Lower Dark Oak slab";
				case 8:
					return "Upper Oak slab";
				case 9:
					return "Upper Spruce slab";
				case 10:
					return "Upper Birch slab";
				case 11:
					return "Upper Jungle slab";
				case 12:
					return "Upper Acacia slab";
				case 13:
					return "Upper Dark Oak slab";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 159:
			return "Stained Clay";
			break;
		case 160:
			return "Stained Glass Pane";
			break;
		case 168:
			switch (block->data) {
				case 0:
					return "Prismarine";
				case 1:
					return "Dark Prismarine";
				case 2:
					return "Prismarine Bricks";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 171:
			return "Carpet";
			break;
		case 175:
			while (RawData > 7)
				RawData -= 8;
			switch (RawData) {
				case 0:
					return "Sunflower";
				case 1:
					return "Lilac";
				case 2:
					return "Double Tallgrass";
				case 3:
					return "Large Fern";
				case 4:
					return "Rose Bush";
				case 5:
					return "Peony";
				default:
					Name = GetInfo(block);
					break;
			}
			break;
		case 179:
			switch (block->data) {
				case 0:
					return "Red Sandstone";
				case 1:
					return "Chiseled Red Sandstone";
				case 2:
					return "Cut Red Sandstone";
				case 3:
					return "Smooth Red Sandstone";
				default:
					break;
			}
			break;
		case 199:
			return "Item Frame";
		case 202:
			return "Red Torch";
		case 204:
			return "Blue Torch";
		case 205:
			return "Shulker Box";
		case 218:
			return "Colred Shulker Box";
		case 236:
			return "Concrete";
		case 237:
			return "Concrete Powder";
		case 241:
			return "Stained Glass";
		case 246:
			return "Glowing Obsidian";
		case 247:
			switch (block->data) {
				case 0:
					return "Nether Reactor Core";
				case 1:
					return "Initialized Nether Reactor Core";
				case 2:
					return "Finished Nether Reactor Core";
				default:
					Name = GetInfo(block);
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
					N = GetInfo(block);
					break;
			}
			return N;
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
					N = GetInfo(block);
					break;
			}
			return N;
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
					N = GetInfo(block);
					break;
			}
			return N;
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
					N = GetInfo(block);
					break;
			}
			N += " Bark Wood";
			return N;
		default:
			Name = block->toLegacy()->name.getText();
			break;
	}
#ifdef _DEBUG
	return GetInfo(block);
#else
	char n[0x65];
	sprintf_s(n, 0x65, "%s", Name.c_str());
	if (n[0] != 0)
		n[0] = toupper(n[0]);
	return std::string(n);
#endif
}

std::string inline selectionHighlight::GetInfo(C_Block* block) {
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