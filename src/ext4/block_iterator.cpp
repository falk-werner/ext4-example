#include "ext4/block_iterator.hpp"
#include "ext4/bytearray_reader.hpp"

#include <iostream>

namespace ext4
{

block_iterator::block_iterator(core_i & core)
: m_core(core)
, m_block(core.get_blocksize())
{

}

void block_iterator::foreach_block(inode const & inode, block_visitor visitor)
{
    foreach_direct_block(inode, visitor) &&
    foreach_indirect_block(inode.singly_indirect_blockpointers, visitor) &&
    foreach_doubly_indirect_block(inode.doubly_indirect_blockpointers, visitor) &&
    foreach_triply_indirect_block(inode.triply_indirect_blockpointers, visitor);

}

bool block_iterator::visit_block(uint64_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    if (!m_core.read_block(block_id, m_block))
    {
        std::cerr << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    try
    {            
        return visitor(m_block.data(), m_block.size());
    }
    catch (...)
    {
        std::cerr << "error: core: block visitor throws" << std::endl;
        return false;
    }
}

bool block_iterator::foreach_direct_block(inode const & inode, block_visitor & visitor)
{
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < ino_direct_blockpointers_size); i++)
    {
        uint64_t const block_id = static_cast<uint64_t>(inode.direct_blockpointers[i]);
        continue_ = visit_block(block_id, visitor);
    }

    return continue_;
}

bool block_iterator::foreach_indirect_block(uint32_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    if (!m_core.read_block(static_cast<uint64_t>(block_id), m_block))
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = m_block.size() / address_size;
    bytearray_reader reader(m_block.data(), m_block.size());
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

    if (!m_core.read_block(static_cast<uint64_t>(block_id), m_block))
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = m_block.size() / address_size;
    bytearray_reader reader(m_block.data(), m_block.size());
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

    if (!m_core.read_block(static_cast<uint64_t>(block_id), m_block))
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = m_block.size() / address_size;
    bytearray_reader reader(m_block.data(), m_block.size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = foreach_doubly_indirect_block(address, visitor);
    }
    return continue_;
}


}