#pragma once
#include "Module.h"
class AntiBot : public IModule {
private:
	const bool nameCheck = false;
	bool hitboxCheck = true;
	bool invisibleCheck = false;
	// entId == 63
	bool entityIdCheck = true;
	// Nametag Checks
	bool NametagCheck = true;
	bool NewlineCheck = true;
	bool TextCheck = true;
	// Other Checks
	bool silentCheck = true;
	bool ImobileCheck = true;
	// entId != (64 || 66 || 69(nice))
	bool LagCheck = true;
	bool AtackCheck = true;
	bool VanilaCheck = true;

public:
	AntiBot();
	~AntiBot();

	inline bool isHitboxCheckEnabled() {
		return hitboxCheck && this->isEnabled();
	}
	inline bool isNameCheckEnabled() {
		return nameCheck && this->isEnabled();
	}
	inline bool isInvisibleCheckEnabled() {
		return invisibleCheck && this->isEnabled();
	}
	inline bool isEntityIdCheckEnabled() {
		return entityIdCheck && this->isEnabled();
	}
	inline bool isNametagCheckEnabled() {
		return NametagCheck && this->isEnabled();
	}
	inline bool isNewlineCheckEnabled() {
		return NewlineCheck && this->isEnabled();
	}
	inline bool isTextCheckEnabled() {
		return TextCheck && this->isEnabled();
	}
	inline bool isSilentCheckEnabled() {
		return silentCheck && this->isEnabled();
	}
	inline bool isImobileCheckEnabled() {
		return ImobileCheck && this->isEnabled();
	}
	inline bool isLagCheckEnabled() {
		return this->LagCheck && this->isEnabled();
	}
	inline bool isAttackCheckEnabled() {
		return this->AtackCheck && this->isEnabled();
	}
	inline bool isVanilaCheckEnabled() {
		return this->VanilaCheck && this->isEnabled();
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
