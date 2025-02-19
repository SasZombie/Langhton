#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
class BitReader
{
public:
    BitReader() = default;
    BitReader(T n_number)
        : number(n_number)
    {
        length = sizeof(T) * 8;
        index = (sizeof(T) * 8) - 1;
    }

    void resetIndex()
    {
        index = length - 1;
    }

    //READS MSB
    bool readValueMSB()
    {
        if(index == -1)
        {
            throw std::out_of_range("Read value above lenght. Your number is: " + std::to_string(length) + " bits and you tried to read another one\n");
        }
        bool value = (number & (1ULL << index)); 
        
        --index;

        return value != 0;
    }

    class BitIterator
    {
    public: 
        BitIterator(T n_number, size_t n_index)
            : number(n_number), index(n_index){}

        bool operator*() const
        {
            return (number & (1ULL << index)) != 0;
        }

        BitIterator& operator++()
        {
            --index;
            return *this;
        }

        bool operator!=(const BitIterator& other) const 
        {
            return index != other.index;
        }

    private:
        T number;
        size_t index;
    };

    BitIterator begin() const 
    {
        return BitIterator(number, length - 1);
    }

    BitIterator end() const
    {
        return BitIterator(number, static_cast<size_t>(-1));
    }

    void setBit(const size_t bitIndex, bool value) 
    {
        if(bitIndex > length)
        {
            throw std::out_of_range("Read value above lenght. Your number is: " + std::to_string(length) + " bits and you tried to read above\n");
        }

        if(value)
        {
            number |= (1ULL << bitIndex);   
        }else
        {
            number &= ~(1ULL << bitIndex);
        }
    }

    struct BitReference
    {
        T &number;
        size_t pozition;

        void operator=(bool value)
        {
            if(value)
            {
                number |= (1ULL << pozition);
            }
            else
            {
                number &= ~(1ULL << pozition);
            }
        }

        operator bool() const
        {
            return (number >> pozition) & 1;
        }
    };
    
    

    BitReference operator[](size_t bitIndex)
    {
        if(bitIndex > length)
        {
            throw std::out_of_range("Read value above lenght. Your number is: " + std::to_string(length) + " bits and you tried to read above\n");
        }

        return BitReference{number, bitIndex};
    }


private:
    T number;
    size_t length;
    int index = 0;
};