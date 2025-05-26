#include <cstdint>

struct SaveData {
	bool hasShotgun = false;
	uint8_t level = 0;
	uint8_t hpCount = 0; // 0 => infinite
};

bool IsSaveData(const char* path);

void LoadSaveData(const char* path);

void WriteSaveData(const char* path);

extern SaveData g_SaveData;