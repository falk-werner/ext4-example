#include "ext4/inode.hpp"
#include "ext4/bytearray_reader.hpp"

#include <algorithm>

namespace
{

constexpr size_t const ino_max_size = 256;

constexpr size_t const ino_mode_offset = 0;
constexpr size_t const ino_uid_offset = 2;
constexpr size_t const ino_size_lo_offset = 4;
constexpr size_t const ino_gid_offset = 24;
constexpr size_t const ino_flags_offset = 0x20;

}

namespace ext4
{

void inode::read(std::ifstream & stream, uint64_t offset, uint16_t size)
{
    uint8_t buffer[ino_max_size];
    size_t const ino_size = std::min(static_cast<size_t>(size), ino_max_size);
    auto reader = bytearray_reader::from_stream(stream, buffer, offset, ino_size);

    mode = reader.u16(ino_mode_offset);
    uid = reader.u16(ino_uid_offset);
    gid = reader.u16(ino_gid_offset);
    size = reader.u32(ino_size_lo_offset);
    flags = reader.u32(ino_flags_offset);
}

}