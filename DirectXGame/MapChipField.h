#pragma once
#include <KamataEngine.h>
#include <cstdint>
#include <string>
#include <vector>

enum class MapChipType { kBlank, kBlock };

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
private:
	MapChipData mapChipData_;

public:
	static inline const float KBlockWidth = 2.0f;           // ブロックの幅
	static inline const float KBlockHeight = 2.0f;          // ブロックの高さ
	static inline const uint32_t KNumBlockHorizontal = 100; // 横方向のブロック数
	static inline const uint32_t KNumBlockVertical = 20;    // 縦方向のブロック数

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filename);
	MapChipType GetMapChipTypeByIndex(int32_t xIndex, int32_t yIndex);

	// GameScene側で使うためのゲッター関数
	uint32_t GetNumBlockVertical() const { return KNumBlockVertical; }
	uint32_t GetNumBlockHorizontal() const { return KNumBlockHorizontal; }

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);
};