#pragma once
#include <string>
#include <vector>
#include "../Horion/Module/Modules/Module.h"
#include "../Horion/Module/ModuleManager.h"

//struct AddResult {
//public:
//	bool success;
//	int pos;
//	AddResult(bool s, int Pos);
//};

struct EnumEntry {
public:
	const std::string name;
	const unsigned char val;
	/// <summary>Use this however you want</summary>
	void* ptr = nullptr;
	EnumEntry(const std::string _name, const unsigned char value);
	inline std::string GetName();
	inline unsigned char GetValue();
};

class SettingEnum {
private:
	IModule* owner/* = nullptr*/;

public:
	std::vector<EnumEntry*> Entrys;
	int selected = -1;

	SettingEnum(std::vector<EnumEntry*> entr, IModule* mod = nullptr);
	SettingEnum(IModule* mod = nullptr);
	//SettingEnum();
	bool addEntry(EnumEntry entr);
	EnumEntry* GetEntry(int ind);
	EnumEntry* GetEntry();
	int GetCount();
};