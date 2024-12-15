#ifndef EXT4_BYTEARRAY_READER_HPP
#define EXT4_BYTEARRAY_READER_HPP

#include <cstddef>
#include <cinttypes>
#include <fstream>

namespace ext4
{

class bytearray_reader
{
public:
    bytearray_reader(uint8_t const * data, size_t size);
    uint32_t u32(size_t offset) const;
    uint16_t u16(size_t offset) const;

    static bytearray_reader from_stream(std::ifstream & stream, uint8_t * data, size_t offset, size_t size);

private:
    uint8_t const * m_data;
    size_t const m_size;
};

}

#endif
