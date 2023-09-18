// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there
#include <raylib.h>
#include "..\include\game.h"
#include <iostream>
#include <cmath>

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
    InitWindow(500, 620, "Tetris");
    SetTargetFPS(60);
    
    Game game = Game();
    Font font = LoadFontEx("Font/BarcadeNoBar.otf", 64, 0, 0);

    //std::cout << GetWorkingDirectory();

    while (!WindowShouldClose()) {
        UpdateMusicStream(game.music);
        BeginDrawing();
        //--------------Interval period in seconds is passed to EventTriggered-----------//
        if (EventTriggered(0.3 - std::min(((double) game.score / 100000.0), 0.25))) {
            game.MoveBlockDown();
            //std::cout << 0.3 - std::min(((double) game.score / 100000.0), 0.25) << std::endl;
        }
        //-------------------KEY PRESS PROCESSES HERE-------------------------------//
        game.HandleInput();
        //-------------------------------------------------------------------------//
        ClearBackground(purpleBackground);
        //------------------------UI STARTS---------------------------------------//
        DrawTextEx(font, "Score", { 320 + 20, 15 }, 38, 2, RAYWHITE);
        DrawRectangleRounded({320, 55, 170, 60}, (float)0.3, 6, lightPurpleBackground);
        //-------------- Score text generation ----------------------//
        char scoreText[10];
        sprintf_s(scoreText, sizeof(scoreText), "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font, scoreText, { 320 + (170 - textSize.x) / 2, 66.5}, 38, 2, RAYWHITE);
        //---------------------------------------------------------------//
        DrawTextEx(font, "Next", { 320 + 32.5, 145 }, 38, 2, RAYWHITE);
        DrawRectangleRounded({ 310 + 10, 185, 170, 100 }, (float)0.3, 6, lightPurpleBackground);
        //---------------------------------------------------------------//
        DrawTextEx(font, "Hold", { 320 + 32.5, 315 }, 38, 2, RAYWHITE);
        DrawRectangleRounded({ 310 + 10, 355, 170, 100 }, (float)0.3, 6, lightPurpleBackground);
        if (game.gameOver) {
            DrawTextEx(font, "GAME", { 320 + 25, 500 }, 38, 2, RED);
            DrawTextEx(font, "OVER", { 320 + 30, 540 }, 38, 2, RED);
        }
        //----------------------------UI END------------------------------------//
        game.Draw();
        EndDrawing();

    }
    CloseWindow();
}