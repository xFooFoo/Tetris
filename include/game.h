#pragma once
#include "grid.h"
#include <vector>
#include "..\src\blocks.cpp"

class Game {
public:
	Game();
	Block GetRandomBlock();
	std::vector<Block> GetAllBlocks();
	void Draw();
	void HandleInput();
	void MoveBlockLeft();
	void MoveBlockRight();
	void MoveBlockDown();
	Grid grid;
	int score;
	bool gameOver;
	void Reset();
private:
	bool IsBlockOutside();
	void RotateBlockRight();
	void RotateBlockLeft();
	void LockBlock();
	bool BlockFits();
	std::vector<Block> blocks;
	Block currentBlock;
	Block nextBlock;
};

