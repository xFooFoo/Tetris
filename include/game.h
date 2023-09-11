#pragma once
#include "grid.h"
#include <vector>
#include "..\src\blocks.cpp"

class Game {
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	void MoveBlockDown();
	int score;
	bool gameOver;
	bool held; //true if a player has held a new piece without placing down one
	bool canUseHold; //true is a player has placed a piece while holding another, replenishes their hold use;
	void Reset();
	Music music;
private:
	Grid grid;
	std::vector<Block> blocks; //7 Tetrominos stored here
	std::vector<Block> GetAllBlocks(); //Returns all 7 Tetrominos
	Block currentBlock;
	Block nextBlock;
	Block heldBlock;
	Block GetRandomBlock();
	void MoveBlockLeft();
	void MoveBlockRight();
	void HoldBlock();
	bool IsBlockOutside();
	void RotateBlockRight();
	void RotateBlockLeft();
	void LockBlock();
	void UpdateScore(int rowsCleared, int moveDownPoints);
	bool BlockFits();
	Sound rotateSound;
	Sound clearSound;
};

