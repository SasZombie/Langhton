#include <iostream>
#include "raylib.h"
#include "BitReader.hpp"
#include "antLogic.hpp"
#include "raymath.h"

void DrawBoardPart();
void DrawBoardUnaccessiblePart();
void DrawCell(float x, float y, Color color, Color borderColor);
void DrawAnt(Ant& ant);

const size_t ScreenWidth = 800;
const size_t ScreenHeight = 800;

float 
    cellSize = 20,
    boardPartSize = 8;
void DrawBoardUnaccessiblePart(float offsetX, float offsetY) {
    for(int i = 0; i < boardPartSize; ++i)
        for(int j = 0; j < boardPartSize; ++j)
            DrawCell(i * cellSize + offsetX, j * cellSize + offsetY, BLACK, Color{18, 18, 18, 255});
}
void DrawBoardPart(BitReader<uint64_t> &reader, float offsetX, float offsetY) {
    for(int i = 0; i < boardPartSize; ++i)
        for(int j = 0; j < boardPartSize; ++j) {
            Color cellColor;
            cellColor = reader[i * boardPartSize + j] == 0 ? BLACK : WHITE;

            DrawCell(i * cellSize + offsetX, j * cellSize + offsetY, cellColor, Color{60, 0, 60, 255});
        }
}
void DrawCell(float x, float y, Color color, Color borderColor) {
    DrawRectangle(x, y, cellSize, cellSize, color);
    DrawRectangleLinesEx({x, y, cellSize, cellSize}, 1.f, borderColor);
}
void DrawAnt(Ant& ant) {
    // Making the ant body
    Vector2 v1 = {-4, -4};
    Vector2 v2 = {-4,  4};
    Vector2 v3 = { 8,  0};

    // Rotating the ant body
    v1 = Vector2Rotate(v1, static_cast<size_t>(ant.facing) * 90 * DEG2RAD);
    v2 = Vector2Rotate(v2, static_cast<size_t>(ant.facing) * 90 * DEG2RAD);
    v3 = Vector2Rotate(v3, static_cast<size_t>(ant.facing) * 90 * DEG2RAD);

    // Positioning the ant on the right board cell
    v1 = Vector2Add(v1, {(ant.x + 0.5f + boardPartSize) * cellSize, (ant.y + 0.5f + boardPartSize) * cellSize});
    v2 = Vector2Add(v2, {(ant.x + 0.5f + boardPartSize) * cellSize, (ant.y + 0.5f + boardPartSize) * cellSize});
    v3 = Vector2Add(v3, {(ant.x + 0.5f + boardPartSize) * cellSize, (ant.y + 0.5f + boardPartSize) * cellSize});

    // Drawing the ant
    DrawTriangle(v1, v2, v3, RED);
    DrawCircle(v3.x, v3.y, 2.0, RED);
}

int main()
{
    const size_t 
        boardSideLength = boardPartSize * cellSize,
        boardPartsHorizontally = ScreenWidth / boardSideLength,
        boardPartsVertically = ScreenHeight / boardSideLength;


    InitWindow(ScreenWidth, ScreenHeight, "Langhton's Ant");
    SetTargetFPS(5);

    uint64_t value32 = 0;
    BitReader reader(value32);
    Ant ant{4, 4, Facing::DOWN};

    while(!WindowShouldClose()) {
        BeginDrawing();
        
        for (size_t i = 0; i < boardPartsHorizontally; ++i)
            DrawBoardUnaccessiblePart(i * boardSideLength, 0);
        for (size_t i = 1; i < boardPartsVertically; ++i)
            DrawBoardUnaccessiblePart(0, i * boardSideLength);

        
        DrawLine(boardSideLength, boardSideLength, boardSideLength, ScreenHeight, RED);
        DrawLine(boardSideLength, boardSideLength - 1, ScreenWidth, boardSideLength - 1, RED);

        ClearBackground(Color{18, 18, 18, 255});
        moveAnt(ant, reader);
        DrawBoardPart(reader, boardSideLength, boardSideLength);
        DrawAnt(ant);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}