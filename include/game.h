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
	void HoldBlock();
	Grid grid;
	int score;
	bool gameOver;
	bool held; //true if a player has held a new piece without placing down one
	bool canUseHold; //true is a player has placed a piece while holding another, replenishes their hold use;
	void Reset();
private:
	bool IsBlockOutside();
	void RotateBlockRight();
	void RotateBlockLeft();
	void LockBlock();
	void UpdateScore(int rowsCleared, int moveDownPoints);
	bool BlockFits();
	std::vector<Block> blocks;
	Block currentBlock;
	Block nextBlock;
	Block heldBlock;
};

