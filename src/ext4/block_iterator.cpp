#include "ext4/block_iterator.hpp"
#include "ext4/bytearray_reader.hpp"

#include <iostream>

namespace
{

constexpr size_t const ino_direct_blockpointers_offset = 0;
constexpr size_t const ino_singly_indirect_blockpointers_offset = 48;
constexpr size_t const ino_doubly_indirect_blockpointers_offset = 52;
constexpr size_t const ino_triply_indirect_blockpointers_offset = 56;

}

namespace ext4
{

block_iterator::block_iterator(core_i & core)
: m_core(core)
{

}

void block_iterator::foreach_block(inode const & inode, block_visitor visitor)
{
    if ((inode.flags & ino_flag_extents) != 0)
    {
        return;
    }

    inode_blockpointers blockpointers;
    bytearray_reader reader(reinterpret_cast<const uint8_t*>(inode.block.data()), inode.block.size());
    for(size_t i = 0; i < ino_direct_blockpointers_size; i++)
    {
        blockpointers.direct[i] = reader.u32(ino_direct_blockpointers_offset + (i * 4));
    }
    blockpointers.singly_indirect = reader.u32(ino_singly_indirect_blockpointers_offset);
    blockpointers.doubly_indirect = reader.u32(ino_doubly_indirect_blockpointers_offset);
    blockpointers.triply_indirect = reader.u32(ino_triply_indirect_blockpointers_offset);


    foreach_direct_block(blockpointers, visitor) &&
    foreach_indirect_block(blockpointers.singly_indirect, visitor) &&
    foreach_doubly_indirect_block(blockpointers.doubly_indirect, visitor) &&
    foreach_triply_indirect_block(blockpointers.triply_indirect, visitor);

}

bool block_iterator::visit_block(uint64_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    auto block = m_core.read_block(block_id);
    if (!block.has_value())
    {
        std::cerr << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    try
    {            
        return visitor(block.value().data(), block.value().size());
    }
    catch (...)
    {
        std::cerr << "error: core: block visitor throws" << std::endl;
        return false;
    }
}

bool block_iterator::foreach_direct_block(inode_blockpointers const & blockpointers, block_visitor & visitor)
{
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < ino_direct_blockpointers_size); i++)
    {
        uint64_t const block_id = static_cast<uint64_t>(blockpointers.direct[i]);
        continue_ = visit_block(block_id, visitor);
    }

    return continue_;
}

bool block_iterator::foreach_indirect_block(uint32_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    auto block = m_core.read_block(static_cast<uint64_t>(block_id));
    if (!block.has_value())
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = block.value().size() / address_size;
    bytearray_reader reader(block.value().data(), block.value().size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = visit_block(address, visitor);
    }

    return continue_;
}

bool block_iterator::foreach_doubly_indirect_block(uint32_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    auto block = m_core.read_block(static_cast<uint64_t>(block_id)); 
    if (!block.has_value())
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = block.value().size() / address_size;
    bytearray_reader reader(block.value().data(), block.value().size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = foreach_indirect_block(address, visitor);
    }

    return continue_;
}

bool block_iterator::foreach_triply_indirect_block(uint32_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    auto block = m_core.read_block(static_cast<uint64_t>(block_id));
    if (!block.has_value())
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = block.value().size() / address_size;
    bytearray_reader reader(block.value().data(), block.value().size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = foreach_doubly_indirect_block(address, visitor);
    }
    return continue_;
}


}