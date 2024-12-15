#ifndef EXT4_CRC32_HPP
#define EXT4_CRC32_HPP

#include <cinttypes>
#include <cstddef>

namespace ext4
{

class crc32
{
public:
    crc32();
    void update(uint8_t value);
    void update(uint8_t * value, size_t size);
    uint32_t get_checksum() const;
private:
    uint32_t checksum;
};

}

#endif
