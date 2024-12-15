#include "ext4/blockgroup_descriptor.hpp"
#include "ext4/bytearray_reader.hpp"

namespace
{

size_t min(size_t a, size_t b)
{
    return (a < b) ? a : b;
}

constexpr size_t const bg_block_bitmap_offset = 0;
constexpr size_t const bg_inode_bitmap_offset = 4;
constexpr size_t const bg_inode_table_offset  = 8;

constexpr size_t const bg_descriptor_size = 64;

}

namespace ext4
{

void blockgroup_descriptor::read(std::ifstream & stream, uint64_t offset, uint16_t size)
{
    uint8_t buffer[bg_descriptor_size];
    auto reader = bytearray_reader::from_stream(stream, buffer, offset, min(bg_descriptor_size, size));

    block_bitmap = reader.u32(bg_block_bitmap_offset);
    inode_bitmap = reader.u32(bg_inode_bitmap_offset);
    inode_table  = reader.u32(bg_inode_table_offset);
}

}