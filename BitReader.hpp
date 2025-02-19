#include <string>
#include <iostream>
#include <fstream>

class BitReader
{
public:
    BitReader(const std::string &filename)
        : file(filename, std::ios::binary), currentByte(0), bitsRemaining(0), mask(128)
    {
        if (!file)
        {
            throw std::runtime_error("Unable to open file");
        }
    }

    void readByte()
    {
        file.read(reinterpret_cast<char *>(&currentByte), 1);

        if (file.gcount() != 1)
        {

            throw std::runtime_error("Error reading byte\n");
        }
        this->bitsRemaining = 8;
        this->mask = 128;
    }

    bool readBit()
    {
        if (!this->bitsRemaining)
        {
            this->readByte();
        }

        bool val = (currentByte & mask) >> (bitsRemaining - 1);
        --bitsRemaining;
        mask = mask / 2;

        return val;
    }

    unsigned char readBits(unsigned int bits)
    {
        if (bits < 1 || bits > 8)
        {
            std::cout << "Warning: incorrect value of bits, truncated to 1\n";
            bits = 1;
        };

        unsigned char number = 0;

        for (size_t i = 0; i < bits; ++i)
        {
            bool rezult = readBit();
            number |= (rezult << (bits - i - 1));
        }

        return number;
    }

private:
    std::ifstream file;
    unsigned char currentByte;
    int bitsRemaining;
    size_t mask;
};
