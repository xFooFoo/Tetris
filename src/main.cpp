// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <raylib.h>
#include "..\include\game.h"




int main()
{
    InitWindow(500, 620, "Tetris");
    //SetTargetFPS(60); //we already have delta time to account for the periodic game.MoveBlockDown()
    Game game = Game();
    Font font = LoadFontEx("Font/BarcadeNoBar.otf", 64, 0, 0);
    char scoreText[10];
    //std::cout << GetWorkingDirectory();



    while (!WindowShouldClose()) {
        UpdateMusicStream(game.music);
        BeginDrawing();
        //--------------Interval period in seconds is passed to EventTriggered-----------//
        if (!game.isPaused) {
            if (game.SystemEventTriggered(0.5 - std::min(((double)game.score / 50000.0), 0.30))) {
                game.MoveBlockDown();
                //std::cout << 0.5 - std::min(((double)game.score / 100000.0), 0.20) << std::endl;
            }
        }
        //-------------------KEY PRESS PROCESSES HERE-------------------------------//
        game.HandleInput();
        //-------------------------------------------------------------------------//
        ClearBackground(purpleBackground);
        //------------------------UI STARTS---------------------------------------//
        DrawRectangleLinesEx({ 0, 0, 500, 620 }, 2, RAYWHITE);
        DrawTextEx(font, "Score", { 320 + 20, 15 }, 38, 2, RAYWHITE);
        DrawRectangleRounded({320, 55, 170, 60}, (float)0.3, 6, lightPurpleBackground);
        //-------------- Score text generation ----------------------//
        std::snprintf(scoreText, sizeof(scoreText), "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font, scoreText, { 320 + (170 - textSize.x) / 2, 66.5}, 38, 2, RAYWHITE);
        //---------------------------------------------------------------//
        DrawTextEx(font, "Next", { 320 + 32.5, 145 }, 38, 2, RAYWHITE);
        DrawRectangleRounded({ 310 + 10, 185, 170, 100 }, (float)0.3, 6, lightPurpleBackground);
        //---------------------------------------------------------------//
        DrawTextEx(font, "Hold", { 320 + 32.5, 315 }, 38, 2, RAYWHITE);
        DrawRectangleRounded({ 310 + 10, 355, 170, 100 }, (float)0.3, 6, lightPurpleBackground);
        //----------------------------UI END------------------------------------//
        //Draw Blocks & Grids
        game.Draw();
        //---------------------------- SYSTEM MESSAGE ------------------------------------//
        //Vector2 playTextSize = MeasureTextEx(font, "P TO PLAY", 38, 2);//playTextSize X = 215.5 Y = 38
        //std::cout << "x: " << playTextSize.x << " y: " << playTextSize.y << std::endl;
        if (game.gameOver) {
            DrawTextEx(font, "GAME", { 320 + 25, 500 }, 38, 2, RED);
            DrawTextEx(font, "OVER", { 320 + 30, 540 }, 38, 2, RED);
            //playTextSize X = 215.5 Y = 38
            DrawRectangleRounded({ (300 - 215.5) / 2, (605 - 38) / 2, 215.5 + 10, 38 + 5}, (float)0.3, 6, lightPurpleBackground);
            DrawTextEx(font, "P TO PLAY", { (310 - 215.5) / 2, (610 - 38) / 2 }, 38, 2, RAYWHITE);
        }
        else if (game.isPaused) {
            DrawTextEx(font, "PAUSED", { 300 + 25, 510 }, 38, 2, RED);
            DrawRectangleRounded({ (300 - 215.5) / 2, (605 - 38) / 2, 215.5 + 10, 38 + 5 }, (float)0.3, 6, lightPurpleBackground);
            DrawTextEx(font, "P TO PLAY", { (310 - 215.5) / 2, (610 - 38) / 2 }, 38, 2, RAYWHITE);
        }
        EndDrawing();
    }
    CloseWindow();
}