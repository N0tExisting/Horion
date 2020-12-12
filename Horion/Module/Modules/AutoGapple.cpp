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

void AutoGapple::onTick(C_GameMode* gm) {
	currHealth = 20;  //replace 20 w/ value from Memory(0x??...??C4)
	if (supplies == nullptr)
		supplies = g_Data.getLocalPlayer()->getSupplies();
	if (inv == nullptr)
		inv = supplies->inventory;
	if (health >= currHealth && !eating) {
		AutoGapple::PrepareEat(gm);
	}else if (eating) {
		supplies->selectedHotbarSlot = slot;
		AutoGapple::Eat(gm);
	}
}

void AutoGapple::onDisable() {
	supplies->selectedHotbarSlot = prevSlot;
}
//Prepare a gapple
void AutoGapple::PrepareEat(C_GameMode* gm) {
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
			} else if ((*stack->item)->itemId == 466) {
				slot = n;
				eatingTime = (*stack->item)->duration;
					eating = true;
				if (PrefEnchant)
					return;
			}
		}
	}
}

void AutoGapple::Eat(C_GameMode* gm) {
	if (timeEating < eatingTime + lagComp)
		eating = false;
	C_ItemStack* stack = inv->getItemStack(slot);
	(*stack->item)->C_Item::use(*stack, *gm->player);
	timeEating++;
}