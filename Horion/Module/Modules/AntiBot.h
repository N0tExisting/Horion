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
	bool extraCheck = false;
	// Other Checks
	bool otherCheck = true;
	// entId != (64 || 66 || 69(nice))
	bool LagCheck = true;

public:
	AntiBot();
	~AntiBot();

	bool isHitboxCheckEnabled() {
		return hitboxCheck && this->isEnabled();
	}
	bool isNameCheckEnabled() {
		return nameCheck && this->isEnabled();
	}
	bool isInvisibleCheckEnabled() {
		return invisibleCheck && this->isEnabled();
	}
	bool isEntityIdCheckEnabled() {
		return entityIdCheck && this->isEnabled();
	}
	bool isOtherCheckEnabled() {
		return otherCheck && this->isEnabled();
	}
	bool isExtraCheckEnabled() {
		return this->extraCheck && this->isEnabled();
	}
	bool isLagCheckEnabled() {
		return this->LagCheck && this->isEnabled();
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
