#include <iostream>
#include "raylib.h"
#include "BitReader.hpp"
#include "antLogic.hpp"
#include "raymath.h"
#include "BoardPart.hpp"
#include <vector>

void DrawBoardUnaccessiblePart(float offsetX, float offsetY);
void DrawBoardPart(BitReader<uint64_t> &reader, float offsetX, float offsetY);
void DrawCell(float x, float y, Color color, Color borderColor);
void DrawAnt(Ant<uint64_t> &ant);
void antChangeReader(Ant<uint64_t> &a, std::vector<BoardPart<uint64_t>> boardParts);
void createBoardParts(long id, std::vector<BoardPart<uint64_t>>& boardParts);

constexpr size_t ScreenWidth = 800;
constexpr size_t ScreenHeight = 800;

constexpr float cellSize = 20, boardPartSize = 8;
void DrawBoardUnaccessiblePart(float offsetX, float offsetY)
{
    for (int i = 0; i < boardPartSize; ++i)
        for (int j = 0; j < boardPartSize; ++j)
            DrawCell(i * cellSize + offsetX, j * cellSize + offsetY, BLACK, Color{18, 18, 18, 255});
}
void DrawBoardPart(BitReader<uint64_t> &reader, float offsetX, float offsetY)
{
    for (int i = 0; i < boardPartSize; ++i)
        for (int j = 0; j < boardPartSize; ++j)
        {
            Color cellColor;
            cellColor = reader[i * boardPartSize + j] == 0 ? BLACK : WHITE;

            DrawCell(i * cellSize + offsetX, j * cellSize + offsetY, cellColor, Color{60, 0, 60, 255});
        }
}
void DrawCell(float x, float y, Color color, Color borderColor)
{
    DrawRectangle(x, y, cellSize, cellSize, color);
    DrawRectangleLinesEx({x, y, cellSize, cellSize}, 1.f, borderColor);
}
void DrawAnt(Ant<uint64_t> &ant)
{
    // Making the ant body
    Vector2 v1 = {-4, -4};
    Vector2 v2 = {-4, 4};
    Vector2 v3 = {8, 0};

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

void antChangeReader(Ant<uint64_t> &a, std::vector<BoardPart<uint64_t>> boardParts) {
    
}

void createBoardParts(long id, std::vector<BoardPart<uint64_t>>& boardParts) {
    auto it = std::find_if(boardParts.begin(), boardParts.end(), [&](BoardPart<uint64_t> bp) { return bp.getId() == id; });

    if (it != boardParts.end()) {
        for (size_t i = 0; i < 4; ++i)
            if(boardParts[id].getNeighbourId(i) == -1) {
                boardParts[id].setNeighbour(i, boardParts.size());

                size_t x = boardParts[id].getX(), y = boardParts[id].getY();
                switch(i) {
                    case 0: if(y == boardPartSize * cellSize) continue;
                            y -= boardPartSize * cellSize;
                            break;
                    case 1: if(x == boardPartSize * cellSize) continue;
                            x -= boardPartSize * cellSize;
                            break;
                    case 2: if(y >= 3 * boardPartSize * cellSize) continue;
                            y += boardPartSize * cellSize;
                            break;
                    case 3: if(x >= 3 * boardPartSize * cellSize) continue;
                            x += boardPartSize * cellSize;
                            break;
                    default: std::cerr << "WTF"; std::abort();
                }
                std::cout << "CREATED BOARD " << boardParts.size();
                boardParts.push_back({BitReader<uint64_t>(0), boardParts.size(), x, y}); 
            }
    } else {
        std::cout << "The board part doesn't yet exist!";
    }
}

int main()
{
    constexpr size_t boardSideLength = boardPartSize * cellSize;
    constexpr size_t boardPartsHorizontally = ScreenWidth / boardSideLength;
    constexpr size_t boardPartsVertically = ScreenHeight / boardSideLength;

    InitWindow(ScreenWidth, ScreenHeight, "Langhton's Ant");
    SetTargetFPS(5);


    // struct Ant
    // {
    //     size_t x, y;
    //     Facing facing;
    //     BitReader<T> reader[9];
    // };

    uint64_t value64 = 0;
    BitReader<uint64_t> reader(value64);
    std::vector<BoardPart<uint64_t>> boardParts;
    boardParts.reserve(16);
    boardParts.push_back({reader, 0, boardSideLength, boardSideLength});
    Ant<uint64_t> ant{4, 4, Facing::DOWN, 0};

    createBoardParts(0, boardParts);
    createBoardParts(1, boardParts);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        for (size_t i = 0; i < boardPartsHorizontally; ++i)
            DrawBoardUnaccessiblePart(i * boardSideLength, 0);
        for (size_t i = 1; i < boardPartsVertically; ++i)
            DrawBoardUnaccessiblePart(0, i * boardSideLength);

        DrawLine(boardSideLength, boardSideLength, boardSideLength, ScreenHeight, RED);
        DrawLine(boardSideLength, boardSideLength - 1, ScreenWidth, boardSideLength - 1, RED);

        ClearBackground(Color{18, 18, 18, 255});
        moveAnt(ant, reader);

        // for(size_t i = 0; i < boardParts.size(); ++i) {
        //     DrawBoardPart(reader, boardParts[i].getX(), boardParts[i].getY());
        //     std::cout << i % 4 + 1 << ' ' << i / 4 + 1 << '\n';
        // }
        DrawAnt(ant);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}