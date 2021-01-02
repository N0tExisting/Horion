#pragma once
#include <string>
#include <vector>
#include "../Horion/Module/ModuleManager.h"

struct AddResult {
public:
	bool success;
	int pos;
	AddResult(bool s, int Pos);
};

struct EnumEntry {
private:
	std::string name;
	unsigned char val;
public:
	/// <summary>Use this however you want</summary>
	void* ptr = nullptr;
	EnumEntry(std::string _name, unsigned char value);
	const std::string GetName() { return name; }
	const unsigned char GetValue() { return val; }
};

class SettingEnum {
private:
	std::vector<EnumEntry*> Entrys;
	IModule* owner = nullptr;

public:
	int selected = -1;
	SettingEnum(std::vector<EnumEntry*> entr, IModule* mod = nullptr);
	//SettingEnum(EnumEntry* entr[], IModule* mod = nullptr);
	AddResult addEntry(EnumEntry* entr);
	//AddResult* addEntry(EnumEntry* entrs[]);
	EnumEntry* GetEntry(int ind);
	EnumEntry* GetEntry();
	int GetCount() { return (int)Entrys.size(); }
};