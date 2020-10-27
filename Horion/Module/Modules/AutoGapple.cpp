#include "AutoGapple.h"

int currHealth = 20;
void eat();

AutoGapple::AutoGapple() : IModule(0x0, Category::PLAYER, "Auto eat gapples if you're low health") {
	registerIntSetting("health", &this->health, this->health, 1, 36);
	//registerBoolSetting("On Equal health", &this->SmolEqual, this->SmolEqual);
	registerBoolSetting("Pref enchanted", &this->PrefEnchant, this->PrefEnchant);
	//registerBoolSetting("Check Inv", &this->CheckInv, this->CheckInv);
	registerBoolSetting("Select", &this->Selected, this->Selected);
	//Get player health offset(0x??...??C4)
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}
void AutoGapple::onTick(C_GameMode* gm) {
	if (AutoGapple::CheckInv && AutoGapple::Selected) {
		Selected = false;
	}
	currHealth == 20;	//replace 20 w/ value from Memory(0x??...??C4)
	/*if (SmolEqual) {
		if (health >= currHealth)
			AutoGapple::eat();
	} else if (health > currHealth)*/
		AutoGapple::eat();
	;}
//eat a gapple
void AutoGapple::eat() {
	//loop Hotbar
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int slot = supplies->selectedHotbarSlot;
	int prevSlot = slot;
	int max;
	if (!AutoGapple::CheckInv)
		max = 9;
	else
		max = 36; 
	for (int n = 0; n < max; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 322){
				if (Selected) {
					slot = n;
					supplies->selectedHotbarSlot = slot;
				}
#ifdef _UseDone
				C_Item::use(stack&, g_Data.getLocalPlayer()&);
#endif
				/*if (Selected)
					supplies->selectedHotbarSlot = prevSlot;*/
				if (PrefEnchant)
					return;
			}else if ((*stack->item)->itemId == 466) {
				if (Selected) {
					slot = n;
					supplies->selectedHotbarSlot = slot;
				}
#ifdef _UseDone
				C_Item::use(stack&, g_Data.getLocalPlayer()&);
#endif
				/*if (Selected)
					supplies->selectedHotbarSlot = prevSlot;*/
				if (PrefEnchant)
					return;
			}
		}
	} 
}
