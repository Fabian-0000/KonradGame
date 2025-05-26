#include "SaveData.hpp"

#include <fstream>
#include <json.hpp>

using namespace nlohmann;

bool IsSaveData(const char* path) {
	std::ifstream inputFile;
	inputFile.open(path);

	if (!inputFile.good()) {
		inputFile.close();
		return false;
	}

	inputFile.close();

	inputFile.open(path);

	json in;

	if (inputFile.is_open()) {
		in = json::parse(inputFile, nullptr, false);
		inputFile.close();
	}

	if (!in.contains("hasShotgun") || !in["hasShotgun"].is_boolean()) return false;
	if (!in.contains("level") || !in["level"].is_number_unsigned()) return false;
	if (!in.contains("hpCount") || !in["hpCount"].is_number_unsigned()) return false;

	return true;
}

void LoadSaveData(const char* path) {
	std::ifstream inputFile;
	inputFile.open(path);

	json in;

	if (inputFile.is_open()) {
		in = json::parse(inputFile, nullptr, false);
		inputFile.close();
	}

	if (!in.contains("hasShotgun") || !in["hasShotgun"].is_boolean()) return;
	if (!in.contains("level") || !in["level"].is_number_unsigned()) return;
	if (!in.contains("hpCount") || !in["hpCount"].is_number_unsigned()) return;

	g_SaveData.hasShotgun = in["hasShotgun"];
	g_SaveData.level = std::clamp((int)in["level"], 0, 255);
	g_SaveData.hpCount = std::clamp((int)in["hpCount"], 0, 255);
}

void WriteSaveData(const char* path) {
	json out;
	out["hasShotgun"] = g_SaveData.hasShotgun;
	out["level"] = g_SaveData.level;
	out["hpCount"] = g_SaveData.hpCount;

	std::ofstream outputFile;
	outputFile.open(path);
	if (outputFile.is_open()) {
		outputFile << out;
		outputFile.close();
	}
}

SaveData g_SaveData;