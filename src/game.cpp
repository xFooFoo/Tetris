#include "..\include\game.h"
#include <random>

Game::Game() {
	grid = Grid();
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	score = 0;
	lastSystemUpdateTime = 0;
	lastUserUpdateTime = 0;
	gameOver = false;
	held = false;
	canUseHold = true;
	isPaused = true;
	InitAudioDevice();
	music = LoadMusicStream("Sounds/korobeiniki.mp3");
	SetMusicVolume(music, 0.2f);
	PlayMusicStream(music);
	rotateSound = LoadSound("Sounds/rotate.mp3");
	SetSoundVolume(rotateSound, 0.2f);
	clearSound = LoadSound("Sounds/clear.mp3");
	SetSoundVolume(clearSound, 0.5f);
	holdSound = LoadSound("Sounds/hold.mp3");
	SetSoundVolume(holdSound, 0.2f);
	hardDropSound = LoadSound("Sounds/hardDrop.mp3");
	SetSoundVolume(hardDropSound, 0.2f);
}

Game::~Game() {
	UnloadMusicStream(music);
	UnloadSound(rotateSound);
	UnloadSound(clearSound);
	UnloadSound(holdSound);
	UnloadSound(hardDropSound);
	CloseAudioDevice();
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
	currentBlock.Draw(10, 10);

	// To account for the move shift when block is first spawned
	switch (nextBlock.id) {
	case 3: //IBLOCK
		nextBlock.Draw(255, 220);
		break;
	case 4://OBLOCK
		nextBlock.Draw(255, 205);
		break;
	default:
		nextBlock.Draw(270, 205);
		//nextBlock.Draw(320 - 90 + 170 / 2 - 90 / 2, 185 + 100 / 2 - 60 / 2); CALCULATION
		break;
	}

	if (held) {
		// These blocks have been reset to 0,0 to reset all player movements & initial spawn move. We will undo-spawn move here.
		switch (heldBlock.id) {
		case 3: //IBLOCK
			heldBlock.Draw(345, 360);
			break;
		case 4://OBLOCK
			heldBlock.Draw(375, 375);
			break;
		default: // ALL OTHER BLOCKS
			heldBlock.Draw(360, 375); //nextBlock.Draw(320 + 170 / 2 - 90 / 2 - 90, 205 + 170); CALCULATION
			break;
		}
	}
}

bool Game::SystemEventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastSystemUpdateTime >= interval) {
		lastSystemUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool Game::UserInputTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUserUpdateTime >= interval) {
		lastUserUpdateTime = currentTime;
		
		return true;
	}
	return false;
}

void Game::UpdateSystemTime() {
	//Resets the interval required for the system to move the block down again, if the user has moved manually!
	lastSystemUpdateTime = lastUserUpdateTime;
}

