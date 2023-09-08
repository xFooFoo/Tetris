// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there
#include <raylib.h>
#include "..\include\game.h"

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    Color darkGrayGrid = { 86, 86, 96, 255 };

    InitWindow(300, 600, "Christine");
    SetTargetFPS(60);
    
    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();
        //Interval period in seconds is passed to EventTriggered
        if (EventTriggered(0.5)) {
            game.MoveBlockDown();
        }
        game.HandleInput();
        //game.Update();
        ClearBackground(darkGrayGrid);
        game.Draw();
        EndDrawing();

    }
    CloseWindow();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
