#include "SettingEnum.h"

#pragma region EnumEntry 
EnumEntry::EnumEntry(const std::string _name, const unsigned char value) {
	name = _name;
	val = value;
}
inline std::string EnumEntry::GetName() {
	return name;
}
inline unsigned char EnumEntry::GetValue() {
	return val;
}
//AddResult::AddResult(bool s, int Pos) {
//	success = s;
//	pos = Pos;
//}
#pragma endregion
#pragma region SettingEnum
SettingEnum::SettingEnum(std::vector<EnumEntry*> entr, IModule* mod) {
	Entrys = entr;
	owner = mod;
	std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
		return rhs.GetValue() < lhs.GetValue();
	});
}
SettingEnum::SettingEnum(IModule* mod) {
	//std::vector<EnumEntry*> entr;
	//Entrys = entr;
	owner = mod;
	//std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
	//	return rhs.GetValue() < lhs.GetValue();
	//});
}
//SettingEnum::SettingEnum() {
//	owner = nullptr;
//}

bool SettingEnum::addEntry(EnumEntry entr) {
	auto etr = new EnumEntry(entr);
	bool SameVal = false;
	for (auto it = this->Entrys.begin(); it != this->Entrys.end(); it++) {
			SameVal = (*it._Ptr)->GetValue() == etr->GetValue() || SameVal;
	}
	if (!SameVal) {
		Entrys.push_back(etr);
		std::sort(Entrys.begin(), Entrys.end(),
		  [](EnumEntry rhs, EnumEntry lhs) {
			return rhs.GetValue() < lhs.GetValue();
		});
	}
	return SameVal;
}
//AddResult* SettingEnum::addEntry(EnumEntry* entrs[]) {
//	AddResult results[sizeof(entrs)]{};
//	for (size_t i = 0; i < sizeof(entrs); i++) {
//		Entrys.push_back(entrs[i]);
//	}
//	std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
//		return rhs.GetValue() < lhs.GetValue();
//	});
//	for (size_t o = 0; o < sizeof(entrs); o++) {
//		bool SameVal = false;
//		int i = 0;
//		int e = -1;
//		for (auto it = this->Entrys.begin(); it != this->Entrys.end(); it++, i++) {
//			if (entrs[o] == (*it._Ptr))
//				e = 1;
//			else
//				SameVal = (*it._Ptr)->GetValue() == entrs[o]->GetValue() || SameVal;
//		}
//		results[o] = AddResult(!SameVal, e);
//	}
//	return results;
//}
EnumEntry* SettingEnum::GetEntry(int ind) {
	try {
		return Entrys.at(ind);
	} catch (const std::exception&) {
		return nullptr;
	}
}
EnumEntry* SettingEnum::GetEntry() {
	if (selected == -1)
		return nullptr;
	else
		return GetEntry(selected);
}
int SettingEnum::GetCount() {
	return (int)Entrys.size();
}
#pragma endregion