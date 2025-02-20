#include <iostream>
#include "raylib.h"
#include "BitReader.hpp"
#include "antLogic.hpp"

void DrawBoardPart();
void DrawCell(float x, float y, Color color);


constexpr float cellSize = 20, boardSize = 8;
void DrawBoardPart(BitReader<uint64_t> &reader, float offsetX, float offsetY) {
    for(int i = 0; i < boardSize; ++i)
        for(int j = 0; j < boardSize; ++j) {
            Color cellColor;
            cellColor = reader[i * boardSize + j] == 0 ? BLACK : WHITE;

            DrawCell(i * cellSize + offsetX, j * cellSize + offsetY, cellColor);
        }
}
void DrawCell(float x, float y, Color color) {
    DrawRectangle(x, y, cellSize, cellSize, color);
    DrawRectangleLinesEx({x, y, cellSize, cellSize}, 1.f, Color{60, 0, 60, 255});
}

int main()
{
    InitWindow(800, 600, "Langhton's Ant");
    SetTargetFPS(1);

    uint64_t value32 = 0;
    
    BitReader reader(value32);
    Ant ant{4, 4, Facing::UP};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{18, 18, 18, 255});
        moveAnt(ant, reader);
        DrawBoardPart(reader, 300, 200);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}