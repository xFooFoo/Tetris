#pragma once
#include <vector>
#include <raylib.h>

class Grid {
public:
    Grid();
    int grid[20][10];
    void Initialize();
    void Print();
    void Draw();
    bool IsCellOutside(int row, int col);//this checks if blocktiles are in frame
    bool IsCellEmpty(int row, int col);//this checks if blocktiles are preoccupied
    int ClearFullRows();

private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    int cellWidth;
    int cellLength;
    int cellSize;
    int borderSize;
    std::vector<Color> colours;
};
