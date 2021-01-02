#pragma once
#include "../../../SDK/CItem.h"

class ArmorStruct {
public:
	ArmorStruct(C_ItemStack* item, C_ArmorItem* yot, int slot) {
		armor = yot;
		m_slot = slot;
		m_item = item;
	}
	bool isEqual(ArmorStruct& src) {
		if (this->m_item->getArmorValueWithEnchants() == src.m_item->getArmorValueWithEnchants())
			return true;
		else
			return false;
	}

	bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
		return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
	}
	C_ArmorItem* armor = nullptr;
	C_ItemStack* m_item = nullptr;
	int m_slot = 0;
};