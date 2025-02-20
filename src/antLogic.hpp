#include <iostream>
#include "BitReader.hpp"

enum struct Facing{
    UP, 
    RIGHT,
    DOWN,
    LEFT,
};

template <Numeric T>
struct Ant
{
    size_t x, y;
    Facing facing;
    std::array<BitReader<T>, 9> readers;
};

template <Numeric T>
std::pair<size_t, size_t> move(const Ant<T>& a)
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

template<Numeric T>
void moveAnt(Ant<T>& ant, BitReader<T>& bit)
{
    size_t index = ant.x * sizeof(T) + ant.y;

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
