#include "..\include\game.h"
#include <random>

Game::Game() {
	grid = Grid();
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	score = 0;
	gameOver = false;
}

Block Game::GetRandomBlock() {
	if (blocks.empty()) {
		blocks = GetAllBlocks();
	}
	int randomIndex = rand() % blocks.size();
	Block block = blocks[randomIndex];
	blocks.erase(blocks.begin() + randomIndex);
	return block;
}

std::vector<Block> Game::GetAllBlocks() {
	return { IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock() };
}

void Game::Draw() {
	grid.Draw();
	currentBlock.Draw();
}

// Create hard drop and hold mechanics
void Game::HandleInput() {
	int keyPressed = GetKeyPressed();

	if (gameOver && keyPressed) {
		Reset();
	}
	switch (keyPressed) {
	case KEY_LEFT:
		MoveBlockLeft();
		break;
	case KEY_RIGHT:
		MoveBlockRight();
		break;
	case KEY_DOWN:
		MoveBlockDown();
		break;
	case KEY_UP:
	case KEY_X:
		RotateBlockRight();
		break;
	case KEY_Z:
		RotateBlockLeft();
		break;
	}
}

bool Game::IsBlockOutside()
{
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		if (grid.IsCellOutside(item.row, item.col)) {
			return true;
		}
	}
	return false;
}

bool Game::BlockFits()
{
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		if (!grid.IsCellEmpty(item.row, item.col)) {
			return false;
		}
	}
	return true;
}

void Game::LockBlock()
{
	//set the block on the grid for drawing
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		grid.grid[item.row][item.col] = currentBlock.id;
	}
	//Whenever something new locks, we have to check if a full row is cleared!
	score = grid.ClearFullRows();
	//update the currentBlock to the new block as we no longer need the old one!
	currentBlock = nextBlock;
	nextBlock = GetRandomBlock();
	if (!BlockFits()) {
		gameOver = true;
	}
}

void Game::MoveBlockLeft() {
	if (!gameOver) {
		currentBlock.Move(0, -1);
		if (IsBlockOutside() || !BlockFits()) {
			//Revert movement
			currentBlock.Move(0, 1);
		}
	}
}

void Game::MoveBlockRight() {
	if (!gameOver) {
		currentBlock.Move(0, 1);
		if (IsBlockOutside() || !BlockFits()) {
			//Revert movement
			currentBlock.Move(0, -1);
		}
	}
}
//Only place where we should be locking the block! Otherwise Blocks would lock on 
//hitting the frame edges when there are space below!
void Game::MoveBlockDown() {
	if (!gameOver) {
		currentBlock.Move(1, 0);
		if (IsBlockOutside() || !BlockFits()) {
			//Revert movement
			currentBlock.Move(-1, 0);
			//Lock block in place and generates a new block!
			LockBlock();
		}
	}
}

void Game::RotateBlockRight()
{
	if (!gameOver) {
		currentBlock.RotateRight();
		if (IsBlockOutside()) {
			//Only undo movement, we don't lock the block in place here
			currentBlock.RotateLeft();
		}
	}
}

void Game::RotateBlockLeft()
{
	if (!gameOver) {
		currentBlock.RotateLeft();
		if (IsBlockOutside()) {
			//Only undo movement, we don't lock the block in place here
			currentBlock.RotateRight();
		}
	}
}

void Game::Reset() {
	gameOver = false;
	//Reset to starting values
	grid.Initialize();
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	score = 0;
}


