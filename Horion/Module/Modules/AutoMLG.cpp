#include "AutoMLG.h"
#include "../ModuleManager.h"
#include "../../../Utils/Logger.h"

AutoMLG::AutoMLG() : IModule(0, Category::PLAYER, "Automaticaly does a MLG for you") {
	registerFloatSetting("min Distance", &this->MLGdist, this->MLGdist, 2.f, 23.f);
	registerBoolSetting("Search Inv", &this->inv, this->inv);
	//registerBoolSetting("Horn", &this->horn, this->horn);
}
AutoMLG::~AutoMLG() {
}
const char* AutoMLG::getModuleName() {
	return "AutoMLG";
}

void AutoMLG::onTick(C_GameMode* gm) {
	//Bucket MLG
	static bool bMLG = false;
	//if ()
	if (inv && false) {
		vec3_t pos = gm->player->aabb.centerPoint();
		pos.y = gm->player->aabb.lower.y - 1.f;
		vec3_ti bPos = pos.floor();
		C_Block* block = gm->player->region->getBlock(bPos);
		if (block != nullptr) {
			C_Material* mat = block->toLegacy()->material;
			if (mat != nullptr) {
				if (mat->isBlockingMotion)
					logF("Blocking motion");
				else
					logF("not blocking motion");
				if (mat->isBlockingPrecipitation)
					logF("blocking precipitapion");
				else
					logF("not blocking precipitapion");
				logF("Mat Type: %i", mat->type);
			}
		}
		//auto supplies = g_Data.getLocalPlayer()->getSupplies();  //->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot
		//if (supplies != nullptr) {
		//	if (supplies->inventory != nullptr) {
		//		C_ItemStack* stack = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
		//			if (stack->getItem() != nullptr) {
		//				logF("%i", stack->getItem()->itemId);
		//				return;
		//			}
		//	}
		//}
	}
	if (gm->player->fallDistance <= MLGdist) return;
	C_Block* block = nullptr;
	vec3_ti max = vec3_t(0.f, 256.f, 0.f);
	float maxY = 0;
	float dist;
	vec3_t pPos = vec3_t{gm->player->aabb.centerPoint().x, gm->player->aabb.lower.y, gm->player->aabb.centerPoint().z};
	std::vector<vec3_ti> corners;
	corners.push_back(gm->player->aabb.lower.floor());
	corners.push_back(vec3_t(gm->player->aabb.upper.x, gm->player->aabb.lower.y, gm->player->aabb.upper.z).floor());
	corners.push_back(vec3_t(gm->player->aabb.lower.x, gm->player->aabb.lower.y, gm->player->aabb.upper.z).floor());
	corners.push_back(vec3_t(gm->player->aabb.upper.x, gm->player->aabb.lower.y, gm->player->aabb.lower.z).floor());
	for (size_t i = 0; i < 4; i++) {
		bool found = false;
		for (int* v = &corners.at(i).y; *v > 0; (*v)--) {
			if (found || *v < maxY) continue;
			C_Block* b = gm->player->region->getBlock(corners.at(i));
			int bID = b->toLegacy()->blockId;
			if ((!(bID == 0 || bID == 8 || bID == 9))/* && */) {
				block = b;
				max = corners.at(i);
				found = true;
				dist = gm->player->fallDistance + *v;
				switch (i) {
					case 0:
						DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
						break;
					case 1:
						DrawUtils::setColor(1.f, 1.f, 0.f, 1.f);
						break;
					case 2:
						DrawUtils::setColor(0.f, 1.f, 0.f, 1.f);
						break;
					case 3:
						DrawUtils::setColor(0.f, 0.f, 1.f, 1.f);
						break;
					default:
						DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);
						break;
				}
				DrawUtils::drawBox(max.toVec3t(), max.add(1).toVec3t(), 1.5f);
			}
			//corners.at(i).add(0.f, 1.f, 0.f);
		}
	}
	float reach = moduleMgr->getModule<ExtendedBlockReach>()->getBlockReach()
				+ moduleMgr->getModule<Reach>()->GetCurrentReach() - 3.f;
	if (block != nullptr && max.toVec3t().dist(gm->player->eyePos0) < reach
		&& max.add(1).toVec3t().dist(gm->player->eyePos0) < reach) {
		tryMLG(max.toVec3t());
	}
}

