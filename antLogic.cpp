#include <iostream>
#include "BitReader.hpp"

enum struct Facing{
    UP, 
    DOWN,
    RIGHT,
    LEFT,
};

struct Ant
{
    size_t x, y;
    Facing facing;
};

std::pair<size_t, size_t> move(const Ant& a)
{
    switch (a.facing)
    {
    case Facing::UP:
        return std::make_pair(a.x - 1, a.y);
    
    case Facing::DOWN:
        return std::make_pair(a.x + 1, a.y);

    case Facing::LEFT:
        return std::make_pair(a.x, a.y - 1);

    case Facing::RIGHT:
        return std::make_pair(a.x, a.y + 1);

    default:
        std::cerr << "UNREACHABLE with value: " << static_cast<size_t>(a.facing) << '\n';
        exit(EXIT_FAILURE);
        break;
    }
}

void moveAnt(Ant& ant, BitReader<uint64_t>& bit)
{
    size_t index = ant.x * 8 + ant.y;

    if(bit[index] == 0)
    {
        ant.facing = static_cast<Facing>((static_cast<size_t>(ant.facing) - 1) % 4);
    }
    else
    {
        ant.facing = static_cast<Facing>((static_cast<size_t>(ant.facing) + 1) % 4);
    }

    const auto[x, y] = move(ant);

    ant.x = x;
    ant.y = y;

    bit[index] = !bit[index];
}

int main()
{
    uint64_t value32 = 0;
    
    BitReader reader(value32);
    Ant ant{4, 4, Facing::UP};

    for(const auto elem : reader)
    {
        std::cout << elem;
    }

    moveAnt(ant, reader);

    std::cout << '\n';
    for(const auto elem : reader)
    {
        std::cout << elem;
    }
}
