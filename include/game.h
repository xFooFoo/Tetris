#pragma once
#include "grid.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "..\src\blocks.cpp"

class Game {
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	bool MoveBlockDown(); //Returns true or false on whether the block has been locked in place
	int score;
	double lastSystemUpdateTime;
	double lastUserUpdateTime;
	bool SystemEventTriggered(double interval);
	bool gameOver;
	bool held; //true if a player has held a new piece without placing down one
	bool canUseHold; //true is a player has placed a piece while holding another, replenishes their hold use;
	bool isPaused;
	void Reset();
	Music music;
	std::string name;
	void AddHighScore(std::string name);
	bool isAddingHighScore;
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
	void HardDrop();
	bool IsBlockOutside();
	void RotateBlockRight();
	void RotateBlockLeft();
	void LockBlock();
	void UpdateScore(int rowsCleared, int moveDownPoints);
	bool BlockFits();
	bool UserInputTriggered(double interval);
	void UpdateSystemTime();
	Sound rotateSound;
	Sound clearSound;
	Sound holdSound;
	Sound hardDropSound;
};

