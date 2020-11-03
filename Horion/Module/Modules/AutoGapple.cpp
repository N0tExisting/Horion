#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0x0, Category::PLAYER, "Auto eat gapples if you're low health") {
	registerIntSetting("health", &this->health, this->health, 1, 35);
	registerBoolSetting("Pref enchanted", &this->PrefEnchant, this->PrefEnchant);
	//Get player health offset(0x??...??C4)
}
C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

void AutoGapple::onEnable() {
	/*prevSlot = supplies->selectedHotbarSlot;
	slot = PrepareEat();
	if (!eating)
		AutoGapple::clientMessageF("§1Coud not find Gapple. Make sure you have one in your hotbar");*/
}

void AutoGapple::onTick(C_GameMode* gm) {
	currHealth = 20;	//replace 20 w/ value from Memory(0x??...??C4)
	if (health >= currHealth && !eating)
		AutoGapple::PrepareEat();
	if (eating)
		//AutoGapple::Eat();
		supplies->selectedHotbarSlot = slot;
}

void AutoGapple::onDisable() {
	supplies->selectedHotbarSlot = prevSlot;
}
//Prepare a gapple
int AutoGapple::PrepareEat() {
	//loop Hotbar
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int slot = supplies->selectedHotbarSlot;
	prevSlot = slot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 322) {
				slot = n;
				//supplies->selectedHotbarSlot = slot;
#ifdef _UseDone
				(*stack->item)->C_Item::use(stack&, g_Data.getLocalPlayer()&);
#endif
				if (PrefEnchant) {
					eating = true;
					return slot;
				}
			} else if ((*stack->item)->itemId == 466) {
				slot = n;
				//supplies->selectedHotbarSlot = slot;
#ifdef _UseDone
				(*stack->item)->C_Item::use(stack&, g_Data.getLocalPlayer()&);
#endif
				if (PrefEnchant) {
					eating = true;
					return slot;
				}
			}
		}
	}
	return slot;
}

void AutoGapple::Eat() {
	/*supplies->selectedHotbarSlot = prevSlot;*/
	eating = false;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	//supplies->selectedHotbarSlot = prevSlot;
}