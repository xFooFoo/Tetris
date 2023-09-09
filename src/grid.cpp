#include <stdio.h>
#include "..\include\grid.h"
#include <iostream>
#include <raylib.h>
#include "..\include\colours.h"

Grid::Grid() {
    cellWidth = 10;
    cellLength = 20;
    cellSize = 30;
    borderSize = 1;
    Initialize();
    colours = GetCellColours();
}

void Grid::Initialize() {
    for (int row = 0; row < cellLength; row++) {
        for (int col = 0; col < cellWidth; col++) {
            grid[row][col] = 0;
        }
    }
}
void Grid::Print() {
    for (int row = 0; row < cellLength; row++) {
        for (int col = 0; col < cellWidth; col++) {
            std::cout << grid[row][col] << " ";
            
        }
        std::cout << std::endl;
    }
}
void Grid::Draw() {
    for (int row = 0; row < cellLength; row++) {
        for (int col = 0; col < cellWidth; col++) {
            int cellValue = grid[row][col];
            // borderSize adjustments such that the background drawn will appear as the grids!
            // +10 is to show the frame on the top & left
            DrawRectangle(col * cellSize + borderSize + 10, row * cellSize + borderSize + 10, cellSize - borderSize, cellSize - borderSize, colours[cellValue]);
            //The background can be the grid...
            //DrawRectangleLinesEx(Rectangle{ (float)col * cellSize + 1, (float)row * cellSize + 1, (float)cellSize , (float)cellSize }, (float)1, DARKGRAY);
        }
    }
}

bool Grid::IsCellOutside(int row, int col)
{
    if (row >= 0 && row < cellLength && col >= 0 && col < cellWidth) {
        return false;
    }
    return true;
}

bool Grid::IsCellEmpty(int row, int col)
{
    if (grid[row][col] == 0) {
        return true;
    }
    return false;
}

int Grid::ClearFullRows()
{
    int completed = 0;
    //Checks each row, from bottom-up (BU), Very important to use a BU approach
    // to ensure we shift down the appropriate rows correctly
    for (int row = cellLength - 1; row >= 0; row--) {
        if (IsRowFull(row)) {
            ClearRow(row);
            completed++;
        }
        //Move rows down if previous rows were cleared
        else if (completed > 0) {
            MoveRowDown(row, completed);
        }
    }
    return completed;
}

bool Grid::IsRowFull(int row)
{
    for (int col = 0; col < cellWidth; col++) {
        if (IsCellEmpty(row, col)) {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row)
{
    for (int col = 0; col < cellWidth; col++) {
        grid[row][col] == 0;
    }
}

void Grid::MoveRowDown(int row, int numRows)
{
    for (int col = 0; col < cellWidth; col++) {
        grid[row + numRows][col] = grid[row][col];
        grid[row][col] = 0;
    }
}

