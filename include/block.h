#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colours.h"


class Block {
public:
	Block();
	std::map<int, std::vector<Position>> cells;
	void Draw();
	void Move(int rows, int cols);
	void RotateRight();
	void RotateLeft();
	std::vector<Position> GetCellPositions();
	int id;
	int gridOffset;
	
private:
	int cellSize;
	int rotationState;
	std::vector<Color> colours;
	int rowOffset;
	int colOffset;
};