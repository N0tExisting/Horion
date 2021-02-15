#pragma once
#include "Module.h"
class Freecam : public IModule {
private:
	vec3_t oldPos;
	//C_LocalPlayer* old = {};
	//C_MovePlayerPacket last;
	//PlayerAuthInputPacket Last;

public:
	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	//virtual void onSendPacket(C_Packet*);
	virtual void onEnable() override;
	virtual void onDisable() override;
	//virtual bool callWhenDisabled(){return true;}


	inline vec3_t getOldPos(){return oldPos;}
	//inline C_LocalPlayer getOld(){return old;}
	//inline C_MovePlayerPacket getLastMoveP(){return last;}
	//inline PlayerAuthInputPacket getLastAuthP(){return Last;}
};