bool AutoMLG::tryMLG(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();
	logF("Tried to MLG");
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = *(block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);
		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			if (!(*(g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			findItem();
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		}
	}
	return false;
}

short AutoMLG::isMLG(C_ItemStack* i) {
	if (!i->isValid())
		return false;
	int id = i->getItem()->itemId;
	//if (id == 325 && damage == 8)
	//	return true;
	if (id == 165)
		return true;
	logF("%i", id);
	return 0;
}

bool AutoMLG::findItem() {
	//logF("Finding Item");

	static C_ItemStack* emptyItemStack = nullptr;
	if (emptyItemStack == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 3D ? ? ? ? 80 B8 ? ? ? ? ? 75 19 48 8B 88 ? ? ? ? 48 8B 11 4C 8B 42 28 8B 50 10");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		emptyItemStack = reinterpret_cast<C_ItemStack*>(sigOffset + offset + /*length of instruction*/ 7);
	}

	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	if (isMLG(inv->getItemStack(supplies->selectedHotbarSlot))) return true;

	short found = 0;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (isMLG(stack) && (*stack->item)->itemId != 0) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	if (this->inv && false) {
		bool found = false;
		for (int i = 9; i < 36; i++) {
			auto c = *g_Data.getLocalPlayer()->getTransactionManager();
			int slot = findSlot();
			int to = inv->getFirstEmptySlot();
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nullptr) {
				if (isMLG(stack) && (*stack->item)->itemId != 0) {
					C_ItemStack* swap = inv->getItemStack(slot);

#ifdef DEBUG

					C_InventoryAction* first = new C_InventoryAction(i, swap, nullptr, 632);
					C_InventoryAction* last = new C_InventoryAction(to, nullptr, swap);

					if (emptyItemStack != nullptr)
						*inv->getItemStack(i) = *emptyItemStack;
					*inv->getItemStack(to) = *swap;

					c.addInventoryAction(*first);
					c.addInventoryAction(*last);

					delete first;
					delete last;

					first = new C_InventoryAction(slot, nullptr, stack);
					last = new C_InventoryAction(i, stack, nullptr, 632);

					if (emptyItemStack != nullptr)
						*inv->getItemStack(i) = *emptyItemStack;
					*inv->getItemStack(slot) = *swap;

					c.addInventoryAction(*first);
					c.addInventoryAction(*last);

					delete first;
					delete last;

					auto b = C_ComplexInventoryTransaction(c.transac);
					auto a = C_InventoryTransactionPacket(&b);

					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);

#else

					C_InventoryAction* first = new C_InventoryAction(i, swap, stack, 632, 0);
					C_InventoryAction* last = new C_InventoryAction(slot, stack, swap, 0, 632);

					*inv->getItemStack(i) = *stack;
					*inv->getItemStack(slot) = *swap;

					c.addInventoryAction(*first);
					c.addInventoryAction(*last);

					delete first;
					delete last;

					//first = new C_InventoryAction(slot, nullptr, stack);
					//last = new C_InventoryAction(i, stack, nullptr, 632);
					//
					//c.addInventoryAction(*first);
					//c.addInventoryAction(*last);
					//
					//delete first;
					//delete last;

					auto b = C_ComplexInventoryTransaction(c.transac);
					auto a = C_InventoryTransactionPacket(&b);

					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
#endif
					for (int n = 0; n < 9; n++) {
						C_ItemStack* stack = inv->getItemStack(n);
						if (stack->item != nullptr) {
							if (isMLG(stack) && (*stack->item)->itemId != 0) {
								C_MobEquipmentPacket a(id, *stack, n, n);
								g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
								return true;
							}
						}
					}
				}
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

int AutoMLG::findSlot(/*bool* wasEmpty*/) {
	return std::max(8, g_Data.getLocalPlayer()->getSupplies()->inventory->getFirstEmptySlot());
}