#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <array>


template <Numeric T>
class BoardPart
{
public:
    BoardPart() = default;
    BoardPart(BitReader<T> n_bitReader, long n_id, size_t n_x, size_t n_y)
        : bitReader(n_bitReader)
    {
        id = n_id;
        x = n_x;
        y = n_y;
    }

    void setNeighbour(size_t position, long n_id)
    {
        if(position > neighbours.max_size())
            throw std::out_of_range("Insert position above lenght. Your length is: " + std::to_string(position) + " and the neighbours array has only 4 positions\n");

        neighbours[position] = n_id;
    }

    long getNeighbourId(size_t position)
    {
        if(position > neighbours.max_size())
            throw std::out_of_range("Read position above lenght. Your length is: " + std::to_string(position) + " and the neighbours array has only 4 positions\n");
            
        return neighbours[position];
    }

    long getId() {
        return id;
    }
    size_t getX() {
        return x;
    }
    size_t getY() {
        return y;
    }

private:
    BitReader<uint64_t> bitReader;
    std::array<long, 4> neighbours = {-1, -1, -1, -1}; // WASD
    long id;
    size_t x, y;
};