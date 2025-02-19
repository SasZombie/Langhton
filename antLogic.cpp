#include <iostream>
#include "BitReader.hpp"

uint64_t board;

int main()
{
    uint64_t table1 = 0;
    uint64_t table2 = 0;

    uint64_t value32 = 0b1111111111111111111110000000000;
    // 111111111111111111111000010000000
    // 1111111111111111111110000000000
    //   1111111111111111111110000000000
    //   10101010101010101010101010101010;
    //   01010101010101010101010101010101 // Example value for uint32_t

    BitReader reader(value32);

    for (size_t i = 0; i < 32; ++i)
    {
        std::cout << reader.readValueMSB();
    }

    std::cout << '\n';

    for (const auto elem : reader)
    {
        std::cout << elem;
    }

    std::cout << '\n';

    reader.setBit(1, 1);

    for (const auto elem : reader)
    {
        std::cout << elem;
    }
    std::cout << '\n';
    std::cout << reader[2];

    reader[2] = 1;

    std::cout << reader[2];

    std::cout << '\n';
}
