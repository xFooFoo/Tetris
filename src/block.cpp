#include "..\include\block.h"

Block::Block() {
	cellSize = 30;
	rotationState = 0;
	colours = GetCellColours();
	rowOffset = 0;
	colOffset = 0;
	gridOffset = 1;
}

void Block::Draw(int offsetX, int offsetY) {
	//Original State
	std::vector<Position> tiles = GetCellPositions();
	for (Position item : tiles) {
		//When blocks are initialized, they call Move to the centre of the grid so it has an offset in the X direction!
		DrawRectangle(item.col * cellSize + gridOffset + offsetX, item.row * cellSize + gridOffset + offsetY, cellSize - gridOffset, cellSize - gridOffset, colours[id]);
	}
}

void Block::Move(int rows, int cols) {
	rowOffset += rows;
	colOffset += cols;
}

void Block::RotateRight()
{
	rotationState++;
	if (rotationState == cells.size()) {
		rotationState = 0;
	}
}

void Block::RotateLeft()
{
	rotationState--;
	if (rotationState < 0) {
		rotationState = cells.size() - 1;
	}
}

void Block::Reset() {
	rotationState = 0;
	rowOffset = 0;
	colOffset = 0;
}

std::vector<Position> Block::GetCellPositions(){
	//Original State
	std::vector<Position> tiles = cells[rotationState];
	//Overall movement/offset to the tiles
	std::vector<Position> movedTiles;
	for (Position item : tiles) {
		Position newPos = Position(item.row + rowOffset, item.col + colOffset);
		movedTiles.push_back(newPos);
	}
	return movedTiles;
}