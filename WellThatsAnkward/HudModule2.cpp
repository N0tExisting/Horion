#include "HudModule2.h"
#include "../Horion/DrawUtils.h"
#include "../Horion/Scripting/ScriptManager.h"
#include "../Utils/Logger.h"
#include "HudEditor.h"

HudModule2::HudModule2() : IModule(0x0, Category::VISUAL, "Edit HUD elements") {
	registerBoolSetting("TabGui", &this->tabgui, this->tabgui);
	registerBoolSetting("ArrayList", &this->arraylist, this->arraylist);
	registerBoolSetting("ClickToggle", &this->clickToggle, this->clickToggle);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
	registerBoolSetting("Coordinates", &this->coordinates, this->coordinates);
	registerBoolSetting("Show Keybinds", &this->keybinds, this->keybinds);
	registerBoolSetting("Show ArmorHUD", &this->displayArmor, this->displayArmor);
	registerBoolSetting("Keystrokes", &this->keystrokes, this->keystrokes);
	registerBoolSetting("Show FPS", &this->fps, this->fps);
	registerBoolSetting("Show CPS", &this->cps, this->cps);
	//registerBoolSetting("Always show", &this->alwaysShow, this->alwaysShow);
	
	registerBoolSetting("welcome", &this->welcome, this->welcome);
	registerBoolSetting("renderInv", &this->invRender, this->invRender);
	registerBoolSetting("pvpResources", &this->pvpR, this->pvpR);
	registerBoolSetting("playerList", &this->doList, this->doList);
	registerFloatSetting("InvPosX", &this->xVal, this->xVal, 1.f, 465.f);
	registerFloatSetting("HUD Scale", &this->scale, this->scale, 0.5f, 1.5f);

	/*
	registerFloatSetting("invPosX", &this->invPosX, this->invPosX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("invPosY", &this->invPosY, this->invPosY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("pvpResourcesX", &this->pvpX, this->pvpX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("pvpResourcesY", &this->pvpY, this->pvpY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("armorX", &this->armorX, this->armorX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("armorY", &this->armorY, this->armorY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);

	//*/
}
HudModule2::~HudModule2() {
}
const char* HudModule2::getModuleName() {
	return ("HUD");
}

static std::vector<C_Entity*> playerList;
void findPlayers(C_Entity* currentEntity, bool isRegularEntity) {
	//log(*currentEntity->getNameTag()->getText());
	if (currentEntity == nullptr)
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	if (currentEntity->getEntityTypeId() != 63)  // player
		return;

	playerList.push_back(currentEntity);
}

int Odelay = 0;
float xDiff = -1.f;
float yDiff = -1.f;