// Create hard drop and hold mechanics
//Maybe use "IsKeyDown" to implement continuous movement if a user holds down a key
void Game::HandleInput() {
	int keyPressed = GetKeyPressed();

	if (isPaused) {
		if (keyPressed == KEY_P) {
			isPaused = !isPaused;
		}
	}
	else {
		if (gameOver) {
			if (keyPressed && !isAddingHighScore) {
				Reset();
			}
		}
		else {
			//Keys that CAN be updated by holding key down
			if (UserInputTriggered(0.1)) {
				if (IsKeyDown(KEY_DOWN)) {
					MoveBlockDown();
					UpdateSystemTime();
					UpdateScore(0, 1);
				}
				if (IsKeyDown(KEY_RIGHT)) {
					MoveBlockRight();
				}
				if (IsKeyDown(KEY_LEFT)) {
					MoveBlockLeft();
				}
			}
			//Keys that CANNOT be updated by holding key down
			switch (keyPressed) {
			case KEY_UP:
			case KEY_X:
				RotateBlockRight();
				break;
			case KEY_Z:
				RotateBlockLeft();
				break;
			case KEY_SPACE:
				HardDrop();
				break;
			case KEY_C:
				HoldBlock();
				break;
			case KEY_P:
				isPaused = !isPaused;
				break;
			}
		}
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
	canUseHold = true; //Replenish one use of the hold swap
	//set the block on the grid for drawing
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		grid.grid[item.row][item.col] = currentBlock.id;
	}
	//Whenever something new locks, we have to check if a full row is cleared!
	int rowsCleared = grid.ClearFullRows();
	if (rowsCleared > 0) {
		PlaySound(clearSound);
		UpdateScore(rowsCleared, 0);
	}
	//update the currentBlock to the new block as we no longer need the old one!
	currentBlock = nextBlock;
	nextBlock = GetRandomBlock();
	if (!BlockFits()) {
		gameOver = true;
		isAddingHighScore = true;
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
//Returns True for when it locks a block in place
bool Game::MoveBlockDown() {
	if (!gameOver) {
		currentBlock.Move(1, 0);
		if (IsBlockOutside() || !BlockFits()) {
			//Revert movement
			currentBlock.Move(-1, 0);
			//Lock block in place and generates a new block!
			LockBlock();
			return true;  
		}
	}
	return false;
}

void Game::RotateBlockLeft()
{
	if (!gameOver) {
		currentBlock.RotateLeft();
		if (IsBlockOutside() || !BlockFits()) {
			//Only undo movement, we don't lock the block in place here
			currentBlock.RotateRight();
		}
		else {
			//maybe have another sound when you cannot rotate
			PlaySound(rotateSound);
		}
	}
}

void Game::RotateBlockRight()
{
	if (!gameOver) {
		currentBlock.RotateRight();
		PlaySound(rotateSound);
		if (IsBlockOutside() || !BlockFits()) {
			//Only undo movement, we don't lock the block in place here
			currentBlock.RotateLeft();
		}
		else {
			//maybe have another sound when you cannot rotate
			PlaySound(rotateSound);
		}
	}
}

void Game::HoldBlock() {
	//We can only perform holds once before the player has to place the tile
	if (canUseHold) {
		PlaySound(holdSound);
		//------------RESET ALL OFFSETS ON CURRENTBLOCK TO BE HELD-------------------------------//
		currentBlock.Reset();
		//Re-do the initial spawn move
		switch (currentBlock.id) {
		case 3: // IBLOCK
			heldBlock.Move(-1, 3);
			break;
		case 4: //OBLOCK
			heldBlock.Move(0, 4);
			break;
		default: // ALL OTHER BLOCKS
			heldBlock.Move(0, 3);
			break;
		}
		//-------------------------------SWAP LOGIC-------------------------------------------//
		Block tempBlock = currentBlock;
		// When no blocks are held we can just make nextBlock as current, and generate a new nextBlock
		if (!held) {
			currentBlock = nextBlock;
			nextBlock = GetRandomBlock();
		}
		//if a block is already held, swap current and held block
		else {
			currentBlock = heldBlock;
		}
		//Must update the heldblock after the current block
		heldBlock = tempBlock;
		held = true;
		canUseHold = false;
	}
}

void Game::HardDrop() {
	//Keeps moving block down until it locks it in place and returns true
	if (!gameOver) {
		PlaySound(hardDropSound);
		while (!MoveBlockDown()) {
		}
	}
}

void Game::UpdateScore(int rowsCleared, int moveDownPoints)
{
	switch (rowsCleared) {
	case 1:
		score += 100;
		break;
	case 2:
		score += 300;
		break;
	case 3:
		score += 500;
		break;
	default:
		score += rowsCleared * 200;
	}
	score += moveDownPoints;
}

void Game::AddHighScore(std::string name) 
{
	// ADD LOGIC TO RETRIEVE USER's NAME
	// 
	// Maybe reset the game (thus score) after adding the highscore
	// Open a text file for writing, appends to it/does not clear
	std::ofstream outFile("highscore.txt", std::ios::app);

	// Check if the file is successfully opened
	if (outFile.is_open()) {
		// Write content to the file
		outFile << name << " " << score << std::endl;

		// Close the file
		outFile.close();
		std::cout << "File written successfully.\n";
		// Notify the browser about the new file
	}
	else {
		std::cerr << "Error opening the file." << std::endl;
	}
	isAddingHighScore = false;

}

void Game::Reset() {
	gameOver = false;
	held = false;
	canUseHold = true;
	isPaused = false;
	//Reset to starting values
	grid.Initialize();
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	//heldBlock; Don't need to reset
	score = 0;
	isAddingHighScore = false;
}


