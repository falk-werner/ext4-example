#include "crc32.hpp"

namespace ext4
{

namespace
{

constexpr const uint32_t crc32_polynom = 0x04C11DB7;

}

crc32::crc32()
: checksum(0)
{
}

void crc32::update(uint8_t value)
{
    for(size_t i = 0; i < 8; i++)
    {
        uint32_t const value_bit = (value >> i) & 1;
        uint8_t const checksum_msb = (checksum >> 31) & 1;
        if (((checksum >> 31) & 1) == 1)
        {
            checksum = ((checksum << 1) + value_bit) ^ crc32_polynom;
        }
        else
        {
            checksum = ((checksum << 1) + value_bit);
        }
    }
}

void crc32::update(uint8_t * value, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        update(value[i]);
    }
}

uint32_t crc32::get_checksum() const
{
    return checksum;
}


}