void HudModule2::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	float f = 10.f * this->scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	if (tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;

	vec2_t mousePosition = *g_Data.getClientInstance()->getMousePos();
	mousePosition = mousePosition.div(windowSizeReal);
	mousePosition = mousePosition.mul(windowSize);
	static auto hudEditorMod = moduleMgr->getModule<HudEditor>();

	if (hudEditorMod->isEnabled()) {
		if (g_Data.getClientInstance()->getMouseGrabbed())
			g_Data.getClientInstance()->releaseMouse();
	}

	{  // FPS
		if (!(g_Data.getLocalPlayer() == nullptr || !this->fps)) {
			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), 1.f);
			DrawUtils::drawText(textPos, &fpsText, MC_Color(200, 200, 200), scale);

			startY += f;
		}
	}
	{  // CPS
		if (!(g_Data.getLocalPlayer() == nullptr || !this->cps)) {
			std::string cpsText = "CPS: " + std::to_string(g_Data.getLeftCPS()) + " - " + std::to_string(g_Data.getRightCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), 1.f);
			DrawUtils::drawText(textPos, &cpsText, MC_Color(200, 200, 200), scale);

			startY += f;
		}
	}
	{  // Coordinates
		if (!(g_Data.getLocalPlayer() == nullptr || !this->coordinates)) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();

			std::string coordsX = "X: " + std::to_string((int)floorf(pos->x));
			std::string coordsY = "Y: " + std::to_string((int)floorf(pos->y));
			std::string coordsZ = "Z: " + std::to_string((int)floorf(pos->z));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			vec2_t outLinePos = vec2_t(textPos.x + 0.6f * scale, textPos.y - 0.6f * scale);
			//DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), 1.f);
			std::string coordsText = "" + coordsX + " " + coordsY + " " + coordsZ;
			DrawUtils::drawText(outLinePos, &coordsText, MC_Color(0, 0, 0), scale * 1.3f, 1.f, Fonts::SMOOTH);
			DrawUtils::drawText(textPos, &coordsText, MC_Color(255, 255, 255), scale * 1.3f, 1.f, Fonts::SMOOTH);
			startY += f;
		}
	}
	{  // ArmorHUD
		if (!(g_Data.getLocalPlayer() == nullptr || !this->displayArmor)) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 15.f;
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			float x = hudEditorMod->armorX;
			float y = hudEditorMod->armorY;
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					x += scale * spacing;
				}
			}
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			//x += scale * spacing;
			if (item->item != nullptr)
				DrawUtils::drawItem(item, vec2_t(x, y), opacity, scale, item->isEnchanted());

			if (hudEditorMod->isEnabled()) {
				if ((mousePosition.x >= hudEditorMod->armorX && mousePosition.x <= x) && (mousePosition.y >= hudEditorMod->armorY && mousePosition.y <= hudEditorMod->armorY + 20.f)) {
					if (!g_Data.isLeftClickDown()) {
						xDiff = mousePosition.x - hudEditorMod->armorX;
						yDiff = mousePosition.y - hudEditorMod->armorY;
					}

					if (xDiff <= 0.f) {
						xDiff = 40.f;
					}

					if (yDiff <= 0.f) {
						yDiff = 40.f;
					}
					DrawUtils::setColor(0.7f, 0.7f, 0.7f, 0.8f);
					DrawUtils::fillRectangle(vec2_t(hudEditorMod->armorX, hudEditorMod->armorY), vec2_t(x, hudEditorMod->armorY + 20.f));

					if (g_Data.isLeftClickDown()) {
						if (mousePosition.x - xDiff >= 1.f) {
							hudEditorMod->armorX = mousePosition.x - xDiff;
						} else {
							hudEditorMod->armorX = mousePosition.x;
						}

						if (mousePosition.y - yDiff >= 1.f) {
							hudEditorMod->armorY = mousePosition.y - yDiff;
						} else {
							hudEditorMod->armorY = mousePosition.y;
						}  //*/
						   //	pvpX = mousePosition.x;
						   //	pvpY = mousePosition.y;

						DrawUtils::setColor(0, 1, 1, 1);
						DrawUtils::drawLine(vec2_t(0, 0), mousePosition, 1.f);
					}
				}
			}


		}
	}
	{
		if (doList) {
			if ((g_Data.getLocalPlayer() != nullptr)) {  //player list
				int runs2 = 0;
				playerList.clear();
				g_Data.forEachEntity(findPlayers);
				if (!playerList.empty()) {
					//g_Data.forEachEntity()
					for (auto& ye : playerList) {
						float distye = (*ye->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
						char eeeee = *ye->getNameTag()->getText();
						std::string why = ye->getNameTag()->getText();
						std::string* isThisSoBroken = &why;
						//	log(*playerList[0]->getNameTag()->getText());
						if (runs2 == 0) {
							if (strcmp(g_Data.getLocalPlayer()->getNameTag()->getText(), ye->getNameTag()->getText()) == 0) {
								DrawUtils::drawText(vec2_t(windowSize.x - 120.f, 30.f), isThisSoBroken, MC_Color(0, 0, 255), 1.f);
							} else if (FriendList::findPlayer(ye->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled()) {
								DrawUtils::drawText(vec2_t(windowSize.x - 120.f, 30.f), isThisSoBroken, MC_Color(0, 255, 0), 1.f);
							} else {
								DrawUtils::drawText(vec2_t(windowSize.x - 120.f, 30.f + (15.f * runs2)), isThisSoBroken, MC_Color(255, 0, 0), 1.f);
							}

						} else {
							if (strcmp(g_Data.getLocalPlayer()->getNameTag()->getText(), ye->getNameTag()->getText()) == 0) {
								DrawUtils::drawText(vec2_t(windowSize.x - 120.f, 30.f), isThisSoBroken, MC_Color(0, 0, 255), 1.f);
							} else if (FriendList::findPlayer(ye->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled()) {
								DrawUtils::drawText(vec2_t(windowSize.x - 120.f, 30.f), isThisSoBroken, MC_Color(0, 255, 0), 1.f);
							} else {
								DrawUtils::drawText(vec2_t(windowSize.x - 120.f, 30.f + (15.f * runs2)), isThisSoBroken, MC_Color(255, 0, 0), 1.f);
							}
						}

						static float constexpr scale2 = 0.6f;
						static float constexpr opacity2 = 0.25f;
						static float constexpr spacing2 = scale2 + 15.f;
						float x2 = windowSize.x - 180.f;
						float y2 = 30.f;
						if (runs2 == 0) {
							y2 = 30.f;
						} else {
							y2 = 30.f + (15.f * runs2);
						}
						for (int i2 = 0; i2 < 4; i2++) {
							C_ItemStack* stack2 = ye->getArmor(i2);
							if (stack2->item != nullptr) {
								DrawUtils::drawItem(stack2, vec2_t(x2, y2), opacity2, scale2, stack2->isEnchanted());
								x2 += scale * spacing2;
							}
						}
						runs2++;
					}
				}
			}
		}
	}
	{  // Keystrokes
		if (!(g_Data.getLocalPlayer() == nullptr || !this->keystrokes || !GameData::canUseMoveKeys())) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			DrawUtils::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 74));
			DrawUtils::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 30));
		}
	}
	{  // welcome
		if (g_Data.getLocalPlayer() != nullptr && this->welcome) {
			const char* yourUsername = "n";
			yourUsername = g_Data.getLocalPlayer()->getNameTag()->getText();

			std::stringstream yikeBuild;
			yikeBuild << "Welcome ";
			yikeBuild << g_Data.getLocalPlayer()->getNameTag()->getText();
			std::string yikes = "Welcome " + *g_Data.getLocalPlayer()->getNameTag()->getText();
			DrawUtils::drawText(vec2_t((windowSize.x) - 430.f, windowSize.y - 413.f), &yikeBuild.str(), MC_Color(255, 0, 0), scale * 1.1f);
		}
	}

	{  // pvp resources
		if (pvpR && g_Data.getLocalPlayer() != nullptr) {
			float testValYe = 40.f * scale;
			float lineWidth2 = 2.f * scale;

		

			DrawUtils::setColor(0, 0, 0, 1);
			DrawUtils::fillRectangle(vec2_t(hudEditorMod->pvpX, hudEditorMod->pvpY), vec2_t(hudEditorMod->pvpX + (testValYe), hudEditorMod->pvpY + (testValYe)));
			DrawUtils::setColor(0.15, 0.15, 0.15, 1);
			DrawUtils::drawRectangle(vec2_t(hudEditorMod->pvpX, hudEditorMod->pvpY), vec2_t(hudEditorMod->pvpX + (testValYe), hudEditorMod->pvpY + testValYe), lineWidth2);
			vec2_t centerPoint = vec2_t(hudEditorMod->pvpX + testValYe / 2, hudEditorMod->pvpY + testValYe / 2);
			DrawUtils::fillRectangle(vec2_t(centerPoint.x - lineWidth2 / 2, centerPoint.y - testValYe / 2), vec2_t(centerPoint.x + lineWidth2 / 2, centerPoint.y + testValYe / 2));
			DrawUtils::fillRectangle(vec2_t(centerPoint.x - testValYe / 2, centerPoint.y - lineWidth2 / 2), vec2_t(centerPoint.x + testValYe / 2, centerPoint.y + lineWidth2 / 2));

			//xp bottle 384

			if (isThereCrystal()) {
				int gSlot = pullSlot(426);
				DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 1.f) + testValYe / 2, hudEditorMod->pvpY + (lineWidth2 * scale - 1.f + testValYe / 2)), 1.f, scale, true);
				std::string yikes = std::to_string(countItem(426));
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f) + testValYe / 2 + (1.f * scale), hudEditorMod->pvpY + (lineWidth2 * scale + 7.f) + testValYe / 2 + (1.f * scale)), &yikes, MC_Color(0, 0, 0), scale);

				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f) + testValYe / 2, hudEditorMod->pvpY + (lineWidth2 * scale + 7.f) + testValYe / 2), &yikes, MC_Color(255, 255, 255), scale);
			}

			if (isThereTotem()) {
				int gSlot = pullSlot(450);
				DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 1.f), hudEditorMod->pvpY + (lineWidth2 * scale - 1.f + testValYe / 2)), 1.f, scale, true);
				std::string yikes = std::to_string(countItem(450));
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f) + (1.f * scale), hudEditorMod->pvpY + (lineWidth2 * scale + 7.f) + 20.f + (1.f * scale)), &yikes, MC_Color(0, 0, 0), scale);
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f), hudEditorMod->pvpY + (lineWidth2 * scale + 7.f) + 20.f), &yikes, MC_Color(255, 255, 255), scale);
			}

			if (isThereGapples()) {
				int gSlot = pullSlot(466);
				DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 1.f), hudEditorMod->pvpY + (lineWidth2 * scale - 1.f)), 1.f, scale, true);
				std::string yikes = std::to_string(countItem(466));
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f) + (1.f * scale), hudEditorMod->pvpY + (lineWidth2 * scale + 7.f) + (1.f * scale)), &yikes, MC_Color(0, 0, 0), scale);
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f), hudEditorMod->pvpY + (lineWidth2 * scale + 7.f)), &yikes, MC_Color(255, 255, 255), scale);
			}

			if (isThereXp()) {
				int gSlot = pullSlot(384);
				DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 1.f) + testValYe / 2, hudEditorMod->pvpY + (lineWidth2 * scale - 1.f)), 1.f, scale, true);
				std::string yikes = std::to_string(countItem(384));
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f) + (1.f * scale) + testValYe / 2, hudEditorMod->pvpY + (lineWidth2 * scale + 7.f) + (1.f * scale)), &yikes, MC_Color(0, 0, 0), scale);
				DrawUtils::drawText(vec2_t(hudEditorMod->pvpX + (lineWidth2 * scale + 4.f) + testValYe / 2, hudEditorMod->pvpY + (lineWidth2 * scale + 7.f)), &yikes, MC_Color(255, 255, 255), scale);
			}
			if (hudEditorMod->isEnabled()) {
				if ((mousePosition.x >= hudEditorMod->pvpX && mousePosition.x <= hudEditorMod->pvpX + testValYe) && (mousePosition.y >= hudEditorMod->pvpY && mousePosition.y <= hudEditorMod->pvpY + testValYe)) {
					if (!g_Data.isLeftClickDown()) {
						xDiff = mousePosition.x - hudEditorMod->pvpX;
						yDiff = mousePosition.y - hudEditorMod->pvpY;
					}

					if (xDiff <= 0.f) {
						xDiff = 40.f;
					}

					if (yDiff <= 0.f) {
						yDiff = 40.f;
					}
					DrawUtils::setColor(0.7f, 0.7f, 0.7f, 0.8f);
					DrawUtils::fillRectangle(vec2_t(hudEditorMod->pvpX, hudEditorMod->pvpY), vec2_t(hudEditorMod->pvpX + testValYe, hudEditorMod->pvpY + testValYe));

					if (g_Data.isLeftClickDown()) {
						if (mousePosition.x - xDiff >= 1.f) {
							hudEditorMod->pvpX = mousePosition.x - xDiff;
						} else {
							hudEditorMod->pvpX = mousePosition.x;
						}

						if (mousePosition.y - yDiff >= 1.f) {
							hudEditorMod->pvpY = mousePosition.y - yDiff;
						} else {
							hudEditorMod->pvpY = mousePosition.y;
						}  //*/
						   //	pvpX = mousePosition.x;
						   //	pvpY = mousePosition.y;

						DrawUtils::setColor(0, 1, 1, 1);
						DrawUtils::drawLine(vec2_t(0, 0), mousePosition, 1.f);
					}
				}
			}
		}
	}
	{  // render inv
		if (!(g_Data.getLocalPlayer() == nullptr || !this->invRender)) {
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			C_PlayerInventoryProxy* supplies2 = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* inv2 = supplies2->inventory;
			g_Data.getLocalPlayer()->getSupplies()->inventory;
			vec2_t invPos = vec2_t(100.f, 100.f);
			vec2_t invPos2 = vec2_t(100.f, 120.f);
			C_ItemStack* test = inv2->getItemStack(1);
			C_ItemStack* test2 = inv2->getItemStack(2);
			float testF = 100.f;
			float sideBars = 3.f;
			float yVal = hudEditorMod->invY + sideBars;
			
			xVal = hudEditorMod->invX + sideBars;

			//	isThereCrystal();
			//if (isThereCrystal()) {
			//DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(pullSlot(71)), vec2_t(windowSize.x / 2, windowSize.y / 2), 1.f, 1.f, true);

			//	}

			/*for (int j = 0; j < 9; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t((testF + (j * 20.f)), windowSize.y - 30.f), 1.f, 1.f, false);
				}
			}*/

			
			DrawUtils::setColor(0, 0, 0, 1);
			DrawUtils::fillRectangle(vec2_t(hudEditorMod->invX, hudEditorMod->invY), vec2_t(hudEditorMod->invX + 180.f + sideBars, hudEditorMod->invY + 60.f + sideBars));

			DrawUtils::setColor(0.15, 0.15, 0.15, 1);
			DrawUtils::drawRectangle(vec2_t(hudEditorMod->invX, hudEditorMod->invY), vec2_t(hudEditorMod->invX + 180.f + sideBars, hudEditorMod->invY + 60.f + sideBars), sideBars / 2);


			for (int j = 9; j < 18; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal, yVal + 0.f), 1.f, scale, true);
					char stackAmount = inv2->getItemStack(j)->count;
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1)
					DrawUtils::drawText(vec2_t(xVal + 7.5f, (yVal + 0.f) + 7.5f), &stackStr, MC_Color(255, 255, 255), scale);
				}
				xVal += 20.f;
			}
			xVal -= 180.f;
			for (int j = 18; j < 27; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal, yVal + 20.f), 1.f, scale, true);
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1)
					DrawUtils::drawText(vec2_t(xVal + 7.5f, (yVal + 20.f) + 7.5f), &stackStr, MC_Color(255, 255, 255), scale);
				}
				xVal += 20.f;
			}
			xVal -= 180.f;
			for (int j = 27; j < 36; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal, yVal + 40.f), 1.f, scale, true);
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1)
					DrawUtils::drawText(vec2_t(xVal + 7.5f, (yVal + 40.f) + 7.5f), &stackStr, MC_Color(255, 255, 255), scale);
				}
				xVal += 20.f;
			}
			xVal -= 180.f;



			if (hudEditorMod->isEnabled()) {
				if ((mousePosition.x >= hudEditorMod->invX && mousePosition.x <= hudEditorMod->invX + 180.f + sideBars) && (mousePosition.y >= hudEditorMod->invY && mousePosition.y <= hudEditorMod->invY + 60.f + sideBars)) {
					if (!g_Data.isLeftClickDown()) {
						xDiff = mousePosition.x - hudEditorMod->invX;
						yDiff = mousePosition.y - hudEditorMod->invY;
					}

					if (xDiff <= 0.f) {
						xDiff = 40.f;
					}

					if (yDiff <= 0.f) {
						yDiff = 40.f;
					}

					DrawUtils::setColor(0.7f, 0.7f, 0.7f, 0.8f);
					DrawUtils::fillRectangle(vec2_t(hudEditorMod->invX, hudEditorMod->invY), vec2_t(hudEditorMod->invX + 180.f + sideBars, hudEditorMod->invY + 60.f + sideBars));

					if (g_Data.isLeftClickDown()) {
						if (mousePosition.x - xDiff >= 1.f) {
							hudEditorMod->invX = mousePosition.x - xDiff;
						} else {
							hudEditorMod->invX = mousePosition.x;
						}

						if (mousePosition.y - yDiff >= 1.f) {
							hudEditorMod->invY = mousePosition.y - yDiff;
						} else {
							hudEditorMod->invY = mousePosition.y;
						}  //*/
						   //	pvpX = mousePosition.x;
						   //	pvpY = mousePosition.y;

						DrawUtils::setColor(0, 1, 1, 1);
						DrawUtils::drawLine(vec2_t(0, 0), mousePosition, 0.5f);
					}
				}
			}
			//?*/
		}
	}
}

/*int HudModule2::countItem(C_ItemStack* itemC) {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = itemC->getItem()->itemId;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	return mafs;

}*/

bool HudModule2::isThereTotem() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 450;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	} else {
		return true;
	}
}

bool HudModule2::isThereCrystal() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 426;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	} else {
		return true;
	}
}

bool HudModule2::isThereGapples() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 466;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	} else {
		return true;
	}
}

bool HudModule2::isThereXp() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 384;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	} else {
		return true;
	}
}

int HudModule2::countItem(int itemIDCount) {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDCount) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	return mafs;
	//mafs = 0;
}

int HudModule2::pullSlot(int itemIDPog) {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDPog) {
			return i;
		}
	}
}

void HudModule2::onTick(C_GameMode* gm) {
}

void HudModule2::onDisable() {
}
