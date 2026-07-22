#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return KamataEngine::Vector3(KBlockWidth * xIndex, KBlockHeight * yIndex, 0.0f); }

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"1", MapChipType::kBlock},
    {"0", MapChipType::kBlank}
};
}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(KNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(KNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	for (uint32_t i = 0; i < KNumBlockVertical; ++i) {
		std::string line;
		if (!getline(mapChipCsv, line))
			break;
		std::istringstream lineStream(line);
		for (uint32_t j = 0; j < KNumBlockHorizontal; ++j) {
			std::string word;
			if (std::getline(lineStream, word, ',')) {
				if (mapChipTable.contains(word)) {
					mapChipData_.data[i][j] = mapChipTable[word];
				}
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(int32_t xIndex, int32_t yIndex) {
	if (xIndex < 0 || static_cast<int32_t>(KNumBlockHorizontal) - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || static_cast<int32_t>(KNumBlockVertical) - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>(position.x / KBlockWidth);
	indexSet.yIndex = static_cast<uint32_t>(position.y / KBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	// 指定ブロックの中心座標を取得する
	KamataEngine::Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - KBlockWidth / 2.0f;
	rect.right = center.x + KBlockWidth / 2.0f;
	rect.bottom = center.y - KBlockHeight / 2.0f;
	rect.top = center.y + KBlockHeight / 2.0f;

	return rect;
}