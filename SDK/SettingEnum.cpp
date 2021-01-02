#include "SettingEnum.h"

#pragma region EnumEntry 
EnumEntry::EnumEntry(std::string _name, unsigned char value) {
	name = /*(const std::string)*/ _name;
	val = /*(const unsigned char)*/ value;
}
AddResult::AddResult(bool s, int Pos) {
	success = s;
	pos = Pos;
}
#pragma endregion
#pragma region SettingEnum
SettingEnum::SettingEnum(std::vector<EnumEntry*> entr, IModule* mod) {
	Entrys = entr;
	owner = mod;
	std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
		return rhs.GetValue() < lhs.GetValue();
	});
}

AddResult SettingEnum::addEntry(EnumEntry* entr) {
	Entrys.push_back(entr);
	std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
		return rhs.GetValue() < lhs.GetValue();
	});
	bool SameVal = false;
	int i = 0;
	int e = -1;
	for (auto it = this->Entrys.begin(); it != this->Entrys.end(); it++, i++) {
		if (entr == (*it._Ptr))
			e = 1;
		else
			SameVal = (*it._Ptr)->GetValue() == entr->GetValue() || SameVal;
	}
	return *new AddResult(!SameVal, e);
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
#pragma endregion