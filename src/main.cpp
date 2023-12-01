// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <raylib.h>
#include "..\include\game.h"

#define MAX_INPUT_CHARS 16


int main()
{
    InitWindow(500, 620, "Tetris");
    //SetTargetFPS(60); //we already have delta time to account for the periodic game.MoveBlockDown()
    Game game = Game();
    Font font = LoadFontEx("Font/BarcadeNoBar.otf", 64, 0, 0);
    char scoreText[10];
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int nameLetterCount = 0;
    int framesCounter = 0;
    Rectangle nameTextBox = { (500 - 400) / 2.0f, 180, 400, 50 };
    bool mouseOnText = false;
    //std::cout << GetWorkingDirectory();



    while (!WindowShouldClose()) {
        UpdateMusicStream(game.music);
        BeginDrawing();

        if (!game.isAddingHighScore)
        {
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
            DrawRectangleRounded({ 320, 55, 170, 60 }, (float)0.3, 6, lightPurpleBackground);
            //-------------- Score text generation ----------------------//
            std::snprintf(scoreText, sizeof(scoreText), "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, { 320 + (170 - textSize.x) / 2, 66.5 }, 38, 2, RAYWHITE);
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
        }

        //------------------------------ADD  HIGHSCORE PROCESS------------------------------------//
        if (CheckCollisionPointRec(GetMousePosition(), nameTextBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..126]
                if ((key >= 32) && (key <= 126) && (nameLetterCount < MAX_INPUT_CHARS))
                {
                    name[nameLetterCount] = (char)key;
                    name[nameLetterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    nameLetterCount++;
                }
                key = GetCharPressed();  // Check next character in the queue
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                
                game.AddHighScore(name);
                // Reset 
                std::memset(name, '\0', sizeof(name));
                nameLetterCount = 0;
                framesCounter = 0;
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                nameLetterCount--;
                if (nameLetterCount < 0) nameLetterCount = 0;
                name[nameLetterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;

        if (game.gameOver && game.isAddingHighScore) {
            //------------ADD HIGHSCORE MENU ----------------------------------------------------//
            ClearBackground(purpleBackground);
            DrawText("PLEASE ENTER YOUR NAME!", 30, 80, 30, RAYWHITE);
            DrawText("PLACE MOUSE OVER INPUT BOX!", 80, 140, 20, RAYWHITE);

            DrawRectangleRec(nameTextBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)nameTextBox.x, (int)nameTextBox.y, (int)nameTextBox.width, (int)nameTextBox.height, RAYWHITE);
            else DrawRectangleLines((int)nameTextBox.x, (int)nameTextBox.y, (int)nameTextBox.width, (int)nameTextBox.height, DARKGRAY);

            DrawText(name, (int)nameTextBox.x + 5, (int)nameTextBox.y + 8, 40, lightPurpleBackground);

            DrawText(TextFormat("INPUT CHARS: %i/%i", nameLetterCount, MAX_INPUT_CHARS), (int)nameTextBox.x, 250, 20, GRAY);

            DrawText(TextFormat("Score: %i", game.score), (int)nameTextBox.x, 400, 40, RAYWHITE);

            if (mouseOnText)
            {
                if (nameLetterCount < MAX_INPUT_CHARS)
                {
                    // Draw blinking underscore char
                    if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)nameTextBox.x + 8 + MeasureText(name, 40), (int)nameTextBox.y + 12, 40, lightPurpleBackground);
                }
                else DrawText("Press BACKSPACE to delete chars...", (int)nameTextBox.x, 300, 20, GRAY);
            }
        }
        else if (game.gameOver) {
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