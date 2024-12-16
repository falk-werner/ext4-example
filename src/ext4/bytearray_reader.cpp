#include "ext4/bytearray_reader.hpp"
#include <stdexcept>

namespace ext4
{

bytearray_reader::bytearray_reader(uint8_t const * data, size_t size)
: m_data(data)
, m_size(size)
{

}

bytearray_reader bytearray_reader::from_stream(std::ifstream & stream, uint8_t * data, size_t offset, size_t size)
{
    stream.seekg(offset);
    if (!stream.good())
    {
        throw std::runtime_error("failed to seek");
    }

    stream.read(reinterpret_cast<char*>(data), size);
    // Note: we presume, we are not at the end of the file
    // which should be true for all our use cases
    if (!stream.good())
    {
        throw std::runtime_error("failed to read");
    }

    return bytearray_reader(data, size);
}


uint32_t bytearray_reader::u32(size_t offset) const
{
    if ((offset + 3) > m_size)
    {
        throw std::range_error("index out of bounds");
    }

    uint32_t value = 0;
    for(size_t i = 0; i < 4; i++)
    {
        value <<= 8;
        value |= m_data[offset + 3 - i];
    }
    return value;
}

uint16_t bytearray_reader::u16(size_t offset) const
{
    if ((offset + 1) > m_size)
    {
        throw std::range_error("index out of bounds");
    }

    uint16_t const low  = static_cast<uint16_t>(m_data[offset]);
    uint16_t const high = static_cast<uint16_t>(m_data[offset + 1]);

    return (high << 8) | low;
}

std::string bytearray_reader::str(size_t offset, size_t length) const
{
    if ((offset + length) >= m_size)
    {
        throw std::range_error("index out of bounds");
    }

    return std::string(reinterpret_cast<char const*>(&m_data[offset]), length);
}


}