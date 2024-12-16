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
constexpr size_t const ino_direct_blockpointers_offset = 40;
constexpr size_t const ino_singly_indirect_blockpointers_offset = 88;
constexpr size_t const ino_doubly_indirect_blockpointers_offset = 92;
constexpr size_t const ino_triply_indirect_blockpointers_offset = 96;

constexpr uint32_t const ion_flag_extents = 0x80000;

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

    if ((flags & ion_flag_extents) == 0)
    {
        for(size_t i = 0; i < ino_direct_blockpointers_size; i++)
        {
            direct_blockpointers[i] = reader.u32(ino_direct_blockpointers_offset + (i * 4));
        }
        singly_indirect_blockpointers = reader.u32(ino_singly_indirect_blockpointers_offset);
        doubly_indirect_blockpointers = reader.u32(ino_doubly_indirect_blockpointers_offset);
        triply_indirect_blockpointers = reader.u32(ino_triply_indirect_blockpointers_offset);
    }
    else
    {
        for(size_t i = 0; i < ino_direct_blockpointers_size; i++)
        {
            direct_blockpointers[i] = 0;
        }
        singly_indirect_blockpointers = 0;
        doubly_indirect_blockpointers = 0;
        triply_indirect_blockpointers = 0;
    }
}

}