#pragma once

#include <vector>

#include "../SDK/CEntity.h"

class Target {
private:
	static bool containsOnlyASCII(const std::string& string);
public:
	static void init(C_LocalPlayer** lc);
	static bool isValidTarget(C_Entity* ent);
	static bool VanillaAttac(C_Entity* ent, bool inclLeash = false);
	static bool VanillaAttac(int id, bool inclLeash = false);
	static bool AntiLag(C_Entity* ent);
};
