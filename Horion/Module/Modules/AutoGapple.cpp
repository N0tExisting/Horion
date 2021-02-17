#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0x0, Category::PLAYER, "Auto eat gapples if you're low health") {
	registerIntSetting("health", &this->health, this->health, 1, 35);
	registerIntSetting("Lag comp", &this->lagComp, this->lagComp, 1, 50);
	registerBoolSetting("Pref enchanted", &this->PrefEnchant, this->PrefEnchant);
	//Get player health offset(0x??...??C4)
}
AutoGapple::~AutoGapple() {
}
const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

void AutoGapple::onEnable() {
	eating = false;
	timeEating = 0;
}

void AutoGapple::preTick(C_GameMode* gm) {
	currHealth = 20;  //replace 20 w/ value from Memory(0x??...??C4)
	if (click == nullptr)
		click = *g_Data.getHIDController();
	if (supplies == nullptr)
		supplies = g_Data.getLocalPlayer()->getSupplies();
	if (inv == nullptr)
		inv = supplies->inventory;

	wasClick = click->rightClickDown;
	if (health >= currHealth && !eating)
		AutoGapple::PrepareEat();
	 else if (eating)
		AutoGapple::Eat();
}

void AutoGapple::onTick(C_GameMode* gm) {
	if (shouldDisable) {
		shouldDisable = false;
		this->setEnabled(false);
		return;
	}
	click->rightClickDown = wasClick;
}

void AutoGapple::onDisable() {
	if (prevSlot > -1 && prevSlot < 10)
		supplies->selectedHotbarSlot = prevSlot;
	click->rightClickDown = wasClick;
}
//Prepare a gapple
void AutoGapple::PrepareEat() {
	bool found = false;
	//loop Hotbar
	prevSlot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 322) {
				slot = n;
				eatingTime = (*stack->item)->duration;
				eating = true;
				if (!PrefEnchant)
					return;
				found = true;
			} else if ((*stack->item)->itemId == 466) {
				slot = n;
				eatingTime = (*stack->item)->duration;
				eating = true;
				if (PrefEnchant)
					return;
				found = true;
			}
		}
	}
}

void AutoGapple::Eat() {
	if (timeEating < eatingTime + lagComp) {
		eating = false;
		shouldDisable = true;
		stack = nullptr;
		return;
	}
	supplies->selectedHotbarSlot = slot;
	click->rightClickDown = true;
	if (stack == nullptr)
		stack = inv->getItemStack(slot);
	timeEating++;
}